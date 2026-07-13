#include "osiris_module_init.h"

#include "heap.h"

#include <algorithm>
#include <cstring>

namespace emuabi
{
namespace
{
static emu_ptr_t vm_alloc(Emu* emu86, size_t size)
{
	if (!emu86 || !size)
		return 0;
	return heap_alloc(emu86, emu86->process_heap, 0, static_cast<emu_ptr_t>(size));
}
}

emu_ptr_t copy_string_to_vm(Emu* emu86, const char* src)
{
	if (!src)
		return 0;

	const size_t len = std::strlen(src) + 1;
	emu_ptr_t guest = vm_alloc(emu86, len);
	if (!guest)
		return 0;

	std::memcpy(emu86->as.base + guest, src, len);
	return guest;
}

emu_ptr_t copy_string_table_to_vm(Emu* emu86, char** src, int count)
{
	if (!emu86 || !src || count <= 0)
		return 0;

	emu_ptr_t table_guest = vm_alloc(emu86, static_cast<size_t>(count) * sizeof(uint32_t));
	if (!table_guest)
		return 0;

	uint32_t* table = reinterpret_cast<uint32_t*>(emu86->as.base + table_guest);
	std::memset(table, 0, static_cast<size_t>(count) * sizeof(uint32_t));
	for (int i = 0; i < count; ++i)
	{
		if (!src[i])
		{
			table[i] = 0;
			continue;
		}

		table[i] = copy_string_to_vm(emu86, src[i]);
	}

	return table_guest;
}

bool encode_osiris_module_init(Emu* emu86, const tOSIRISModuleInit& src, const uint32_t* import_thunks, size_t import_count, OsirisModuleInit32& dst)
{
	if (!emu86)
		return false;

	std::memset(&dst, 0, sizeof(dst));

	const size_t thunk_count = std::min(import_count, static_cast<size_t>(MAX_MODULEFUNCS));
	for (size_t i = 0; i < thunk_count; ++i)
		dst.fp[i] = import_thunks ? import_thunks[i] : 0;

	for (size_t i = thunk_count; i < MAX_MODULEFUNCS; ++i)
		dst.fp[i] = 0;

	dst.string_count = src.string_count;
	dst.module_identifier = src.module_identifier;
	dst.module_is_static = src.module_is_static ? 1u : 0u;
	dst.game_checksum = src.game_checksum;

	if (src.string_count > 0 && src.string_table)
		dst.string_table = copy_string_table_to_vm(emu86, src.string_table, src.string_count);
	else
		dst.string_table = 0;

	dst.script_identifier = copy_string_to_vm(emu86, src.script_identifier);
	return true;
}
}
