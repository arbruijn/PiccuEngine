#ifndef EMUABI_MSAFE_H
#define EMUABI_MSAFE_H

#include "field_kinds.h"
#include "osiris_common.h"
#include "gamecinematics_external.h"

namespace emuabi
{
const int msafe_struct_size_32 = 920;
const int osiris_timer_struct_size_32 = 24;
const int canned_cinematic_info_struct_size_32 = 80;
const int game_cinematic_struct_size_32 = 88;
const int memchunk_struct_size_32 = 16;
void decode_msafe_struct(int type, const void* src, msafe_struct& dst, const VmPtrDecoder& vm);
void encode_msafe_struct(int type, const msafe_struct& src, void* dst, const VmPtrEncoder& vm);
void decode_msafe_getvalue_struct(int type, const void* src, msafe_struct& dst, const VmPtrDecoder& vm);
void encode_msafe_getvalue_struct(int type, const msafe_struct& src, void* dst, const VmPtrEncoder& vm);
void decode_msafe_do_powerup_struct(const void* src, msafe_struct& dst, const VmPtrDecoder& vm);
void encode_msafe_do_powerup_struct(const msafe_struct& src, void* dst, const VmPtrEncoder& vm);
void decode_osiris_timer_struct(const void* src, tOSIRISTIMER& dst);
void decode_osiris_memchunk_struct(const void* src, tOSIRISMEMCHUNK& dst);
void decode_game_cinematic_struct(const void* src, tGameCinematic& dst, const VmPtrDecoder& vm);
void decode_canned_cinematic_info_struct(const void* src, tCannedCinematicInfo& dst, const VmPtrDecoder& vm);
}

#endif
