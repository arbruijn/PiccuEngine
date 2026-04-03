#ifndef TESTS_VALUE_FUNCTION_CALLERS_H
#define TESTS_VALUE_FUNCTION_CALLERS_H

#include "value_function_test.h"

#define VALUE_FUNCTION_TEST_CALLERS(X) \
	X(AI_Value_SetFov) \
	X(AI_Value_SetTargetHandle) \
	X(AI_Value_ClearFlags) \
	X(AI_Value_SetFlags) \
	X(AI_Value_SetTargetHandleNone) \
	X(AI_Value_SetMaxSpeed) \
	X(AI_Value_GetTargetHandle) \
	X(AI_Value_GetAwareness) \
	X(AI_Value_GetStatusReg) \
	X(AI_Value_GetMaxSpeed) \
	X(LGoal_Value_SetStatusFlags) \
	X(LGoal_Value_SetItemDone) \
	X(LGoal_Value_ClearStatusFlags) \
	X(LGoal_Value_SetPriority) \
	X(LGoal_Value_SetCompletionMessage) \
	X(LGoal_Value_InvertEnabledFlags) \
	X(LGoal_Value_GetGlobalStatus) \
	X(LGoal_Value_GetGoalStatusA) \
	X(LGoal_Value_GetGoalStatusB) \
	X(LGoal_Value_GetGoalStatusC) \
	X(Matcen_Value_SetStatusFlags) \
	X(Matcen_Value_ClearStatusFlags) \
	X(Matcen_Value_SetMaxProd) \
	X(Matcen_Value_SetProdMultiplier) \
	X(Matcen_Value_SetMaxAliveChildren) \
	X(Obj_Value_GetTypePrimary) \
	X(Obj_Value_GetTypeSecondary) \
	X(Obj_Value_GetRoomnum) \
	X(Obj_Value_GetPosPrimary) \
	X(Obj_Value_GetPosSecondary) \
	X(Obj_Value_SetPhysicsFlags) \
	X(Obj_Value_SetMovementType) \
	X(Obj_Value_SetVelocity) \
	X(Obj_Value_SetObjectFlags) \
	X(Obj_Value_TogglePhysicsFlags) \
	X(Obj_Value_SetVirusInfected) \
	X(Obj_Value_SetNegativeLight) \
	X(Obj_Value_GetVirusInfected) \
	X(Obj_Value_GetNegativeLight) \
	X(Obj_Value_GetControlTypeA) \
	X(Obj_Value_GetPosTertiary) \
	X(Obj_Value_GetControlTypeB) \
	X(Obj_Value_GetControlTypeC) \
	X(Obj_Value_GetAnimFrame) \
	X(Player_Value_SetStripWeaponsForObject) \
	X(Player_Value_SetStripWeaponsGlobal) \
	X(Room_Value_GetPathPoint)

#define DECLARE_VALUE_FUNCTION_CALLER(name) void TestCaller_##name(void);
VALUE_FUNCTION_TEST_CALLERS(DECLARE_VALUE_FUNCTION_CALLER)
#undef DECLARE_VALUE_FUNCTION_CALLER

void RunAllValueFunctionTestCallers(void);

#endif
