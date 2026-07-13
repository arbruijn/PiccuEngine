#include "osiris_import_registry.h"

#include "osiris_import_bridge.h"

#include <string.h>

namespace emuabi
{
namespace
{
static const emu86_ctx_fun_t* FindInTable(const emu86_ctx_fun_t* table, size_t count, const char* name)
{
	if (!name)
		return 0;

	for (size_t i = 0; i < count; ++i)
	{
		if (table[i].name && !strcmp(table[i].name, name))
			return &table[i];
	}

	return 0;
}

static const emu86_ctx_fun_t* FindWrapper(const char* name)
{
	if (const emu86_ctx_fun_t* fun = FindInTable(kOsirisBridgeFuns, kOsirisBridgeFunsCount, name))
		return fun;
	if (const emu86_ctx_fun_t* fun = FindInTable(kOsirisPrintfBridgeFuns, kOsirisPrintfBridgeFunsCount, name))
		return fun;
	if (const emu86_ctx_fun_t* fun = FindInTable(kOsirisWrapperFuns, kOsirisWrapperFunsCount, name))
		return fun;
	if (const emu86_ctx_fun_t* fun = FindInTable(kOsirisWrapperFuns2, kOsirisWrapperFuns2Count, name))
		return fun;
	if (const emu86_ctx_fun_t* fun = FindInTable(kObjectAIWrapperFuns, kObjectAIWrapperFunsCount, name))
		return fun;
	if (const emu86_ctx_fun_t* fun = FindInTable(kCfileWrapperFuns, kCfileWrapperFunsCount, name))
		return fun;
	return 0;
}
}

bool Osiris_BuildImportThunkTable(Emu* emu86, uint32_t* addresses, size_t address_count, std::vector<std::string>* missing)
{
	if (!emu86 || !addresses)
		return false;

	const unsigned spec_count = osiris_import_spec_count();
	if (address_count < spec_count)
		return false;

	for (unsigned i = 0; i < spec_count; ++i)
	{
		const OsirisImportSpec* spec = osiris_import_spec(i);
		if (!spec)
		{
			addresses[i] = 0;
			continue;
		}

		const emu86_ctx_fun_t* fun = FindWrapper(spec->host_symbol);
		if (!fun)
		{
			addresses[i] = 0;
			if (missing)
				missing->push_back(spec->host_symbol);
			continue;
		}

		uint32_t address = 0;
		if (Osiris_BuildGuestThunkTable(emu86, fun, 1, &address, 1) != 1 || !address)
		{
			addresses[i] = 0;
			if (missing)
				missing->push_back(spec->host_symbol);
			continue;
		}

		addresses[i] = address;
	}

	return true;
}
}
