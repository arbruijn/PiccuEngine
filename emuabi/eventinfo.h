#ifndef EMUABI_EVENTINFO_H
#define EMUABI_EVENTINFO_H

#include "field_kinds.h"
#include "osiris_common.h"

namespace emuabi
{
const int event_info_size_32 = 28;
struct gb_com32
{
	int8_t action;
	int8_t index;
	uint16_t padding;
	uint32_t ptr;
};
static_assert(sizeof(gb_com32) == 8, "gb_com 32-bit ABI size mismatch");

void decode_gb_com(const gb_com32& src, gb_com& dst, const VmPtrDecoder& vm);
void encode_gb_com(const gb_com& src, gb_com32& dst, const VmPtrEncoder& vm);
void decode_event_info(int event, const void* src, tOSIRISEventInfo& dst, const VmPtrDecoder& vm);
void encode_event_info(int event, const tOSIRISEventInfo& src, void *dst, const VmPtrEncoder& vm, uint32_t* temp_buffer = nullptr);
void copy_event_info_temp_buffers(int event, const tOSIRISEventInfo& src, const VmPtrDecoder& vm, uint32_t temp_buffer);
void encode_event_info_with_file_handle(int event, const tOSIRISEventInfo& src, void *dst, const VmPtrEncoder& vm, uint32_t file_handle, uint32_t* temp_buffer = nullptr);
}

#endif
