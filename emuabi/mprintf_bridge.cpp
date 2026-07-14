#include "emu86.h"
#include "emuint.h"

#include "printf_like.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

extern void MonoPrintf(int n, char *format, ...);

namespace emuabi
{
namespace
{
static const size_t kMonoPrintfChunkSize = 256;
static char kMonoPrintfFormatString[] = "%s";

static const char *resolve_vm_string(Emu86FunCtx& ctx, emu_ptr_t addr)
{
	if (!addr)
		return 0;

	emu86_check_cstr(ctx.emu86, addr);

	if (ctx.emu86 && ctx.emu86->as.base && addr < ctx.emu86->as.size)
		return reinterpret_cast<const char *>(ctx.emu86->as.base + addr);

	return reinterpret_cast<const char *>(static_cast<uintptr_t>(addr));
}

static void write_mprintf_chunk(int severity, const char *data, size_t length)
{
	if (!data || !length)
		return;

	char buffer[kMonoPrintfChunkSize + 1];
	while (length)
	{
		const size_t chunk = length > kMonoPrintfChunkSize ? kMonoPrintfChunkSize : length;
		std::memcpy(buffer, data, chunk);
		buffer[chunk] = '\0';
		MonoPrintf(severity, kMonoPrintfFormatString, buffer);
		data += chunk;
		length -= chunk;
	}
}

struct MprintfArgSource
{
	Emu86FunCtx& ctx;
	size_t index;

	const char *next_string()
	{
		return resolve_vm_string(ctx, ctx.arg<emu_ptr_t>(index++));
	}

	int next_int()
	{
		return ctx.arg<int>(index++);
	}

	unsigned next_hex()
	{
		return ctx.arg<unsigned>(index++);
	}

	char next_char()
	{
		return static_cast<char>(ctx.arg<int>(index++));
	}

	const void *next_pointer()
	{
		return reinterpret_cast<const void *>(static_cast<uintptr_t>(ctx.arg<emu_ptr_t>(index++)));
	}
};

struct MprintfWriter
{
	int severity;

	bool operator()(void *, const char *data, size_t length) const
	{
		write_mprintf_chunk(severity, data, length);
		return true;
	}
};
}

void emucall_MonoPrintf(Emu86FunCtx& ctx, void *)
{
	const int severity = ctx.arg<int>(0);
	const char *format = resolve_vm_string(ctx, ctx.arg<emu_ptr_t>(1));
	if (!format)
	{
		write_mprintf_chunk(severity, "(null)", 6);
		return;
	}

	MprintfArgSource args = { ctx, 2 };
	MprintfWriter writer = { severity };
	const tinyprintf::format_result result = tinyprintf::vformat(format, args, writer, 0);
	if (result == tinyprintf::format_result::invalid_format)
		write_mprintf_chunk(severity, format, std::strlen(format));
}

extern const emu86_ctx_fun_t kOsirisPrintfBridgeFuns[] = {
	{"MonoPrintf", emucall_MonoPrintf, 2, 0},
};

extern const size_t kOsirisPrintfBridgeFunsCount = sizeof(kOsirisPrintfBridgeFuns) / sizeof(kOsirisPrintfBridgeFuns[0]);
}
