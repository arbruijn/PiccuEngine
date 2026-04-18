#include "emu86.h"
#include "osiris_import_host.h"

#include "osiris_imports_shared.h"

#include <stdio.h>

namespace
{
	static std::string g_log;

	template <typename TypedFunc>
	int* ModuleInitPointer(TypedFunc func)
	{
		union
		{
			TypedFunc typed;
			int* raw;
		} casted;

		casted.typed = func;
		return casted.raw;
	}

	static unsigned LogImportCall(const char* name, int argc, const unsigned* args)
	{
		char buffer[512];
		int written = snprintf(buffer, sizeof(buffer), "%s argc=%d", name, argc);
		if (written < 0)
		{
			return 0u;
		}

		for (int i = 0; i < argc && written > 0 && written < static_cast<int>(sizeof(buffer)); ++i)
		{
			written += snprintf(buffer + written, sizeof(buffer) - static_cast<size_t>(written), " a%d=0x%08X", i + 1, args[i]);
		}

		if (written > 0 && written < static_cast<int>(sizeof(buffer)))
		{
			buffer[written++] = '\n';
			g_log.append(buffer, static_cast<size_t>(written));
		}

		return 0u;
	}

#define DEFINE_WRAPPER_0(symbol) \
	static unsigned symbol##_Wrapper(void) \
	{ \
		return LogImportCall(#symbol, 0, 0); \
	}

#define DEFINE_WRAPPER_1(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1) \
	{ \
		const unsigned args[] = { a1 }; \
		return LogImportCall(#symbol, 1, args); \
	}

#define DEFINE_WRAPPER_2(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1, unsigned a2) \
	{ \
		const unsigned args[] = { a1, a2 }; \
		return LogImportCall(#symbol, 2, args); \
	}

#define DEFINE_WRAPPER_3(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1, unsigned a2, unsigned a3) \
	{ \
		const unsigned args[] = { a1, a2, a3 }; \
		return LogImportCall(#symbol, 3, args); \
	}

#define DEFINE_WRAPPER_4(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1, unsigned a2, unsigned a3, unsigned a4) \
	{ \
		const unsigned args[] = { a1, a2, a3, a4 }; \
		return LogImportCall(#symbol, 4, args); \
	}

#define DEFINE_WRAPPER_5(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5) \
	{ \
		const unsigned args[] = { a1, a2, a3, a4, a5 }; \
		return LogImportCall(#symbol, 5, args); \
	}

#define DEFINE_WRAPPER_6(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6) \
	{ \
		const unsigned args[] = { a1, a2, a3, a4, a5, a6 }; \
		return LogImportCall(#symbol, 6, args); \
	}

#define DEFINE_WRAPPER_7(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7) \
	{ \
		const unsigned args[] = { a1, a2, a3, a4, a5, a6, a7 }; \
		return LogImportCall(#symbol, 7, args); \
	}

#define DEFINE_WRAPPER_8(symbol) \
	static unsigned symbol##_Wrapper(unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7, unsigned a8) \
	{ \
		const unsigned args[] = { a1, a2, a3, a4, a5, a6, a7, a8 }; \
		return LogImportCall(#symbol, 8, args); \
	}

#define DEFINE_WRAPPER(symbol, argc) DEFINE_WRAPPER_##argc(symbol)
#define DEFINE_WRAPPER_ENTRY(symbol, guest_type, host_symbol, abi_argc, test_argc, return_kind, bridge_kind) DEFINE_WRAPPER(symbol, test_argc)
	OSIRIS_IMPORT_LIST(DEFINE_WRAPPER_ENTRY);
#undef DEFINE_WRAPPER_ENTRY
#undef DEFINE_WRAPPER

	static const emu86_fun_t kOsirisImportWrapperFuns[] = {
#define EMU86_ENTRY_0(symbol) { #symbol, 0, { .fun0 = symbol##_Wrapper } },
#define EMU86_ENTRY_1(symbol) { #symbol, 1, { .fun1 = symbol##_Wrapper } },
#define EMU86_ENTRY_2(symbol) { #symbol, 2, { .fun2 = symbol##_Wrapper } },
#define EMU86_ENTRY_3(symbol) { #symbol, 3, { .fun3 = symbol##_Wrapper } },
#define EMU86_ENTRY_4(symbol) { #symbol, 4, { .fun4 = symbol##_Wrapper } },
#define EMU86_ENTRY_5(symbol) { #symbol, 5, { .fun5 = symbol##_Wrapper } },
#define EMU86_ENTRY_6(symbol) { #symbol, 6, { .fun6 = symbol##_Wrapper } },
#define EMU86_ENTRY_7(symbol) { #symbol, 7, { .fun7 = symbol##_Wrapper } },
#define EMU86_ENTRY_8(symbol) { #symbol, 8, { .fun8 = symbol##_Wrapper } },
#define EMU86_ENTRY(symbol, argc) EMU86_ENTRY_##argc(symbol)
#define EMU86_ENTRY_SPEC(symbol, guest_type, host_symbol, abi_argc, test_argc, return_kind, bridge_kind) EMU86_ENTRY(symbol, test_argc)
		OSIRIS_IMPORT_LIST(EMU86_ENTRY_SPEC)
#undef EMU86_ENTRY_SPEC
#undef EMU86_ENTRY
#undef EMU86_ENTRY_8
#undef EMU86_ENTRY_7
#undef EMU86_ENTRY_6
#undef EMU86_ENTRY_5
#undef EMU86_ENTRY_4
#undef EMU86_ENTRY_3
#undef EMU86_ENTRY_2
#undef EMU86_ENTRY_1
#undef EMU86_ENTRY_0
	};
}

void ResetOsirisImportHostLog(void)
{
	g_log.clear();
}

const std::string& GetOsirisImportHostLog(void)
{
	return g_log;
}

void PopulateOsirisImportModuleInit(tOSIRISModuleInit* module_init)
{
	if (!module_init)
	{
		return;
	}

	int index = 0;

#define ASSIGN_IMPORT(symbol, argc) module_init->fp[index++] = ModuleInitPointer(symbol##_Wrapper);
#define ASSIGN_IMPORT_ENTRY(symbol, guest_type, host_symbol, abi_argc, test_argc, return_kind, bridge_kind) ASSIGN_IMPORT(symbol, test_argc)
	OSIRIS_IMPORT_LIST(ASSIGN_IMPORT_ENTRY);
#undef ASSIGN_IMPORT_ENTRY
#undef ASSIGN_IMPORT

	for (; index < MAX_MODULEFUNCS; ++index)
	{
		module_init->fp[index] = 0;
	}
}

const emu86_fun_t* GetOsirisImportEmu86FunTable(size_t* count)
{
	if (count)
	{
		*count = sizeof(kOsirisImportWrapperFuns) / sizeof(kOsirisImportWrapperFuns[0]);
	}

	return kOsirisImportWrapperFuns;
}
