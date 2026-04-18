#include "osiris_event_test.h"

#include <stdarg.h>
#include <stdio.h>

typedef void (OSIRIS_CALLEVENT_CDECL *osiris_event_logger_fp)(const char* text);

namespace
{
	static osiris_event_logger_fp g_logger = 0;
}

extern "C" void OSIRIS_CALLEVENT_CDECL OsirisEventTestLog(int lvl, const char* format, ...)
{
	if (!g_logger)
	{
		return;
	}

	char buffer[4096];
	va_list args;
	va_start(args, format);
	const int written = vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	if (written < 0)
	{
		return;
	}

	buffer[sizeof(buffer) - 1] = '\0';
	g_logger(buffer);
}

extern "C" {

__declspec(dllexport) unsigned OSIRIS_CALLEVENT_CDECL set_osiris_event_logger(osiris_event_logger_fp fn)
{
	g_logger = fn;
	return g_logger != 0 ? 1u : 0u;
}

__declspec(dllexport) short OSIRIS_CALLEVENT_CDECL call_instance_event_export(int id, void* ptr, int event, tOSIRISEventInfo* data)
{
	return CallInstanceEvent(id, ptr, event, data);
}

}
