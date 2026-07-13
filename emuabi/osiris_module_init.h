#ifndef EMUABI_OSIRIS_MODULE_INIT_H
#define EMUABI_OSIRIS_MODULE_INIT_H

#include "emu86.h"
#include "osiris_common.h"

#include <stdint.h>
#include <vector>

namespace emuabi
{
struct OsirisModuleInit32
{
	uint32_t fp[MAX_MODULEFUNCS];
	uint32_t string_table;
	int32_t string_count;
	int32_t module_identifier;
	uint32_t module_is_static;
	uint32_t script_identifier;
	uint32_t game_checksum;
};

bool encode_osiris_module_init(Emu* emu86, const tOSIRISModuleInit& src, const uint32_t* import_thunks, size_t import_count, OsirisModuleInit32& dst);
emu_ptr_t copy_string_to_vm(Emu* emu86, const char* src);
emu_ptr_t copy_string_table_to_vm(Emu* emu86, char** src, int count);
}

#endif
