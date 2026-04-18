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

	static void LogImportCall(const char* name, int argc, const unsigned* args)
	{
		char buffer[512];
		int written = snprintf(buffer, sizeof(buffer), "%s argc=%d", name, argc);
		if (written < 0)
		{
			return;
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
	}

	static const char* const kImportNames[] = {
#define IMPORT_NAME_ENTRY(symbol, guest_type, host_symbol, abi_argc, test_argc, return_kind, bridge_kind) #symbol,
		OSIRIS_IMPORT_LIST(IMPORT_NAME_ENTRY)
#undef IMPORT_NAME_ENTRY
	};

#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_VOID(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_INT(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_SHORT(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_SBYTE(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_UBYTE(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_BOOL(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_FLOAT(ctx) (ctx).set_return_double(0.0)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_DOUBLE(ctx) (ctx).set_return_double(0.0)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_POINTER(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN_OSIRIS_IMPORT_RETURN_VECTOR(ctx) (ctx).set_return(0u)
#define SET_WRAPPER_RETURN(kind, ctx) SET_WRAPPER_RETURN_##kind(ctx)

#define DEFINE_WRAPPER_0(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		LogImportCall(#symbol, 0, 0); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_1(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)) }; \
		LogImportCall(#symbol, 1, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_2(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(1)) }; \
		LogImportCall(#symbol, 2, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_3(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(1)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(2)) }; \
		LogImportCall(#symbol, 3, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_4(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(1)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(2)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(3)) }; \
		LogImportCall(#symbol, 4, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_5(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(1)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(2)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(3)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(4)) }; \
		LogImportCall(#symbol, 5, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_6(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(1)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(2)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(3)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(4)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(5)) }; \
		LogImportCall(#symbol, 6, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_7(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(1)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(2)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(3)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(4)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(5)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(6)) }; \
		LogImportCall(#symbol, 7, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER_8(symbol, return_kind) \
	static void symbol##_Wrapper(Emu86FunCtx &ctx, void *) \
	{ \
		const unsigned args[] = { static_cast<unsigned>(ctx.arg<emu_ptr_t>(0)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(1)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(2)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(3)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(4)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(5)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(6)), static_cast<unsigned>(ctx.arg<emu_ptr_t>(7)) }; \
		LogImportCall(#symbol, 8, args); \
		SET_WRAPPER_RETURN(return_kind, ctx); \
	}

#define DEFINE_WRAPPER(symbol, argc, return_kind) DEFINE_WRAPPER_##argc(symbol, return_kind)
#define DEFINE_WRAPPER_ENTRY(symbol, guest_type, host_symbol, abi_argc, test_argc, return_kind, bridge_kind) DEFINE_WRAPPER(symbol, test_argc, return_kind)
	OSIRIS_IMPORT_LIST(DEFINE_WRAPPER_ENTRY);
#undef DEFINE_WRAPPER_ENTRY
#undef DEFINE_WRAPPER

	static const emu86_fun_t kOsirisImportWrapperFuns[] = {
#define EMU86_ENTRY_0(symbol) { #symbol, symbol##_Wrapper, 0, 0 },
#define EMU86_ENTRY_1(symbol) { #symbol, symbol##_Wrapper, 1, 0 },
#define EMU86_ENTRY_2(symbol) { #symbol, symbol##_Wrapper, 2, 0 },
#define EMU86_ENTRY_3(symbol) { #symbol, symbol##_Wrapper, 3, 0 },
#define EMU86_ENTRY_4(symbol) { #symbol, symbol##_Wrapper, 4, 0 },
#define EMU86_ENTRY_5(symbol) { #symbol, symbol##_Wrapper, 5, 0 },
#define EMU86_ENTRY_6(symbol) { #symbol, symbol##_Wrapper, 6, 0 },
#define EMU86_ENTRY_7(symbol) { #symbol, symbol##_Wrapper, 7, 0 },
#define EMU86_ENTRY_8(symbol) { #symbol, symbol##_Wrapper, 8, 0 },
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

void RunOsirisImportHostCall(unsigned slot, int argc, const unsigned* args)
{
	if (slot >= (sizeof(kImportNames) / sizeof(kImportNames[0])))
	{
		return;
	}

	LogImportCall(kImportNames[slot], argc, args);
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
