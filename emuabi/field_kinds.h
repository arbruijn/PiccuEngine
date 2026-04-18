#ifndef EMUABI_FIELD_KINDS_H
#define EMUABI_FIELD_KINDS_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

namespace emuabi
{
struct VmPtrEncoder
{
	const uint8_t* base;

	uint32_t encode_ptr32(const void* ptr) const
	{
		if (!ptr)
		{
			return 0;
		}

		assert(base != nullptr);

		const auto* address = static_cast<const uint8_t*>(ptr);
		assert(address >= base);

		const ptrdiff_t delta = address - base;
		assert(delta >= 0);
		assert(static_cast<uint64_t>(delta) <= UINT32_MAX);

		return static_cast<uint32_t>(delta);
	}
};

struct VmPtrDecoder
{
	uint8_t* base;

	void* decode_ptr32(uint32_t ptr32) const
	{
		if (ptr32 == 0)
		{
			return nullptr;
		}

		assert(base != nullptr);
		return base + ptr32;
	}

	template <typename T>
	T* decode_ptr32(uint32_t ptr32) const
	{
		return static_cast<T*>(decode_ptr32(ptr32));
	}
};
}

#endif
