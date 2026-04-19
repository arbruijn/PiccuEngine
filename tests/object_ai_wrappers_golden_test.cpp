#include "object_ai_wrappers_callers.h"
#include "object_ai_wrappers_test.h"

#include <stdio.h>
#include <stdlib.h>

#include <string>

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <expected-log> <actual-log>\n", argv[0]);
		return 2;
	}

	const char* expected_log_path = argv[1];
	const char* actual_log_path = argv[2];

	ObjectAI_ResetLog();
	RunAllObjectAITestCallers();

	if (!ObjectAI_WriteLog(actual_log_path))
	{
		fprintf(stderr, "failed to write actual log: %s\n", actual_log_path);
		return 2;
	}

	if (!ObjectAI_FileExists(expected_log_path))
	{
		if (!ObjectAI_WriteLog(expected_log_path))
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
