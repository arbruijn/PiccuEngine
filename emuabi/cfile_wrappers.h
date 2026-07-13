#ifndef EMUABI_CFILE_WRAPPERS_H
#define EMUABI_CFILE_WRAPPERS_H

#include "emu86.h"

struct CFILE;

namespace emuabi
{
emu_ptr_t register_file_handle(void* native_ptr);
void unregister_file_handle(emu_ptr_t guest_handle);
CFILE* resolve_file_handle(emu_ptr_t guest_handle);
}

#endif
