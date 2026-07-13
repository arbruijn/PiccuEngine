#ifndef EMUABI_EVENTINFO_H
#define EMUABI_EVENTINFO_H

#include "field_kinds.h"
#include "osiris_common.h"

namespace emuabi
{
const int event_info_size_32 = 28;
void encode_event_info(int event, const tOSIRISEventInfo& src, void *dst, const VmPtrEncoder& vm);
void encode_event_info_with_file_handle(int event, const tOSIRISEventInfo& src, void *dst, const VmPtrEncoder& vm, uint32_t file_handle);
}

#endif
