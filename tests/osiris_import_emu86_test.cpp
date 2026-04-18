#include "osiris_import_host.h"

#include "emu86.h"
#include "emuint.h"
#include "heap.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>

bool Debug_break = false;
bool Debug_print_block = false;

extern Emu* curdll;

namespace
{
	struct OsirisModuleInitFpTable32
	{
		uint32_t fp[MAX_MODULEFUNCS];
	};

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
			const emu_ptr_t proc = emu86_find(emu86, names[i]);
			if (proc)
			{
				return proc;
			}
		}

		return 0;
	}
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

	ResetOsirisImportHostLog();

	emu86_init();
	Emu* emu86 = emu86_load(dll_path);
	if (!emu86)
	{
		fprintf(stderr, "failed to load emu86 DLL: %s\n", dll_path);
		emu86_done();
		return 2;
	}

	size_t wrapper_count = 0;
	const emu86_fun_t* wrappers = GetOsirisImportEmu86FunTable(&wrapper_count);
	std::vector<emu_ptr_t> wrapper_addresses(wrapper_count);
	if (emu86_add_fun_list(emu86, wrappers, wrapper_count, &wrapper_addresses[0], wrapper_count) != wrapper_count)
	{
		fprintf(stderr, "failed to register Osiris import wrappers\n");
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	const char* const initializer_candidates[] = {
		"initialize_osiris_import_test",
		"_initialize_osiris_import_test",
		"_initialize_osiris_import_test@4",
	};
	const char* const runner_candidates[] = {
		"run_all_osiris_import_test_callers",
		"_run_all_osiris_import_test_callers",
		"_run_all_osiris_import_test_callers@0",
	};

	const emu_ptr_t initializer = FindExport(emu86, initializer_candidates, sizeof(initializer_candidates) / sizeof(initializer_candidates[0]));
	const emu_ptr_t runner = FindExport(emu86, runner_candidates, sizeof(runner_candidates) / sizeof(runner_candidates[0]));
	if (!initializer || !runner)
	{
		fprintf(stderr, "failed to find required DLL exports\n");
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	const emu_ptr_t module_init_ptr = heap_alloc(emu86, sizeof(OsirisModuleInitFpTable32));
	if (!module_init_ptr)
	{
		fprintf(stderr, "failed to allocate emu86 module init buffer\n");
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	OsirisModuleInitFpTable32 module_init;
	memset(&module_init, 0, sizeof(module_init));
	for (size_t i = 0; i < wrapper_count && i < MAX_MODULEFUNCS; ++i)
	{
		module_init.fp[i] = static_cast<uint32_t>(wrapper_addresses[i]);
	}
	memcpy(emu86->as.base + module_init_ptr, &module_init, sizeof(module_init));

	const unsigned initialize_result = emu86fun_call(emu86, initializer, 1, module_init_ptr);
	if (initialize_result != 0u)
	{
		fprintf(stderr, "initialize_osiris_import_test failed at slot %u\n", initialize_result);
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	const unsigned run_result = emu86fun_call(emu86, runner, 0);
	if (run_result != 0u)
	{
		fprintf(stderr, "run_all_osiris_import_test_callers failed at slot %u\n", run_result);
		emu86_free(emu86);
		emu86_done();
		return 2;
	}

	emu86_free(emu86);
	emu86_done();

	const std::string& log = GetOsirisImportHostLog();
	if (!WriteFile(actual_log_path, log))
	{
		fprintf(stderr, "failed to write actual log: %s\n", actual_log_path);
		return 2;
	}

	if (!FileExists(expected_log_path))
	{
		if (!WriteFile(expected_log_path, log))
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
