#ifndef EMUABI_MSAFE_H
#define EMUABI_MSAFE_H

#include "field_kinds.h"
#include "osiris_common.h"

namespace emuabi
{
void decode_msafe_struct(int type, const void* src, msafe_struct& dst, const VmPtrDecoder& vm);
}

#endif
