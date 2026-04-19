#include "object_ai_wrappers_callers.h"

#include "object_ai_wrappers_test.h"

#include <string.h>

namespace
{
	static void SeedVector(vector* v, float x, float y, float z)
	{
		v->x = x;
		v->y = y;
		v->z = z;
	}

	static void SeedRayInfo(ray_info* ri)
	{
		memset(ri, 0, sizeof(*ri));
		ri->fate = 7;
		ri->hit_room = 11;
		SeedVector(&ri->hit_point, 1.0f, 2.0f, 3.0f);
		ri->hit_object = 101;
		ri->hit_subobject = 202;
		ri->hit_face = 303;
		SeedVector(&ri->hit_face_pnt, 4.0f, 5.0f, 6.0f);
		ri->hit_face_room = 404;
		SeedVector(&ri->hit_wallnorm, 7.0f, 8.0f, 9.0f);
	}
}

void TestCaller_osipf_ObjectGetTimeLived(void)
{
	(void)osipf_ObjectGetTimeLived(1001);
}

void TestCaller_Obj_GetGunPos(void)
{
	vector gun_pnt;
	vector gun_normal;
	SeedVector(&gun_pnt, 10.0f, 20.0f, 30.0f);
	SeedVector(&gun_normal, 40.0f, 50.0f, 60.0f);
	Obj_GetGunPos(1001, 2, &gun_pnt, &gun_normal);
}

void TestCaller_Obj_GetGroundPos(void)
{
	vector ground_pnt;
	vector ground_normal;
	SeedVector(&ground_pnt, 11.0f, 21.0f, 31.0f);
	SeedVector(&ground_normal, 41.0f, 51.0f, 61.0f);
	Obj_GetGroundPos(1001, 4, &ground_pnt, &ground_normal);
}

void TestCaller_osipf_GetNumAttachSlots(void)
{
	(void)osipf_GetNumAttachSlots(1001);
}

void TestCaller_osipf_GetAttachChildHandle(void)
{
	(void)osipf_GetAttachChildHandle(1001, 'A');
}

void TestCaller_osipf_UnattachFromParent(void)
{
	osipf_UnattachFromParent(1001);
}

void TestCaller_osipf_UnattachChild(void)
{
	osipf_UnattachChild(1001, 'B');
}

void TestCaller_osipf_UnattachChildren(void)
{
	osipf_UnattachChildren(1001);
}

void TestCaller_osipf_RayCast(void)
{
	vector p0;
	vector p1;
	ray_info ri;
	SeedVector(&p0, 1.5f, 2.5f, 3.5f);
	SeedVector(&p1, 4.5f, 5.5f, 6.5f);
	SeedRayInfo(&ri);
	(void)osipf_RayCast(1001, &p0, &p1, 7, 1.25f, 42, &ri);
}

void TestCaller_osipf_AIGetPathID(void)
{
	char path_name[] = "alpha-path";
	(void)osipf_AIGetPathID(path_name);
}

void TestCaller_AI_GoalFollowPathSimple(void)
{
	(void)AI_GoalFollowPathSimple(1001, 7, 9, 3, 4);
}

void TestCaller_osipf_AIPowerSwitch(void)
{
	(void)osipf_AIPowerSwitch(1001, 1);
}

void TestCaller_osipf_AIValue(void)
{
	float value = 3.25f;
	osipf_AIValue(1001, '=', 'f', &value);
}

void TestCaller_Obj_Value(void)
{
	int value = 17;
	Obj_Value(1001, '+', 'i', &value, 5);
}

void TestCaller_osipf_AITurnTowardsVectors(void)
{
	vector fvec;
	vector uvec;
	SeedVector(&fvec, 1.0f, 0.0f, 0.0f);
	SeedVector(&uvec, 0.0f, 1.0f, 0.0f);
	(void)osipf_AITurnTowardsVectors(1001, &fvec, &uvec);
}

void TestCaller_osipf_AISetType(void)
{
	osipf_AISetType(1001, 12);
}

void TestCaller_osipf_AIFindHidePos(void)
{
	int hide_room = 7;
	vector hidden = osipf_AIFindHidePos(1001, 2002, 3.5f, &hide_room);
	(void)hidden;
}

void TestCaller_osipf_AIGoalAddEnabler(void)
{
	int payload = 99;
	(void)osipf_AIGoalAddEnabler(1001, 2, 3, 0.75f, 1.5f, &payload);
}

void TestCaller_osipf_AIGoalAdd(void)
{
	(void)osipf_AIGoalAdd(1001, 4, 5, 0.25f, 6, 7);
}

void TestCaller_osipf_AIGoalClear(void)
{
	osipf_AIGoalClear(1001, 2);
}

void TestCaller_AI_FindObjOfType(void)
{
	(void)AI_FindObjOfType(1001, 12, 34, true, 56);
}

void TestCaller_osipf_AIGetRoomPathPoint(void)
{
	(void)osipf_AIGetRoomPathPoint(7);
}

void TestCaller_osipf_AIFindEnergyCenter(void)
{
	(void)osipf_AIFindEnergyCenter(1001);
}

void TestCaller_osipf_AIGetDistToObj(void)
{
	(void)osipf_AIGetDistToObj(1001, 2002);
}

void TestCaller_osipf_AISetGoalFlags(void)
{
	(void)osipf_AISetGoalFlags(1001, 2, 0x40, 1);
}

void TestCaller_osipf_AISetGoalCircleDist(void)
{
	osipf_AISetGoalCircleDist(1001, 2, 12.5f);
}

void TestCaller_osipf_AIIsObjFriend(void)
{
	(void)osipf_AIIsObjFriend(1001, 2002);
}

void TestCaller_osipf_AIIsObjEnemy(void)
{
	(void)osipf_AIIsObjEnemy(1001, 2002);
}

void TestCaller_osipf_AIGetCurGoalIndex(void)
{
	(void)osipf_AIGetCurGoalIndex(1001);
}

void TestCaller_osipf_ObjMakeListOfType(void)
{
	int handles[3] = { 701, 702, 703 };
	(void)osipf_ObjMakeListOfType(1001, 12, 34, false, 56, 3, handles);
}

void TestCaller_osipf_AIIsDestReachable(void)
{
	(void)osipf_AIIsDestReachable(1001, 7);
}

void TestCaller_osipf_AIIsObjReachable(void)
{
	(void)osipf_AIIsObjReachable(1001, 2002);
}

void RunAllObjectAITestCallers(void)
{
#define RUN_OBJECT_AI_CALLER(name) TestCaller_##name();
	OBJECT_AI_WRAPPERS_TEST_CALLERS(RUN_OBJECT_AI_CALLER)
#undef RUN_OBJECT_AI_CALLER
}
