#include "osiris_import_runtime.h"

#include "osiris_import_test_list.h"

#ifndef __LINUX__
#include <stddef.h>
#endif
#include "osiris_import.h"

#include <stdint.h>

namespace
{
	typedef unsigned (*raw_fun0)(void);
	typedef unsigned (*raw_fun1)(unsigned);
	typedef unsigned (*raw_fun2)(unsigned, unsigned);
	typedef unsigned (*raw_fun3)(unsigned, unsigned, unsigned);
	typedef unsigned (*raw_fun4)(unsigned, unsigned, unsigned, unsigned);
	typedef unsigned (*raw_fun5)(unsigned, unsigned, unsigned, unsigned, unsigned);
	typedef unsigned (*raw_fun6)(unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
	typedef unsigned (*raw_fun7)(unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
	typedef unsigned (*raw_fun8)(unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);

	template <typename TypedFunc>
	uintptr_t RawAddress(TypedFunc func)
	{
		union
		{
			TypedFunc typed;
			uintptr_t raw;
		} casted;

		casted.typed = func;
		return casted.raw;
	}

	template <typename TypedFunc>
	unsigned CallRaw0(TypedFunc func)
	{
		union
		{
			TypedFunc typed;
			raw_fun0 raw;
		} casted;

		casted.typed = func;
		return casted.raw();
	}

	template <typename TypedFunc>
	unsigned CallRaw1(TypedFunc func, unsigned a1)
	{
		union
		{
			TypedFunc typed;
			raw_fun1 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1);
	}

	template <typename TypedFunc>
	unsigned CallRaw2(TypedFunc func, unsigned a1, unsigned a2)
	{
		union
		{
			TypedFunc typed;
			raw_fun2 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1, a2);
	}

	template <typename TypedFunc>
	unsigned CallRaw3(TypedFunc func, unsigned a1, unsigned a2, unsigned a3)
	{
		union
		{
			TypedFunc typed;
			raw_fun3 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1, a2, a3);
	}

	template <typename TypedFunc>
	unsigned CallRaw4(TypedFunc func, unsigned a1, unsigned a2, unsigned a3, unsigned a4)
	{
		union
		{
			TypedFunc typed;
			raw_fun4 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1, a2, a3, a4);
	}

	template <typename TypedFunc>
	unsigned CallRaw5(TypedFunc func, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5)
	{
		union
		{
			TypedFunc typed;
			raw_fun5 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1, a2, a3, a4, a5);
	}

	template <typename TypedFunc>
	unsigned CallRaw6(TypedFunc func, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6)
	{
		union
		{
			TypedFunc typed;
			raw_fun6 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1, a2, a3, a4, a5, a6);
	}

	template <typename TypedFunc>
	unsigned CallRaw7(TypedFunc func, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7)
	{
		union
		{
			TypedFunc typed;
			raw_fun7 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename TypedFunc>
	unsigned CallRaw8(TypedFunc func, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7, unsigned a8)
	{
		union
		{
			TypedFunc typed;
			raw_fun8 raw;
		} casted;

		casted.typed = func;
		return casted.raw(a1, a2, a3, a4, a5, a6, a7, a8);
	}

	static unsigned TestArg(unsigned slot, unsigned arg_index)
	{
		return 0xA5000000u | (slot << 8) | arg_index;
	}

	template <typename TypedFunc>
	int VerifyImport(unsigned slot, TypedFunc func, int* expected)
	{
		if (!func)
		{
			return static_cast<int>(slot);
		}

		if (RawAddress(func) != reinterpret_cast<uintptr_t>(expected))
		{
			return static_cast<int>(slot);
		}

		return 0;
	}

	template <typename TypedFunc>
	int CallImport(unsigned slot, TypedFunc func, int argc)
	{
		if (!func)
		{
			return static_cast<int>(slot);
		}

		switch (argc)
		{
		case 0:
			(void)CallRaw0(func);
			break;
		case 1:
			(void)CallRaw1(func, TestArg(slot, 1));
			break;
		case 2:
			(void)CallRaw2(func, TestArg(slot, 1), TestArg(slot, 2));
			break;
		case 3:
			(void)CallRaw3(func, TestArg(slot, 1), TestArg(slot, 2), TestArg(slot, 3));
			break;
		case 4:
			(void)CallRaw4(func, TestArg(slot, 1), TestArg(slot, 2), TestArg(slot, 3), TestArg(slot, 4));
			break;
		case 5:
			(void)CallRaw5(func, TestArg(slot, 1), TestArg(slot, 2), TestArg(slot, 3), TestArg(slot, 4), TestArg(slot, 5));
			break;
		case 6:
			(void)CallRaw6(func, TestArg(slot, 1), TestArg(slot, 2), TestArg(slot, 3), TestArg(slot, 4), TestArg(slot, 5), TestArg(slot, 6));
			break;
		case 7:
			(void)CallRaw7(func, TestArg(slot, 1), TestArg(slot, 2), TestArg(slot, 3), TestArg(slot, 4), TestArg(slot, 5), TestArg(slot, 6), TestArg(slot, 7));
			break;
		case 8:
			(void)CallRaw8(func, TestArg(slot, 1), TestArg(slot, 2), TestArg(slot, 3), TestArg(slot, 4), TestArg(slot, 5), TestArg(slot, 6), TestArg(slot, 7), TestArg(slot, 8));
			break;
		default:
			return static_cast<int>(slot);
		}

		return 0;
	}
}

int InitializeOsirisImportTest(tOSIRISModuleInit* module_init)
{
	if (!module_init)
	{
		return -1;
	}

	osicommon_Initialize(module_init);

	unsigned slot = 0;

#define VERIFY_IMPORT(symbol, argc) \
	do \
	{ \
		++slot; \
		const int result = VerifyImport(slot, symbol, module_init->fp[slot - 1]); \
		if (result != 0) \
		{ \
			return result; \
		} \
	} while (0);

	OSIRIS_IMPORT_TEST_LIST(VERIFY_IMPORT);

#undef VERIFY_IMPORT

	return 0;
}

int RunAllOsirisImportTestCallers(void)
{
	unsigned slot = 0;

#define CALL_IMPORT(symbol, argc) \
	do \
	{ \
		++slot; \
		const int result = CallImport(slot, symbol, argc); \
		if (result != 0) \
		{ \
			return result; \
		} \
	} while (0);

	OSIRIS_IMPORT_TEST_LIST(CALL_IMPORT);

#undef CALL_IMPORT

	return 0;
}
