#include "osiris_wrappers2_callers.h"

#include <string.h>

#define DEFINE_OSIRIS_WRAPPERS2_POINTER(name, rettype, args) name##_fp name = 0;
OSIRIS_WRAPPERS2_TEST_CALLERS(DEFINE_OSIRIS_WRAPPERS2_POINTER)
#undef DEFINE_OSIRIS_WRAPPERS2_POINTER

#define DEFINE_OSIRIS_WRAPPERS2_SETTER(name, rettype, args) \
	extern "C" __declspec(dllexport) unsigned set_##name(unsigned fn) \
	{ \
		name = reinterpret_cast<name##_fp>(fn); \
		return name ? 1u : 0u; \
	}

OSIRIS_WRAPPERS2_TEST_CALLERS(DEFINE_OSIRIS_WRAPPERS2_SETTER)

#undef DEFINE_OSIRIS_WRAPPERS2_SETTER

extern "C" {

__declspec(dllexport) unsigned run_all_osiris_wrappers2_test_callers(void)
{
	RunAllOsirisWrappers2TestCallers();
	return 0u;
}

}
