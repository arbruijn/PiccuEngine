#include "osiris_import_runtime.h"

#if defined(__WATCOMC__)
#define OSIRIS_IMPORT_TEST_CDECL __cdecl
#else
#define OSIRIS_IMPORT_TEST_CDECL
#endif

extern "C" {

__declspec(dllexport) unsigned OSIRIS_IMPORT_TEST_CDECL initialize_osiris_import_test(void* func_list)
{
	return static_cast<unsigned>(InitializeOsirisImportTest(static_cast<tOSIRISModuleInit*>(func_list)));
}

__declspec(dllexport) unsigned OSIRIS_IMPORT_TEST_CDECL run_all_osiris_import_test_callers(void)
{
	return static_cast<unsigned>(RunAllOsirisImportTestCallers());
}

}
