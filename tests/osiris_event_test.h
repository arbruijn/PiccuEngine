#ifndef TESTS_OSIRIS_EVENT_TEST_H
#define TESTS_OSIRIS_EVENT_TEST_H

#include "osiris_common.h"

#if defined(__WATCOMC__)
#define OSIRIS_CALLEVENT_CDECL __cdecl
#else
#define OSIRIS_CALLEVENT_CDECL
#endif

short OSIRIS_CALLEVENT_CDECL CallInstanceEvent(int id, void* ptr, int event, tOSIRISEventInfo* data);
void OsirisEventCall_Test(int event, tOSIRISEventInfo* data);

#endif
