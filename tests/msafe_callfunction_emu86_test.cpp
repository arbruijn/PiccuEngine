#include "msafe_callfunction_test.h"

#include "emuabi/msafe.h"
#include "emu86.h"
#include "emuint.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

bool Debug_break = false;
bool Debug_print_block = false;

extern Emu* curdll;

namespace
{
	static std::string g_log;

	static void AppendFormatted(const char* format, va_list args)
	{
		char buffer[4096];
		va_list copy;
		va_copy(copy, args);
		const int written = vsnprintf(buffer, sizeof(buffer), format, copy);
		va_end(copy);

		if (written < 0)
		{
			return;
		}

		if (written < static_cast<int>(sizeof(buffer)))
		{
			g_log.append(buffer, written);
			return;
		}

		std::string dynamic_buffer;
		dynamic_buffer.resize(static_cast<size_t>(written) + 1);
		vsnprintf(&dynamic_buffer[0], dynamic_buffer.size(), format, args);
		g_log.append(dynamic_buffer.c_str(), static_cast<size_t>(written));
	}

	static bool FileExists(const char* path)
	{
		FILE* file = fopen(path, "rb");
		if (!file)
		{
			return false;
		}
		fclose(file);
		return true;
	}

	static bool WriteFile(const char* path, const std::string& contents)
	{
		FILE* file = fopen(path, "wb");
		if (!file)
		{
			return false;
		}

		const size_t written = fwrite(contents.data(), 1, contents.size(), file);
		fclose(file);
		return written == contents.size();
	}

	static emu_ptr_t FindExport(Emu* emu86, const char* const* names, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			emu_ptr_t proc = emu86_find(emu86, names[i]);
			if (proc)
			{
				return proc;
			}
		}

		return 0;
	}

	static void HostMSafeCallFunction(Emu86FunCtx& ctx, void *)
	{
		if (!curdll)
		{
			ctx.set_return(1u);
			return;
		}

		const unsigned type = ctx.arg<emu_ptr_t>(0);
		const unsigned mstruct_ptr = ctx.arg<emu_ptr_t>(1);
		emuabi::VmPtrDecoder vm = { curdll->as.base };
		msafe_struct decoded;
		emuabi::decode_msafe_struct(static_cast<int>(type), vm.decode_ptr32<void>(mstruct_ptr), decoded, vm);
		msafe_CallFunction_Test(static_cast<ubyte>(type), &decoded);
		ctx.set_return(0u);
	}

	static const emu86_ctx_fun_t kMSafeWrapperFuns[] = {
		{"msafe_CallFunction", HostMSafeCallFunction, 2, 0},
	};
}

void Debug_ConsolePrintf(int n, char* format, ...)
{
	(void)n;
	va_list args;
	va_start(args, format);
	AppendFormatted(format, args);
	va_end(args);
}

void Debug_ConsolePrintf(int n, int row, int col, char* format, ...)
{
	(void)n;
	(void)row;
	(void)col;
	va_list args;
	va_start(args, format);
	AppendFormatted(format, args);
	va_end(args);
}

int main(int argc, char** argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "usage: %s <expected-log> <actual-log> <dll>\n", argv[0]);
		return 2;
	}

	const char* expected_log_path = argv[1];
	const char* actual_log_path = argv[2];
	const char* dll_path = argv[3];

	g_log.clear();

	emu86_init();
	Emu* emu86 = emu86_load(dll_path);
	if (!emu86)
	{
		fprintf(stderr, "failed to load emu86 DLL: %s\n", dll_path);
		emu86_done();
		return 2;
	}

	emu_ptr_t wrapper_address = 0;
	if (emu86_add_fun_list_ctx(emu86, kMSafeWrapperFuns, 1, &wrapper_address, 1) != 1 || !wrapper_address)
	{
		fprintf(stderr, "failed to register msafe_CallFunction wrapper\n");
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	const char* const setter_candidates[] = {
		"set_msafe_CallFunction",
		"_set_msafe_CallFunction",
		"_set_msafe_CallFunction@4",
	};
	const char* const runner_candidates[] = {
		"run_all_msafe_callfunction_test_callers",
		"_run_all_msafe_callfunction_test_callers",
		"_run_all_msafe_callfunction_test_callers@0",
	};

	const emu_ptr_t setter = FindExport(emu86, setter_candidates, sizeof(setter_candidates) / sizeof(setter_candidates[0]));
	const emu_ptr_t runner = FindExport(emu86, runner_candidates, sizeof(runner_candidates) / sizeof(runner_candidates[0]));
	if (!setter || !runner)
	{
		fprintf(stderr, "failed to find required DLL exports\n");
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	if (emu86fun_call(emu86, setter, 1, wrapper_address) != 1u)
	{
		fprintf(stderr, "set_msafe_CallFunction failed\n");
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	if (emu86fun_call(emu86, runner, 0) != 0u)
	{
		fprintf(stderr, "run_all_msafe_callfunction_test_callers failed\n");
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	emu86_free(emu86);
	emu86_done();

	if (!WriteFile(actual_log_path, g_log))
	{
		fprintf(stderr, "failed to write actual log: %s\n", actual_log_path);
		return 2;
	}

	if (!FileExists(expected_log_path))
	{
		if (!WriteFile(expected_log_path, g_log))
		{
			fprintf(stderr, "failed to create expected log: %s\n", expected_log_path);
			return 2;
		}
		fprintf(stdout, "created expected log: %s\n", expected_log_path);
		return 0;
	}

	std::string command = "diff -u \"";
	command += expected_log_path;
	command += "\" \"";
	command += actual_log_path;
	command += "\"";
	return system(command.c_str());
}
