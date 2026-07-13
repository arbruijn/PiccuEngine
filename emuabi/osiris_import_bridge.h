#ifndef EMUABI_OSIRIS_IMPORT_BRIDGE_H
#define EMUABI_OSIRIS_IMPORT_BRIDGE_H

#include "field_kinds.h"
#include "emu86.h"
#include "osiris_imports_shared.h"

#include <stdint.h>
#include <vector>

namespace emuabi
{
static inline unsigned int osiris_import_spec_count()
{
	unsigned int count = 0;
	(void)Osiris_GetImportSpecTable(&count);
	return count;
}

static inline const OsirisImportSpec* osiris_import_spec(unsigned int slot)
{
	return Osiris_GetImportSpec(slot);
}

static inline int import_requires_guest_thunk(const OsirisImportSpec* spec)
{
	return Osiris_ImportRequiresGuestThunk(spec);
}

static inline int import_uses_x87_return(const OsirisImportSpec* spec)
{
	return Osiris_ImportReturnUsesX87(spec);
}

static inline int import_uses_struct_return(const OsirisImportSpec* spec)
{
	return Osiris_ImportReturnUsesStructSret(spec);
}

static inline int import_needs_vm_pointer_bridge(const OsirisImportSpec* spec)
{
	return Osiris_ImportNeedsVmPointerBridge(spec);
}

static inline int import_needs_opaque_handle_bridge(const OsirisImportSpec* spec)
{
	return Osiris_ImportNeedsOpaqueHandleBridge(spec);
}

static inline int import_returns_guest_visible_memory(const OsirisImportSpec* spec)
{
	return Osiris_ImportReturnsGuestVisibleMemory(spec);
}

static inline size_t Osiris_BuildGuestThunkTable(Emu* emu86, const emu86_ctx_fun_t* funs, size_t count, uint32_t* addresses, size_t address_count)
{
	return emu86_add_fun_list_ctx(emu86, funs, count, addresses, address_count);
}

class OpaqueHandleTable
{
public:
	OpaqueHandleTable()
		: next_handle_(1)
	{
	}

	uint32_t Insert(void* ptr)
	{
		if (!ptr)
		{
			return 0;
		}

		for (size_t i = 0; i < entries_.size(); ++i)
		{
			if (entries_[i].ptr == ptr)
			{
				return entries_[i].handle;
			}
		}

		Entry entry;
		entry.handle = NextHandle();
		entry.ptr = ptr;
		entries_.push_back(entry);
		return entry.handle;
	}

	void* Find(uint32_t handle) const
	{
		if (handle == 0)
		{
			return 0;
		}

		for (size_t i = 0; i < entries_.size(); ++i)
		{
			if (entries_[i].handle == handle)
			{
				return entries_[i].ptr;
			}
		}

		return 0;
	}

	void Erase(uint32_t handle)
	{
		if (handle == 0)
		{
			return;
		}

		for (size_t i = 0; i < entries_.size(); ++i)
		{
			if (entries_[i].handle == handle)
			{
				entries_.erase(entries_.begin() + i);
				return;
			}
		}
	}

	void Clear()
	{
		entries_.clear();
		next_handle_ = 1;
	}

private:
	struct Entry
	{
		uint32_t handle;
		void* ptr;
	};

	uint32_t NextHandle()
	{
		for (;;)
		{
			uint32_t candidate = next_handle_++;
			if (candidate != 0 && !Find(candidate))
			{
				return candidate;
			}
		}
	}

	std::vector<Entry> entries_;
	uint32_t next_handle_;
};
}

#endif
