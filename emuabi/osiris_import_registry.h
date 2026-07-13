#ifndef EMUABI_OSIRIS_IMPORT_REGISTRY_H
#define EMUABI_OSIRIS_IMPORT_REGISTRY_H

#include "emu86.h"

#include <string>
#include <vector>

extern const emu86_ctx_fun_t kOsirisWrapperFuns[];
extern const size_t kOsirisWrapperFunsCount;
extern const emu86_ctx_fun_t kOsirisWrapperFuns2[];
extern const size_t kOsirisWrapperFuns2Count;
extern const emu86_ctx_fun_t kObjectAIWrapperFuns[];
extern const size_t kObjectAIWrapperFunsCount;
extern const emu86_ctx_fun_t kCfileWrapperFuns[];
extern const size_t kCfileWrapperFunsCount;

namespace emuabi
{
bool Osiris_BuildImportThunkTable(Emu* emu86, uint32_t* addresses, size_t address_count, std::vector<std::string>* missing);
}

#endif
