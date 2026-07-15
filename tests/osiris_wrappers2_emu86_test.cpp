#define INCLUDED_FROM_D3
#include "pstypes.h"
#include "osiris_wrappers2_callers.h"
#include "osiris_wrappers2_test.h"

#include "emu86.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

template <>
inline int* emu_arg<int*>(Emu* emu86, int n)
{
	return static_cast<int*>(emu_arg<void*>(emu86, n));
}

template <>
inline char* emu_arg<char*>(Emu* emu86, int n)
{
	return static_cast<char*>(emu_arg<void*>(emu86, n));
}

template <>
inline vector* emu_arg<vector*>(Emu* emu86, int n)
{
	return static_cast<vector*>(emu_arg<void*>(emu86, n));
}

template <>
inline matrix* emu_arg<matrix*>(Emu* emu86, int n)
{
	return static_cast<matrix*>(emu_arg<void*>(emu86, n));
}

template <>
inline const matrix* emu_arg<const matrix*>(Emu* emu86, int n)
{
	return static_cast<const matrix*>(emu_arg<void*>(emu86, n));
}

template <>
inline msafe_struct* emu_arg<msafe_struct*>(Emu* emu86, int n)
{
	return static_cast<msafe_struct*>(emu_arg<void*>(emu86, n));
}

template <>
inline ubyte emu_arg<ubyte>(Emu* emu86, int n)
{
	return static_cast<ubyte>(emu_arg<int>(emu86, n));
}

#include "../emuabi/osiris_wrappers2.cpp"

namespace
{
	static Emu* g_emu86 = 0;

	static emu_ptr_t FindExportDecorated(Emu* emu86, const std::string& name)
	{
		emu_ptr_t proc = emu86_find(emu86, name.c_str());
		if (proc)
		{
			return proc;
		}

		const std::string underscored = std::string("_") + name;
		proc = emu86_find(emu86, underscored.c_str());
		if (proc)
		{
			return proc;
		}

		const std::string stdcall = underscored + "@4";
		return emu86_find(emu86, stdcall.c_str());
	}

	static bool SetPointerHooks(Emu* emu86, const emu86_ctx_fun_t* funs, size_t count, const uint32_t* addresses)
	{
		for (size_t i = 0; i < count; ++i)
		{
			std::string setter_name = "set_";
			setter_name += funs[i].name;
			const emu_ptr_t setter = FindExportDecorated(emu86, setter_name);
			if (!setter)
			{
				fprintf(stderr, "failed to find setter export: %s\n", setter_name.c_str());
				return false;
			}

			if (emu86fun_call(emu86, setter, 1, addresses[i]) != 1u)
			{
				fprintf(stderr, "failed to set pointer hook: %s\n", setter_name.c_str());
				return false;
			}
		}

		return true;
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

	OsirisWrappers2_ResetLog();

	g_emu86 = emu86_load(dll_path);
	if (!g_emu86)
	{
		fprintf(stderr, "failed to load emu86 DLL: %s\n", dll_path);
		return 2;
	}

	emu86_set_trace(g_emu86, false);

	const size_t fun_count = sizeof(kOsirisWrapperFuns2) / sizeof(kOsirisWrapperFuns2[0]);
	uint32_t addresses[sizeof(kOsirisWrapperFuns2) / sizeof(kOsirisWrapperFuns2[0])] = {};
	const size_t added = emu86_add_fun_list_ctx(g_emu86, kOsirisWrapperFuns2, fun_count, addresses, fun_count);
	if (added != fun_count)
	{
		fprintf(stderr, "failed to register osiris wrappers2 host callbacks\n");
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	if (!SetPointerHooks(g_emu86, kOsirisWrapperFuns2, fun_count, addresses))
	{
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	const emu_ptr_t runner = FindExportDecorated(g_emu86, "run_all_osiris_wrappers2_test_callers");
	if (!runner)
	{
		fprintf(stderr, "failed to find required DLL export\n");
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	if (emu86fun_call(g_emu86, runner, 0) != 0u)
	{
		fprintf(stderr, "run_all_osiris_wrappers2_test_callers failed\n");
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	emu86_free(g_emu86);
	g_emu86 = 0;

	if (!OsirisWrappers2_WriteLog(actual_log_path))
	{
		fprintf(stderr, "failed to write actual log: %s\n", actual_log_path);
		return 2;
	}

	if (!OsirisWrappers2_FileExists(expected_log_path))
	{
		if (!OsirisWrappers2_WriteLog(expected_log_path))
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
