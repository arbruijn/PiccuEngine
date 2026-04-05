#ifndef EMUABI_EVENTINFO_H
#define EMUABI_EVENTINFO_H

#include "field_kinds.h"
#include "../emu86/osiris32.h"

namespace emuabi
{
void encode_event_info(int event, const tOSIRISEventInfo& src, tOSIRISEventInfo32& dst, const VmPtrEncoder& vm);
}

#endif
