#include "value_function_callers.h"

void TestCaller_AI_Value_SetFov(void)
{
	float fov = 90.0f;
	AI_Value_Test(1001, VF_SET, AIV_F_FOV, &fov);
}

void TestCaller_AI_Value_SetTargetHandle(void)
{
	int targethandle = 1002;
	AI_Value_Test(1001, VF_SET, AIV_I_TARGET_HANDLE, &targethandle);
}

void TestCaller_AI_Value_ClearFlags(void)
{
	int flags = 0x20;
	AI_Value_Test(1001, VF_CLEAR_FLAGS, AIV_I_FLAGS, &flags);
}

void TestCaller_AI_Value_SetFlags(void)
{
	int flags = 0x40;
	AI_Value_Test(1001, VF_SET_FLAGS, AIV_I_FLAGS, &flags);
}

void TestCaller_AI_Value_SetTargetHandleNone(void)
{
	int targethandle = OBJECT_HANDLE_NONE;
	AI_Value_Test(1001, VF_SET, AIV_I_TARGET_HANDLE, &targethandle);
}

void TestCaller_AI_Value_SetMaxSpeed(void)
{
	float max_speed = 42.5f;
	AI_Value_Test(1001, VF_SET, AIV_F_MAX_SPEED, &max_speed);
}

void TestCaller_AI_Value_GetTargetHandle(void)
{
	int targethandle = 1002;
	AI_Value_Test(1001, VF_GET, AIV_I_TARGET_HANDLE, &targethandle);
}

void TestCaller_AI_Value_GetAwareness(void)
{
	float awareness = 0.75f;
	AI_Value_Test(1001, VF_GET, AIV_F_AWARENESS, &awareness);
}

void TestCaller_AI_Value_GetStatusReg(void)
{
	int status = 0x80;
	AI_Value_Test(1001, VF_GET, AIV_I_STATUS_REG, &status);
}

void TestCaller_AI_Value_GetMaxSpeed(void)
{
	float max_speed = 42.5f;
	AI_Value_Test(1001, VF_GET, AIV_F_MAX_SPEED, &max_speed);
}

void TestCaller_LGoal_Value_SetStatusFlags(void)
{
	int flags = 0x01;
	LGoal_Value_Test(VF_SET_FLAGS, LGSV_I_STATUS, &flags, 2, -1);
}

void TestCaller_LGoal_Value_SetItemDone(void)
{
	bool done = true;
	LGoal_Value_Test(VF_SET, LGSSV_B_ITEM_DONE, &done, 2, 1);
}

void TestCaller_LGoal_Value_ClearStatusFlags(void)
{
	int flags = 0x02;
	LGoal_Value_Test(VF_CLEAR_FLAGS, LGSV_I_STATUS, &flags, 2, -1);
}

void TestCaller_LGoal_Value_SetPriority(void)
{
	int priority = 7;
	LGoal_Value_Test(VF_SET, LGSV_I_PRIORITY, &priority, 2, -1);
}

void TestCaller_LGoal_Value_SetCompletionMessage(void)
{
	char message[] = "Goal complete";
	LGoal_Value_Test(VF_SET, LGSV_PC_COMPLETION_MESSAGE, message, 2, -1);
}

void TestCaller_LGoal_Value_InvertEnabledFlags(void)
{
	int flags = 0x04;
	LGoal_Value_Test(VF_SET_FLAGS, LGSV_I_STATUS, &flags, 2, -1);
}

void TestCaller_LGoal_Value_GetGlobalStatus(void)
{
	int flags = 0x08;
	LGoal_Value_Test(VF_GET, LGV_I_STATUS, &flags, -1, -1);
}

void TestCaller_LGoal_Value_GetGoalStatusA(void)
{
	int flags = 0x10;
	LGoal_Value_Test(VF_GET, LGSV_I_STATUS, &flags, 2, -1);
}

void TestCaller_LGoal_Value_GetGoalStatusB(void)
{
	int flags = 0x20;
	LGoal_Value_Test(VF_GET, LGSV_I_STATUS, &flags, 3, -1);
}

void TestCaller_LGoal_Value_GetGoalStatusC(void)
{
	int flags = 0x40;
	LGoal_Value_Test(VF_GET, LGSV_I_STATUS, &flags, 4, -1);
}

void TestCaller_Matcen_Value_SetStatusFlags(void)
{
	int flags = 0x01;
	Matcen_Value_Test(11, VF_SET_FLAGS, MTNV_I_STATUS, &flags, 0);
}

void TestCaller_Matcen_Value_ClearStatusFlags(void)
{
	int flags = 0x02;
	Matcen_Value_Test(11, VF_CLEAR_FLAGS, MTNV_I_STATUS, &flags, 0);
}

void TestCaller_Matcen_Value_SetMaxProd(void)
{
	int max_produced = 12;
	Matcen_Value_Test(11, VF_SET, MTNV_I_MAX_PROD, &max_produced, 0);
}

void TestCaller_Matcen_Value_SetProdMultiplier(void)
{
	float multiplier = 1.5f;
	Matcen_Value_Test(11, VF_SET, MTNV_F_PROD_MULTIPLIER, &multiplier, 0);
}

void TestCaller_Matcen_Value_SetMaxAliveChildren(void)
{
	int max_alive = 4;
	Matcen_Value_Test(11, VF_SET, MTNV_I_MAX_ALIVE_CHILDREN, &max_alive, 0);
}

void TestCaller_Obj_Value_GetTypePrimary(void)
{
	int type = 2;
	Obj_Value_Test(1001, VF_GET, OBJV_I_TYPE, &type, 0);
}

void TestCaller_Obj_Value_GetTypeSecondary(void)
{
	int type = 3;
	Obj_Value_Test(1002, VF_GET, OBJV_I_TYPE, &type, 0);
}

void TestCaller_Obj_Value_GetRoomnum(void)
{
	int roomnum = 7;
	Obj_Value_Test(1001, VF_GET, OBJV_I_ROOMNUM, &roomnum, 0);
}

void TestCaller_Obj_Value_GetPosPrimary(void)
{
	vector pos = { 10.0f, 20.0f, 30.0f };
	Obj_Value_Test(1001, VF_GET, OBJV_V_POS, &pos, 0);
}

void TestCaller_Obj_Value_GetPosSecondary(void)
{
	vector pos = { 40.0f, 50.0f, 60.0f };
	Obj_Value_Test(1002, VF_GET, OBJV_V_POS, &pos, 0);
}

void TestCaller_Obj_Value_SetPhysicsFlags(void)
{
	int flags = 0x100;
	Obj_Value_Test(1001, VF_SET_FLAGS, OBJV_I_PHYSICS_FLAGS, &flags, 0);
}

void TestCaller_Obj_Value_SetMovementType(void)
{
	char movement_type = 6;
	Obj_Value_Test(1001, VF_SET, OBJV_C_MOVEMENT_TYPE, &movement_type, 0);
}

void TestCaller_Obj_Value_SetVelocity(void)
{
	vector velocity = { 1.0f, -2.0f, 3.5f };
	Obj_Value_Test(1001, VF_SET, OBJV_V_VELOCITY, &velocity, 0);
}

void TestCaller_Obj_Value_SetObjectFlags(void)
{
	int flags = 0x200;
	Obj_Value_Test(1001, VF_CLEAR_FLAGS, OBJV_I_FLAGS, &flags, 0);
}

void TestCaller_Obj_Value_TogglePhysicsFlags(void)
{
	int flags = 0x400;
	Obj_Value_Test(1001, VF_CLEAR_FLAGS, OBJV_I_PHYSICS_FLAGS, &flags, 0);
}

void TestCaller_Obj_Value_SetVirusInfected(void)
{
	char enabled = 1;
	Obj_Value_Test(1001, VF_SET, OBJV_C_VIRUS_INFECTED, &enabled, 0);
}

void TestCaller_Obj_Value_SetNegativeLight(void)
{
	char enabled = 1;
	Obj_Value_Test(1001, VF_SET, OBJV_C_NEGATIVE_LIGHT, &enabled, 0);
}

void TestCaller_Obj_Value_GetVirusInfected(void)
{
	char enabled = 1;
	Obj_Value_Test(1001, VF_GET, OBJV_C_VIRUS_INFECTED, &enabled, 0);
}

void TestCaller_Obj_Value_GetNegativeLight(void)
{
	char enabled = 0;
	Obj_Value_Test(1001, VF_GET, OBJV_C_NEGATIVE_LIGHT, &enabled, 0);
}

void TestCaller_Obj_Value_GetControlTypeA(void)
{
	char control_type = 5;
	Obj_Value_Test(1001, VF_GET, OBJV_C_CONTROL_TYPE, &control_type, 0);
}

void TestCaller_Obj_Value_GetPosTertiary(void)
{
	vector pos = { -5.0f, 2.0f, 99.0f };
	Obj_Value_Test(1003, VF_GET, OBJV_V_POS, &pos, 0);
}

void TestCaller_Obj_Value_GetControlTypeB(void)
{
	char control_type = 6;
	Obj_Value_Test(1001, VF_GET, OBJV_C_CONTROL_TYPE, &control_type, 0);
}

void TestCaller_Obj_Value_GetControlTypeC(void)
{
	char control_type = 7;
	Obj_Value_Test(1001, VF_GET, OBJV_C_CONTROL_TYPE, &control_type, 0);
}

void TestCaller_Obj_Value_GetAnimFrame(void)
{
	float anim_frame = 12.25f;
	Obj_Value_Test(1004, VF_GET, OBJV_F_ANIM_FRAME, &anim_frame, 0);
}

void TestCaller_Player_Value_SetStripWeaponsForObject(void)
{
	int value = 1;
	Player_Value_Test(1001, VF_SET, PLYV_I_STRIP_WEAPONS, &value, 0);
}

void TestCaller_Player_Value_SetStripWeaponsGlobal(void)
{
	int value = 1;
	Player_Value_Test(OBJECT_HANDLE_NONE, VF_SET, PLYV_I_STRIP_WEAPONS, &value, 0);
}

void TestCaller_Room_Value_GetPathPoint(void)
{
	vector pos = { 8.0f, 9.0f, 10.0f };
	Room_Value_Test(7, VF_GET, RMSV_V_PATH_PNT, &pos, 0);
}

void RunAllValueFunctionTestCallers(void)
{
#define RUN_VALUE_FUNCTION_CALLER(name) TestCaller_##name();
	VALUE_FUNCTION_TEST_CALLERS(RUN_VALUE_FUNCTION_CALLER)
#undef RUN_VALUE_FUNCTION_CALLER
}
