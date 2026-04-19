#ifndef TESTS_OSIRIS_WRAPPERS2_CALLERS_H
#define TESTS_OSIRIS_WRAPPERS2_CALLERS_H

#include "osiris_wrappers2_test.h"

#define DECLARE_OSIRIS_WRAPPERS2_CALLER(name, rettype, args) void TestCaller_##name(void);
OSIRIS_WRAPPERS2_TEST_CALLERS(DECLARE_OSIRIS_WRAPPERS2_CALLER)
#undef DECLARE_OSIRIS_WRAPPERS2_CALLER

void RunAllOsirisWrappers2TestCallers(void);

#endif
