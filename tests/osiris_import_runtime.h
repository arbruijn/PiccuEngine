#ifndef TESTS_OSIRIS_IMPORT_RUNTIME_H
#define TESTS_OSIRIS_IMPORT_RUNTIME_H

#include "osiris_common.h"

int InitializeOsirisImportTest(tOSIRISModuleInit* module_init);
int RunAllOsirisImportTestCallers(void);
void RunOsirisImportHostCall(unsigned slot, int argc, const unsigned* args);

#endif
