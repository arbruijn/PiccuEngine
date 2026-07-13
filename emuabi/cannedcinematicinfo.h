#ifndef EMUABI_CANNEDCINEMATICINFO_H
#define EMUABI_CANNEDCINEMATICINFO_H

#include "field_kinds.h"
#include "gamecinematics_external.h"

namespace emuabi
{
void decode_canned_cinematic_info_struct(const void* src, tCannedCinematicInfo& dst, const VmPtrDecoder& vm);
}

#endif
