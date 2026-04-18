#ifndef TESTS_OSIRIS_IMPORT_HOST_H
#define TESTS_OSIRIS_IMPORT_HOST_H

#include "osiris_common.h"

#include <stddef.h>
#include <string>

struct emu86_fun_t;
typedef struct emu86_fun_t emu86_fun_t;

void ResetOsirisImportHostLog(void);
const std::string& GetOsirisImportHostLog(void);
void RunOsirisImportHostCall(unsigned slot, int argc, const unsigned* args);

void PopulateOsirisImportModuleInit(tOSIRISModuleInit* module_init);

const emu86_fun_t* GetOsirisImportEmu86FunTable(size_t* count);

#endif
