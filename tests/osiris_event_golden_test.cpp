#include "osiris_event_callers.h"
#include "emuabi/cfile_wrappers.h"
#include "emuabi/eventinfo.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

bool Debug_break = false;
bool Debug_print_block = false;

namespace emuabi
{
	CFILE* resolve_file_handle(emu_ptr_t)
	{
		return nullptr;
	}
}

namespace
{
	static std::string g_log;

	static void TestGbCom()
	{
		uint8_t vm_memory[256] = {};
		gb_com source = {};
		source.action = COM_DO_ACTION;
		source.index = 17;
		source.ptr = vm_memory + 0x24;

		emuabi::gb_com32 encoded = {};
		emuabi::VmPtrEncoder encoder = {vm_memory};
		emuabi::encode_gb_com(source, encoded, encoder);

		gb_com decoded = {};
		emuabi::VmPtrDecoder decoder = {vm_memory};
		emuabi::decode_gb_com(encoded, decoded, decoder);

		g_log += "gb_com encoded.action=" + std::to_string(static_cast<int>(encoded.action)) +
			" index=" + std::to_string(static_cast<int>(encoded.index)) +
			" ptr=" + std::to_string(encoded.ptr) + "\n";
		g_log += "gb_com decoded.action=" + std::to_string(static_cast<int>(decoded.action)) +
			" index=" + std::to_string(static_cast<int>(decoded.index)) +
			" ptr_offset=" + std::to_string(static_cast<uint8_t*>(decoded.ptr) - vm_memory) + "\n";

		source.ptr = nullptr;
		emuabi::encode_gb_com(source, encoded, encoder);
		g_log += "gb_com null_ptr=" + std::to_string(encoded.ptr) + "\n";
	}

	static void AppendFormatted(const char* format, va_list args)
	{
		char buffer[4096];
		va_list copy;
		va_copy(copy, args);
		const int written = vsnprintf(buffer, sizeof(buffer), format, copy);
		va_end(copy);

		if (written < 0)
			return;

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
			return false;
		fclose(file);
		return true;
	}

	static bool WriteFile(const char* path, const std::string& contents)
	{
		FILE* file = fopen(path, "wb");
		if (!file)
			return false;
		const size_t written = fwrite(contents.data(), 1, contents.size(), file);
		fclose(file);
		return written == contents.size();
	}
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
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <expected-log> <actual-log>\n", argv[0]);
		return 2;
	}

	const char* expected_log_path = argv[1];
	const char* actual_log_path = argv[2];

	g_log.clear();
	RunAllOsirisEventTestCallers();
	TestGbCom();

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
