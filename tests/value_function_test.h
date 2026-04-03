#ifndef TESTS_VALUE_FUNCTION_TEST_H
#define TESTS_VALUE_FUNCTION_TEST_H

#include "osiris_common.h"

void AI_Value_Test(int objhandle, char op, char vtype, void* ptr);
void LGoal_Value_Test(char op, char vtype, void* ptr, int g_index, int i_index);
void Matcen_Value_Test(int matcen_handle, char op, char vtype, void* ptr, int prod_index);
void Obj_Value_Test(int objhandle, char op, char vtype, void* ptr, int index);
void Player_Value_Test(int obj_handle, char op, char vhandle, void* ptr, int index);
void Room_Value_Test(int roomnum, char op, char vhandle, void* ptr, int index);

#endif
