#include "osiris_wrappers2_callers.h"

#include <string.h>

namespace
{
	static void SeedVector(vector* v, float x, float y, float z)
	{
		v->x = x;
		v->y = y;
		v->z = z;
	}

	static void SeedMatrix(matrix* m)
	{
		SeedVector(&m->rvec, 1.0f, 0.0f, 0.0f);
		SeedVector(&m->uvec, 0.0f, 1.0f, 0.0f);
		SeedVector(&m->fvec, 0.0f, 0.0f, 1.0f);
	}

	static void SeedMsafe(msafe_struct* ms)
	{
		memset(ms, 0, sizeof(*ms));
		ms->roomnum = 7;
		ms->objhandle = 1001;
		ms->ithandle = 1002;
		ms->scalar = 1.25f;
		ms->interval = 2.5f;
		ms->state = 1;
		ms->count = 8;
		ms->flags = 0x40;
		SeedVector(&ms->pos, 10.0f, 20.0f, 30.0f);
		SeedVector(&ms->pos2, 40.0f, 50.0f, 60.0f);
		strcpy(ms->message, "powerup-one");
		strcpy(ms->message2, "powerup-two");
	}
}

void TestCaller_osipf_SoundTouch(void)
{
	char sound_name[] = "alpha-sound";
	osipf_SoundTouch(sound_name);
}

void TestCaller_osipf_ObjectFindID(void)
{
	char object_name[] = "alpha-object";
	(void)osipf_ObjectFindID(object_name);
}

void TestCaller_osipf_ObjectFindType(void)
{
	char object_name[] = "beta-object";
	(void)osipf_ObjectFindType(object_name);
}

void TestCaller_osipf_WeaponFindID(void)
{
	char weapon_name[] = "gamma-weapon";
	(void)osipf_WeaponFindID(weapon_name);
}

void TestCaller_Room_Value(void)
{
	int value = 42;
	Room_Value(7, '=', 'i', &value, 3);
}

void TestCaller_osipf_IsRoomValid(void)
{
	(void)osipf_IsRoomValid(17);
}

void TestCaller_osipf_GetAttachParent(void)
{
	(void)osipf_GetAttachParent(2001);
}

void TestCaller_osipf_AttachObjectAP(void)
{
	(void)osipf_AttachObjectAP(1001, 'P', 1002, 'C', 1);
}

void TestCaller_osipf_AttachObjectRad(void)
{
	(void)osipf_AttachObjectRad(1001, 'P', 1002, 0.625f);
}

void TestCaller_Matcen_Value(void)
{
	vector value;
	SeedVector(&value, 1.0f, 2.0f, 3.0f);
	Matcen_Value(11, '=', 'v', &value, 4);
}

void TestCaller_Osiris_FreeMemory(void)
{
	Osiris_FreeMemory(reinterpret_cast<void*>(0x1234));
}

void TestCaller_Osiris_CancelTimer(void)
{
	Osiris_CancelTimer(21);
}

void TestCaller_Obj_Create(void)
{
	vector pos;
	vector vel;
	matrix orient;
	SeedVector(&pos, 12.0f, 13.0f, 14.0f);
	SeedVector(&vel, 15.0f, 16.0f, 17.0f);
	SeedMatrix(&orient);
	(void)Obj_Create(4, 77, 8, &pos, &orient, 1001, &vel);
}

void TestCaller_osipf_GameTime(void)
{
	(void)osipf_GameTime();
}

void TestCaller_osipf_FrameTime(void)
{
	(void)osipf_FrameTime();
}

void TestCaller_Obj_WBValue(void)
{
	int value = 11;
	Obj_WBValue(1001, 2, '=', 'i', &value, 5);
}

void TestCaller_Osiris_TimerExists(void)
{
	(void)Osiris_TimerExists(41);
}

void TestCaller_osipf_MatcenReset(void)
{
	osipf_MatcenReset(51);
}

void TestCaller_osipf_MatcenCopy(void)
{
	osipf_MatcenCopy(61, 62);
}

void TestCaller_osipf_MatcenFindId(void)
{
	char name[] = "matcen-alpha";
	(void)osipf_MatcenFindId(name);
}

void TestCaller_osipf_MissionFlagSet(void)
{
	osipf_MissionFlagSet(7, 1);
}

void TestCaller_osipf_MissionFlagGet(void)
{
	(void)osipf_MissionFlagGet(7);
}

void TestCaller_Player_Value(void)
{
	float value = 9.25f;
	Player_Value(1001, '+', 'f', &value, 6);
}

void TestCaller_Obj_SetCustomAnim(void)
{
	Obj_SetCustomAnim(1001, 1.25f, 2.5f, 3.75f, 'F', 9001, 'N');
}

void TestCaller_osipf_PlayerAddHudMessage(void)
{
	char message[] = "hello hud";
	osipf_PlayerAddHudMessage(1001, message);
}

void TestCaller_osipf_ObjGhost(void)
{
	osipf_ObjGhost(1001, true);
}

void TestCaller_Obj_Burning(void)
{
	Obj_Burning(1001, 4.5f, 6.75f);
}

void TestCaller_osipf_ObjIsEffect(void)
{
	(void)osipf_ObjIsEffect(1001, 3);
}

void TestCaller_Sound_Stop(void)
{
	Sound_Stop(5001, true);
}

void TestCaller_Sound_Play2d(void)
{
	(void)Sound_Play2d(1001, 42, 0.5f);
}

void TestCaller_Sound_Play3d(void)
{
	(void)Sound_Play3d(1001, 43, 0.75f);
}

void TestCaller_osipf_SoundFindId(void)
{
	char name[] = "sound-alpha";
	(void)osipf_SoundFindId(name);
}

void TestCaller_AI_GoalValue(void)
{
	char value = 'Q';
	(void)AI_GoalValue(1001, 4, '=', 'c', &value, 2);
}

void TestCaller_AI_GetNearbyObjs(void)
{
	vector pos;
	int handles[4] = { 11, 22, 33, 44 };
	SeedVector(&pos, 18.0f, 19.0f, 20.0f);
	(void)AI_GetNearbyObjs(&pos, 7, 12.5f, handles, 4, false, true, false, true);
}

void TestCaller_Cinematic_Stop(void)
{
	Cinematic_Stop();
}

void TestCaller_osipf_GetTriggerRoom(void)
{
	(void)osipf_GetTriggerRoom(71);
}

void TestCaller_osipf_GetTriggerFace(void)
{
	(void)osipf_GetTriggerFace(72);
}

void TestCaller_Game_CreateRandomSparks(void)
{
	vector pos;
	SeedVector(&pos, 1.5f, 2.5f, 3.5f);
	Game_CreateRandomSparks(5, &pos, 9, 2, 4.25f);
}

void TestCaller_Osiris_CancelTimerID(void)
{
	Osiris_CancelTimerID(81);
}

void TestCaller_osipf_EnableShip(void)
{
	char ship_name[] = "Black Pyro";
	osipf_EnableShip(ship_name, true);
}

void TestCaller_osipf_IsShipEnabled(void)
{
	char ship_name[] = "Blue Hornet";
	(void)osipf_IsShipEnabled(ship_name);
}

void TestCaller_Path_GetInformation(void)
{
	vector pos;
	int room = 13;
	matrix orient;
	SeedVector(&pos, 21.0f, 22.0f, 23.0f);
	SeedMatrix(&orient);
	(void)Path_GetInformation(91, 2, &pos, &room, &orient);
}

void TestCaller_LGoal_Value(void)
{
	char message[] = "goal-text";
	LGoal_Value('=', 's', message, 5, 1);
}

void TestCaller_osipf_ObjKill(void)
{
	osipf_ObjKill(1001, 1002, 12.5f, 3, 4.5f, 6.5f);
}

void TestCaller_osipf_GameGetDiffLevel(void)
{
	(void)osipf_GameGetDiffLevel();
}

void TestCaller_osipf_GetLanguageSetting(void)
{
	(void)osipf_GetLanguageSetting();
}

void TestCaller_osipf_PathValue(void)
{
	int value = 77;
	osipf_PathValue(101, 4, '=', 3, &value);
}

void RunAllOsirisWrappers2TestCallers(void)
{
#define CALL_OSIRIS_WRAPPERS2_TEST_CALLER(name, rettype, args) TestCaller_##name();
	OSIRIS_WRAPPERS2_TEST_CALLERS(CALL_OSIRIS_WRAPPERS2_TEST_CALLER)
#undef CALL_OSIRIS_WRAPPERS2_TEST_CALLER
}
