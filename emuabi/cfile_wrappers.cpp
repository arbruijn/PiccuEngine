#include "emu86.h"
#include "cfile_wrappers.h"
#include "pstypes.h"
#include "osiris_predefs.h"
#include <cstdio>
#include <unordered_map>
#include <cstring>

// Guest-visible handles for native file pointers.
static std::unordered_map<emu_ptr_t, CFILE*> g_fileptr_map;
static emu_ptr_t g_next_fileptr_handle = 1;

namespace emuabi
{
// Helper function to convert emu_ptr_t to native void* (FILE*)
CFILE* resolve_file_handle(emu_ptr_t fileptr)
{
	if (fileptr == 0)
		return nullptr;
	
	auto it = g_fileptr_map.find(fileptr);
	if (it != g_fileptr_map.end())
		return it->second;
	
	// If not found in map, assume it's already a native pointer
	// (this can happen for raw host pointers passed through native code).
	return reinterpret_cast<CFILE*>(static_cast<uintptr_t>(fileptr));
}

emu_ptr_t register_file_handle(void* native_ptr)
{
	if (!native_ptr)
		return 0;

	CFILE *file_ptr = reinterpret_cast<CFILE *>(native_ptr);

	for (const auto& entry : g_fileptr_map)
	{
		if (entry.second == file_ptr)
			return entry.first;
	}

	for (;;)
	{
		emu_ptr_t candidate = g_next_fileptr_handle++;
		if (candidate != 0 && g_fileptr_map.find(candidate) == g_fileptr_map.end())
		{
			g_fileptr_map[candidate] = file_ptr;
			return candidate;
		}
	}
}

void unregister_file_handle(emu_ptr_t emu_ptr)
{
	if (emu_ptr != 0)
		g_fileptr_map.erase(emu_ptr);
}
}

// CFReadBytes wrapper
void emucall_osipf_CFReadBytes(Emu86FunCtx& ctx, void *)
{
	ubyte *buffer = ctx.arg<ubyte *>(0);
	int count = ctx.arg<int>(1);
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(2);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFReadBytes(buffer, count, native_fileptr));
}

// CFReadShort wrapper
void emucall_osipf_CFReadShort(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(static_cast<int>(osipf_CFReadShort(native_fileptr)));
}

// CFReadByte wrapper
void emucall_osipf_CFReadByte(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(static_cast<int>(osipf_CFReadByte(native_fileptr)));
}

// CFReadFloat wrapper
void emucall_osipf_CFReadFloat(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFReadFloat(native_fileptr));
}

// CFReadDouble wrapper
void emucall_osipf_CFReadDouble(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFReadDouble(native_fileptr));
}

// CFReadString wrapper
void emucall_osipf_CFReadString(Emu86FunCtx& ctx, void *)
{
	size_t n = static_cast<size_t>(ctx.arg<int>(1));
	char *buf = ctx.arg<char *>(0, n);
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(2);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFReadString(buf, n, native_fileptr));
}

// CFReadInt wrapper
void emucall_osipf_CFReadInt(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFReadInt(native_fileptr));
}

// CFWriteBytes wrapper
void emucall_osipf_CFWriteBytes(Emu86FunCtx& ctx, void *)
{
	const ubyte *buf = ctx.arg<const ubyte *>(0);
	int count = ctx.arg<int>(1);
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(2);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFWriteBytes(buf, count, native_fileptr));
}

// CFWriteShort wrapper
void emucall_osipf_CFWriteShort(Emu86FunCtx& ctx, void *)
{
	short s = static_cast<short>(ctx.arg<int>(0));
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(1);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	osipf_CFWriteShort(s, native_fileptr);
}

// CFWriteByte wrapper
void emucall_osipf_CFWriteByte(Emu86FunCtx& ctx, void *)
{
	sbyte b = static_cast<sbyte>(ctx.arg<int>(0));
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(1);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	osipf_CFWriteByte(b, native_fileptr);
}

// CFWriteFloat wrapper
void emucall_osipf_CFWriteFloat(Emu86FunCtx& ctx, void *)
{
	float f = ctx.arg<float>(0);
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(1);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	osipf_CFWriteFloat(f, native_fileptr);
}

// CFWriteString wrapper
void emucall_osipf_CFWriteString(Emu86FunCtx& ctx, void *)
{
	const char *buf = ctx.arg<const char *>(0);
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(1);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFWriteString(buf, native_fileptr));
}

// CFWriteInt wrapper
void emucall_osipf_CFWriteInt(Emu86FunCtx& ctx, void *)
{
	int i = ctx.arg<int>(0);
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(1);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	osipf_CFWriteInt(i, native_fileptr);
}

// CFWriteDouble wrapper
void emucall_osipf_CFWriteDouble(Emu86FunCtx& ctx, void *)
{
	// Double argument handling - passed as two 32-bit ints on x86
	double d = ctx.arg<double>(0);
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(2);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	osipf_CFWriteDouble(d, native_fileptr);
}

// CFopen wrapper
void emucall_osipf_CFopen(Emu86FunCtx& ctx, void *)
{
	emu86_check_cstr(ctx.emu86, ctx.arg<emu_ptr_t>(0));
	emu86_check_cstr(ctx.emu86, ctx.arg<emu_ptr_t>(1));
	const char *filename = ctx.arg<const char *>(0);
	const char *mode = ctx.arg<const char *>(1);
	CFILE *result = reinterpret_cast<CFILE *>(osipf_CFopen(filename, mode));
	// Store mapping and return emu pointer
	if (result != nullptr) {
		emu_ptr_t emu_ptr = emuabi::register_file_handle(result);
		ctx.set_return(emu_ptr);
	} else {
		ctx.set_return(0);
	}
}

// CFclose wrapper
void emucall_osipf_CFclose(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	osipf_CFclose(native_fileptr);
	emuabi::unregister_file_handle(fileptr);
}

// CFtell wrapper
void emucall_osipf_CFtell(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(osipf_CFtell(native_fileptr));
}

// CFeof wrapper
void emucall_osipf_CFeof(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t fileptr = ctx.arg<emu_ptr_t>(0);
	CFILE *native_fileptr = emuabi::resolve_file_handle(fileptr);
	ctx.set_return(static_cast<int>(osipf_CFeof(native_fileptr)));
}

// Calculate stack bytes to pop: each argument is 4 bytes on x86
emu86_ctx_fun_t kCfileWrapperFuns[] = {
	{"osipf_CFReadBytes", emucall_osipf_CFReadBytes, 3, nullptr},
	{"osipf_CFReadInt", emucall_osipf_CFReadInt, 1, nullptr},
	{"osipf_CFReadShort", emucall_osipf_CFReadShort, 1, nullptr},
	{"osipf_CFReadByte", emucall_osipf_CFReadByte, 1, nullptr},
	{"osipf_CFReadFloat", emucall_osipf_CFReadFloat, 1, nullptr},
	{"osipf_CFReadDouble", emucall_osipf_CFReadDouble, 1, nullptr},
	{"osipf_CFReadString", emucall_osipf_CFReadString, 3, nullptr},
	{"osipf_CFWriteBytes", emucall_osipf_CFWriteBytes, 3, nullptr},
	{"osipf_CFWriteString", emucall_osipf_CFWriteString, 2, nullptr},
	{"osipf_CFWriteInt", emucall_osipf_CFWriteInt, 2, nullptr},
	{"osipf_CFWriteShort", emucall_osipf_CFWriteShort, 2, nullptr},
	{"osipf_CFWriteByte", emucall_osipf_CFWriteByte, 2, nullptr},
	{"osipf_CFWriteFloat", emucall_osipf_CFWriteFloat, 2, nullptr},
	{"osipf_CFWriteDouble", emucall_osipf_CFWriteDouble, 3, nullptr},
	{"osipf_CFopen", emucall_osipf_CFopen, 2, nullptr},
	{"osipf_CFclose", emucall_osipf_CFclose, 1, nullptr},
	{"osipf_CFtell", emucall_osipf_CFtell, 1, nullptr},
	{"osipf_CFeof", emucall_osipf_CFeof, 1, nullptr},
};

size_t kCfileWrapperFunsCount = sizeof(kCfileWrapperFuns) / sizeof(kCfileWrapperFuns[0]);
