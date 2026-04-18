#ifndef TESTS_OSIRIS_IMPORT_TEST_LIST_H
#define TESTS_OSIRIS_IMPORT_TEST_LIST_H

// This list mirrors the positional import order shared by:
// - Descent3/OsirisLoadandBind.cpp: Osiris_CreateModuleInitStruct()
// - scripts/osiris_import.h: osicommon_Initialize()
//
// The test invokes each imported address through a raw 32-bit call site after
// osicommon_Initialize() has populated the script-side globals.
//
// Two imports are intentionally reduced for the harness:
// - AI_AddGoal uses only its fixed 6 arguments and skips the variadic tail.
// - AI_GetNearbyObjs is capped at 8 arguments because emu86 host wrappers only
//   expose up to 8 raw parameters.
#define OSIRIS_IMPORT_TEST_LIST(X) \
	X(mprintf, 2) \
	X(MSafe_CallFunction, 2) \
	X(MSafe_GetValue, 2) \
	X(Obj_CallEvent, 3) \
	X(Trgr_CallEvent, 3) \
	X(Sound_TouchFile, 1) \
	X(Obj_FindID, 1) \
	X(Wpn_FindID, 1) \
	X(Obj_GetTimeLived, 1) \
	X(Obj_GetGunPosFP, 4) \
	X(Room_ValueFP, 5) \
	X(Room_IsValid, 1) \
	X(Obj_GetAttachParent, 1) \
	X(Obj_GetNumAttachSlots, 1) \
	X(Obj_GetAttachChildHandle, 2) \
	X(Obj_AttachObjectAP, 5) \
	X(Obj_AttachObjectRad, 4) \
	X(Obj_UnattachFromParent, 1) \
	X(Obj_UnattachChild, 2) \
	X(Obj_UnattachChildren, 1) \
	X(FVI_RayCast, 7) \
	X(AI_GetPathID, 1) \
	X(AI_GoalFollowPathSimpleFP, 5) \
	X(AI_PowerSwitch, 2) \
	X(AI_TurnTowardsVectors, 3) \
	X(AI_SetType, 2) \
	X(AI_FindHidePos, 4) \
	X(AI_GoalAddEnabler, 6) \
	X(AI_AddGoal, 6) \
	X(AI_ClearGoal, 2) \
	X(AI_Value, 4) \
	X(AI_FindObjOfTypeFP, 5) \
	X(AI_GetRoomPathPoint, 1) \
	X(AI_FindEnergyCenter, 1) \
	X(AI_GetDistToObj, 2) \
	X(AI_SetGoalFlags, 4) \
	X(AI_SetGoalCircleDist, 3) \
	X(File_ReadBytes, 3) \
	X(File_ReadInt, 1) \
	X(File_ReadShort, 1) \
	X(File_ReadByte, 1) \
	X(File_ReadFloat, 1) \
	X(File_ReadDouble, 1) \
	X(File_ReadString, 3) \
	X(File_WriteBytes, 3) \
	X(File_WriteString, 2) \
	X(File_WriteInt, 2) \
	X(File_WriteShort, 2) \
	X(File_WriteByte, 2) \
	X(File_WriteFloat, 2) \
	X(File_WriteDouble, 2) \
	X(Scrpt_MemAlloc, 1) \
	X(Scrpt_MemFree, 1) \
	X(Scrpt_CancelTimer, 1) \
	X(Scrpt_CreateTimer, 1) \
	X(MSafe_DoPowerup, 1) \
	X(Obj_CreateFP, 7) \
	X(Game_GetTime, 0) \
	X(Game_GetFrameTime, 0) \
	X(Obj_WBValueFP, 6) \
	X(Scrpt_TimerExists, 1) \
	X(Obj_ValueFP, 5) \
	X(Matcen_ValueFP, 5) \
	X(Matcen_Reset, 1) \
	X(Matcen_Copy, 2) \
	X(Matcen_Create, 1) \
	X(Matcen_FindID, 1) \
	X(Msn_FlagSet, 2) \
	X(Msn_FlagGet, 1) \
	X(Player_ValueFP, 5) \
	X(Obj_SetCustomAnimFP, 7) \
	X(Player_AddHudMessage, 2) \
	X(Obj_Ghost, 2) \
	X(Obj_BurningFP, 3) \
	X(Obj_IsEffect, 2) \
	X(File_Open, 2) \
	X(File_Close, 1) \
	X(File_Tell, 1) \
	X(File_eof, 1) \
	X(Sound_StopFP, 2) \
	X(Sound_Play2dFP, 3) \
	X(Sound_Play3dFP, 3) \
	X(Sound_FindId, 1) \
	X(AI_IsObjFriend, 2) \
	X(AI_IsObjEnemy, 2) \
	X(AI_GoalValueFP, 6) \
	X(AI_GetNearbyObjsFP, 8) \
	X(AI_GetCurGoalIndex, 1) \
	X(OMMS_Malloc, 3) \
	X(OMMS_Attach, 1) \
	X(OMMS_Detach, 1) \
	X(OMMS_Free, 1) \
	X(OMMS_Find, 2) \
	X(OMMS_GetInfo, 5) \
	X(Cine_Start, 2) \
	X(Cine_Stop, 0) \
	X(Scrpt_FindSoundName, 1) \
	X(Scrpt_FindRoomName, 1) \
	X(Scrpt_FindTriggerName, 1) \
	X(Scrpt_FindObjectName, 1) \
	X(Scrpt_GetTriggerRoom, 1) \
	X(Scrpt_GetTriggerFace, 1) \
	X(Scrpt_FindDoorName, 1) \
	X(Scrpt_FindTextureName, 1) \
	X(Game_CreateRandomSparksFP, 5) \
	X(Scrpt_CancelTimerID, 1) \
	X(Obj_GetGroundPosFP, 4) \
	X(Game_EnableShip, 2) \
	X(Game_IsShipEnabled, 1) \
	X(Path_GetInformationFP, 5) \
	X(Cine_StartCanned, 1) \
	X(Scrpt_FindMatcenName, 1) \
	X(Scrpt_FindPathName, 1) \
	X(Scrpt_FindLevelGoalName, 1) \
	X(Obj_FindType, 1) \
	X(LGoal_ValueFP, 5) \
	X(Obj_MakeListOfType, 7) \
	X(Obj_Kill, 6) \
	X(AI_IsDestReachable, 2) \
	X(AI_IsObjReachable, 2) \
	X(Game_GetDiffLevel, 0) \
	X(Game_GetLanguage, 0) \
	X(Path_Value, 5)

#endif
