#ifndef TESTS_MSAFE_CALLFUNCTION_TEST_H
#define TESTS_MSAFE_CALLFUNCTION_TEST_H

#ifdef MSAFE_CALLFUNCTION_IS_POINTER
#include "osiris_common.h"
#else
#include "vecmat.h"
#include "osiris_share.h"
#endif

#if defined(__WATCOMC__)
#define MSAFE_CALLFUNCTION_CDECL __cdecl
#else
#define MSAFE_CALLFUNCTION_CDECL
#endif

typedef void (MSAFE_CALLFUNCTION_CDECL *msafe_CallFunction_fp)(ubyte type, msafe_struct* mstruct);

#ifdef MSAFE_CALLFUNCTION_IS_POINTER
extern msafe_CallFunction_fp msafe_CallFunction;
#else
void MSAFE_CALLFUNCTION_CDECL msafe_CallFunction(ubyte type, msafe_struct* mstruct);
#endif

void msafe_CallFunction_Test(ubyte type, msafe_struct* mstruct);

#endif
