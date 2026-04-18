#include "osiris_import_host.h"
#include "osiris_import_runtime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

namespace
{
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
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <expected-log> <actual-log>\n", argv[0]);
		return 2;
	}

	const char* expected_log_path = argv[1];
	const char* actual_log_path = argv[2];

	tOSIRISModuleInit module_init;
	memset(&module_init, 0, sizeof(module_init));

	ResetOsirisImportHostLog();
	PopulateOsirisImportModuleInit(&module_init);

	const int initialize_result = InitializeOsirisImportTest(&module_init);
	if (initialize_result != 0)
	{
		fprintf(stderr, "InitializeOsirisImportTest failed at slot %d\n", initialize_result);
		return 2;
	}

	const int run_result = RunAllOsirisImportTestCallers();
	if (run_result != 0)
	{
		fprintf(stderr, "RunAllOsirisImportTestCallers failed at slot %d\n", run_result);
		return 2;
	}

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
