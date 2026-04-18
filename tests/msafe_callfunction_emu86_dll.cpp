#include "msafe_callfunction_callers.h"

msafe_CallFunction_fp msafe_CallFunction = 0;

extern "C" {

__declspec(dllexport) unsigned MSAFE_CALLFUNCTION_CDECL set_msafe_CallFunction(msafe_CallFunction_fp fn)
{
	msafe_CallFunction = fn;
	return msafe_CallFunction != 0 ? 1u : 0u;
}

__declspec(dllexport) unsigned MSAFE_CALLFUNCTION_CDECL run_all_msafe_callfunction_test_callers(void)
{
	if (!msafe_CallFunction)
	{
		return 1u;
	}

	RunAllMSafeCallFunctionTestCallers();
	return 0u;
}

}
