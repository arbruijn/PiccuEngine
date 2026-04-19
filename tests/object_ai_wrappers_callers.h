#ifndef TESTS_OBJECT_AI_WRAPPERS_CALLERS_H
#define TESTS_OBJECT_AI_WRAPPERS_CALLERS_H

#define OBJECT_AI_WRAPPERS_TEST_CALLERS(X) \
	X(osipf_ObjectGetTimeLived) \
	X(Obj_GetGunPos) \
	X(Obj_GetGroundPos) \
	X(osipf_GetNumAttachSlots) \
	X(osipf_GetAttachChildHandle) \
	X(osipf_UnattachFromParent) \
	X(osipf_UnattachChild) \
	X(osipf_UnattachChildren) \
	X(osipf_RayCast) \
	X(osipf_AIGetPathID) \
	X(AI_GoalFollowPathSimple) \
	X(osipf_AIPowerSwitch) \
	X(osipf_AIValue) \
	X(Obj_Value) \
	X(osipf_AITurnTowardsVectors) \
	X(osipf_AISetType) \
	X(osipf_AIFindHidePos) \
	X(osipf_AIGoalAddEnabler) \
	X(osipf_AIGoalAdd) \
	X(osipf_AIGoalClear) \
	X(AI_FindObjOfType) \
	X(osipf_AIGetRoomPathPoint) \
	X(osipf_AIFindEnergyCenter) \
	X(osipf_AIGetDistToObj) \
	X(osipf_AISetGoalFlags) \
	X(osipf_AISetGoalCircleDist) \
	X(osipf_AIIsObjFriend) \
	X(osipf_AIIsObjEnemy) \
	X(osipf_AIGetCurGoalIndex) \
	X(osipf_ObjMakeListOfType) \
	X(osipf_AIIsDestReachable) \
	X(osipf_AIIsObjReachable)

#define DECLARE_OBJECT_AI_CALLER(name) void TestCaller_##name(void);
OBJECT_AI_WRAPPERS_TEST_CALLERS(DECLARE_OBJECT_AI_CALLER)
#undef DECLARE_OBJECT_AI_CALLER

void RunAllObjectAITestCallers(void);

#endif
