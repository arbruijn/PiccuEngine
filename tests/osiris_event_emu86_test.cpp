#include "osiris_event_callers.h"

#include "emuabi/eventinfo.h"
#include "emu86.h"
#include "emuint.h"
#include "heap.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

bool Debug_break = false;
bool Debug_print_block = false;

extern Emu* curdll;

namespace
{
	static std::string g_log;
	static Emu* g_emu86 = 0;
	static emu_ptr_t g_call_instance_event = 0;
	static emu_ptr_t g_event_info_buffer = 0;

	static uint32_t VmTempAlloc(void* context, size_t size)
	{
		return heap_alloc(static_cast<Emu*>(context), static_cast<Emu*>(context)->process_heap, 0, static_cast<emu_ptr_t>(size));
	}

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

	static void* AliasVmOpaquePointer(void* ptr)
	{
		if (!ptr || !g_emu86)
		{
			return ptr;
		}

		uint8_t* const base = g_emu86->as.base;
		uint8_t* const address = static_cast<uint8_t*>(ptr);
		if (address >= base && address < base + g_emu86->as.size)
		{
			return ptr;
		}

		const uintptr_t raw = reinterpret_cast<uintptr_t>(ptr);
		if (raw >= g_emu86->as.size)
		{
			return ptr;
		}

		return base + static_cast<emu_ptr_t>(raw);
	}

	static tOSIRISEventInfo PrepareEventInfoForVmEncoding(int event, const tOSIRISEventInfo& src)
	{
		tOSIRISEventInfo encoded = src;
		if (!(event == EVT_AI_NOTIFY && encoded.evt_ai_notify.notify_type == AIN_USER_DEFINED))
			encoded.extra_info = AliasVmOpaquePointer(encoded.extra_info);

		switch (event)
		{
		case EVT_SAVESTATE:
			encoded.evt_savestate.fileptr = AliasVmOpaquePointer(encoded.evt_savestate.fileptr);
			break;
		case EVT_RESTORESTATE:
			encoded.evt_restorestate.fileptr = AliasVmOpaquePointer(encoded.evt_restorestate.fileptr);
			break;
		case EVT_MEMRESTORE:
			encoded.evt_memrestore.memory_ptr = AliasVmOpaquePointer(encoded.evt_memrestore.memory_ptr);
			break;
		default:
			break;
		}

		return encoded;
	}

	static void HostAppendOsirisEventLog(Emu86FunCtx& ctx, void *)
	{
		if (!curdll)
		{
			ctx.set_return(1u);
			return;
		}

		const unsigned text_ptr = ctx.arg<emu_ptr_t>(0);
		emuabi::VmPtrDecoder vm = { curdll->as.base };
		const char* text = vm.decode_ptr32<const char>(text_ptr);
		if (text)
		{
			g_log.append(text);
		}

		ctx.set_return(0u);
	}

	static const emu86_ctx_fun_t kOsirisEventWrapperFuns[] = {
		{"append_osiris_event_log", HostAppendOsirisEventLog, 1, 0},
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

short OSIRIS_CALLEVENT_CDECL CallInstanceEvent(int id, void* ptr, int event, tOSIRISEventInfo* data)
{
	if (!g_emu86 || !g_call_instance_event)
	{
		return 0;
	}

	unsigned data_ptr = 0u;
	if (data)
	{
		emuabi::VmPtrEncoder vm = { g_emu86->as.base, VmTempAlloc, g_emu86 };
		const tOSIRISEventInfo encoded = PrepareEventInfoForVmEncoding(event, *data);
		emu_ptr_t temp_buffer = 0;
		emuabi::encode_event_info(event, encoded, g_emu86->as.base + g_event_info_buffer, vm, &temp_buffer);
		data_ptr = g_event_info_buffer;
		const unsigned ret = emu86fun_call(
			g_emu86,
			g_call_instance_event,
			4,
			static_cast<unsigned>(id),
			static_cast<unsigned>(reinterpret_cast<uintptr_t>(ptr)),
			static_cast<unsigned>(event),
			data_ptr);
		emuabi::copy_event_info_temp_buffers(event, encoded, {g_emu86->as.base}, temp_buffer);
		if (temp_buffer)
			heap_free(g_emu86, g_emu86->process_heap, 0, temp_buffer);
		return static_cast<short>(ret);
	}

	return static_cast<short>(emu86fun_call(
		g_emu86,
		g_call_instance_event,
		4,
		static_cast<unsigned>(id),
		static_cast<unsigned>(reinterpret_cast<uintptr_t>(ptr)),
		static_cast<unsigned>(event),
		data_ptr));
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

	g_emu86 = emu86_load(dll_path);
	if (!g_emu86)
	{
		fprintf(stderr, "failed to load emu86 DLL: %s\n", dll_path);
		return 2;
	}

	emu_ptr_t logger_address = 0;
	if (emu86_add_fun_list_ctx(g_emu86, kOsirisEventWrapperFuns, 1, &logger_address, 1) != 1 || !logger_address)
	{
		fprintf(stderr, "failed to register osiris event logger wrapper\n");
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	const char* const setter_candidates[] = {
		"set_osiris_event_logger",
		"_set_osiris_event_logger",
		"_set_osiris_event_logger@4",
	};
	const char* const callevent_candidates[] = {
		"call_instance_event_export",
		"_call_instance_event_export",
		"_call_instance_event_export@16",
	};

	const emu_ptr_t setter = FindExport(g_emu86, setter_candidates, sizeof(setter_candidates) / sizeof(setter_candidates[0]));
	g_call_instance_event = FindExport(g_emu86, callevent_candidates, sizeof(callevent_candidates) / sizeof(callevent_candidates[0]));
	if (!setter || !g_call_instance_event)
	{
		fprintf(stderr, "failed to find required DLL exports\n");
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	g_event_info_buffer = heap_alloc(g_emu86, emuabi::event_info_size_32);
	if (!g_event_info_buffer)
	{
		fprintf(stderr, "failed to allocate emu86 event info buffer\n");
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	if (emu86fun_call(g_emu86, setter, 1, logger_address) != 1u)
	{
		fprintf(stderr, "set_osiris_event_logger failed\n");
		emu86_free(g_emu86);
		g_emu86 = 0;
		return 2;
	}

	RunAllOsirisEventTestCallers();

	emu86_free(g_emu86);
	g_emu86 = 0;
	g_call_instance_event = 0;
	g_event_info_buffer = 0;

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
