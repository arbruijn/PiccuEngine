#ifndef DESCENT3_OSIRIS_IMPORTS_SHARED_H
#define DESCENT3_OSIRIS_IMPORTS_SHARED_H

enum OsirisImportReturnKind
{
	OSIRIS_IMPORT_RETURN_VOID = 0,
	OSIRIS_IMPORT_RETURN_INT,
	OSIRIS_IMPORT_RETURN_SHORT,
	OSIRIS_IMPORT_RETURN_SBYTE,
	OSIRIS_IMPORT_RETURN_UBYTE,
	OSIRIS_IMPORT_RETURN_BOOL,
	OSIRIS_IMPORT_RETURN_FLOAT,
	OSIRIS_IMPORT_RETURN_DOUBLE,
	OSIRIS_IMPORT_RETURN_POINTER,
	OSIRIS_IMPORT_RETURN_VECTOR
};

enum OsirisImportBridgeKind
{
	OSIRIS_IMPORT_BRIDGE_DIRECT = 0,
	OSIRIS_IMPORT_BRIDGE_VM_POINTERS,
	OSIRIS_IMPORT_BRIDGE_MSAFE,
	OSIRIS_IMPORT_BRIDGE_EVENT_INFO,
	OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE,
	OSIRIS_IMPORT_BRIDGE_FILE_HANDLE,
	OSIRIS_IMPORT_BRIDGE_SCRIPT_MEMORY,
	OSIRIS_IMPORT_BRIDGE_OMMS,
	OSIRIS_IMPORT_BRIDGE_VARIADIC_TEXT,
	OSIRIS_IMPORT_BRIDGE_VARIADIC_GOAL
};

struct OsirisImportSpec
{
	const char* guest_symbol;
	const char* host_symbol;
	unsigned char abi_argc;
	unsigned char test_argc;
	unsigned char return_kind;
	unsigned char bridge_kind;
};

// The manifest below is the positional import order shared by:
// - Descent3/OsirisLoadandBind.cpp: Osiris_CreateModuleInitStruct()
// - scripts/osiris_import.h: osicommon_Initialize()
//
// test_argc may be smaller than abi_argc when the current emu86 harness cannot
// model the real call shape yet.
#define OSIRIS_IMPORT_LIST(X) \
	X(mprintf, mprintf, MonoPrintf, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VARIADIC_TEXT) \
	X(MSafe_CallFunction, MSafe_CallFunction, msafe_CallFunction, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_MSAFE) \
	X(MSafe_GetValue, MSafe_GetValue, msafe_GetValue, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_MSAFE) \
	X(Obj_CallEvent, Obj_CallEvent, osipf_CallObjectEvent, 3, 3, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_EVENT_INFO) \
	X(Trgr_CallEvent, Trgr_CallEvent, osipf_CallTriggerEvent, 3, 3, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_EVENT_INFO) \
	X(Sound_TouchFile, Sound_TouchFile, osipf_SoundTouch, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Obj_FindID, Obj_FindID, osipf_ObjectFindID, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Wpn_FindID, Wpn_FindID, osipf_WeaponFindID, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Obj_GetTimeLived, Obj_GetTimeLived, osipf_ObjectGetTimeLived, 1, 1, OSIRIS_IMPORT_RETURN_FLOAT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_GetGunPosFP, Obj_GetGunPos, osipf_GetGunPos, 4, 4, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Room_ValueFP, Room_Value, osipf_RoomValue, 5, 5, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(Room_IsValid, Room_IsValid, osipf_IsRoomValid, 1, 1, OSIRIS_IMPORT_RETURN_UBYTE, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_GetAttachParent, Obj_GetAttachParent, osipf_GetAttachParent, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_GetNumAttachSlots, Obj_GetNumAttachSlots, osipf_GetNumAttachSlots, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_GetAttachChildHandle, Obj_GetAttachChildHandle, osipf_GetAttachChildHandle, 2, 2, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_AttachObjectAP, Obj_AttachObjectAP, osipf_AttachObjectAP, 5, 5, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_AttachObjectRad, Obj_AttachObjectRad, osipf_AttachObjectRad, 4, 4, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_UnattachFromParent, Obj_UnattachFromParent, osipf_UnattachFromParent, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_UnattachChild, Obj_UnattachChild, osipf_UnattachChild, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_UnattachChildren, Obj_UnattachChildren, osipf_UnattachChildren, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(FVI_RayCast, FVI_RayCast, osipf_RayCast, 7, 7, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(AI_GetPathID, AI_GetPathID, osipf_AIGetPathID, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(AI_GoalFollowPathSimpleFP, AI_GoalFollowPathSimple, osipf_AIGoalFollowPathSimple, 5, 5, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_PowerSwitch, AI_PowerSwitch, osipf_AIPowerSwitch, 2, 2, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_TurnTowardsVectors, AI_TurnTowardsVectors, osipf_AITurnTowardsVectors, 3, 3, OSIRIS_IMPORT_RETURN_UBYTE, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(AI_SetType, AI_SetType, osipf_AISetType, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_FindHidePos, AI_FindHidePos, osipf_AIFindHidePos, 4, 4, OSIRIS_IMPORT_RETURN_VECTOR, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(AI_GoalAddEnabler, AI_GoalAddEnabler, osipf_AIGoalAddEnabler, 6, 6, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(AI_AddGoal, AI_AddGoal, osipf_AIGoalAdd, 6, 6, OSIRIS_IMPORT_RETURN_FLOAT, OSIRIS_IMPORT_BRIDGE_VARIADIC_GOAL) \
	X(AI_ClearGoal, AI_ClearGoal, osipf_AIGoalClear, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_Value, AI_Value, osipf_AIValue, 4, 4, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(AI_FindObjOfTypeFP, AI_FindObjOfType, osipf_AIFindObjOfType, 5, 5, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_GetRoomPathPoint, AI_GetRoomPathPoint, osipf_AIGetRoomPathPoint, 1, 1, OSIRIS_IMPORT_RETURN_VECTOR, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_FindEnergyCenter, AI_FindEnergyCenter, osipf_AIFindEnergyCenter, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_GetDistToObj, AI_GetDistToObj, osipf_AIGetDistToObj, 2, 2, OSIRIS_IMPORT_RETURN_FLOAT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_SetGoalFlags, AI_SetGoalFlags, osipf_AISetGoalFlags, 4, 4, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_SetGoalCircleDist, AI_SetGoalCircleDist, osipf_AISetGoalCircleDist, 3, 3, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(File_ReadBytes, File_ReadBytes, osipf_CFReadBytes, 3, 3, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_ReadInt, File_ReadInt, osipf_CFReadInt, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_ReadShort, File_ReadShort, osipf_CFReadShort, 1, 1, OSIRIS_IMPORT_RETURN_SHORT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_ReadByte, File_ReadByte, osipf_CFReadByte, 1, 1, OSIRIS_IMPORT_RETURN_SBYTE, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_ReadFloat, File_ReadFloat, osipf_CFReadFloat, 1, 1, OSIRIS_IMPORT_RETURN_FLOAT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_ReadDouble, File_ReadDouble, osipf_CFReadDouble, 1, 1, OSIRIS_IMPORT_RETURN_DOUBLE, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_ReadString, File_ReadString, osipf_CFReadString, 3, 3, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_WriteBytes, File_WriteBytes, osipf_CFWriteBytes, 3, 3, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_WriteString, File_WriteString, osipf_CFWriteString, 2, 2, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_WriteInt, File_WriteInt, osipf_CFWriteInt, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_WriteShort, File_WriteShort, osipf_CFWriteShort, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_WriteByte, File_WriteByte, osipf_CFWriteByte, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_WriteFloat, File_WriteFloat, osipf_CFWriteFloat, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_WriteDouble, File_WriteDouble, osipf_CFWriteDouble, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(Scrpt_MemAlloc, Scrpt_MemAlloc, Osiris_AllocateMemory, 1, 1, OSIRIS_IMPORT_RETURN_POINTER, OSIRIS_IMPORT_BRIDGE_SCRIPT_MEMORY) \
	X(Scrpt_MemFree, Scrpt_MemFree, Osiris_FreeMemory, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_SCRIPT_MEMORY) \
	X(Scrpt_CancelTimer, Scrpt_CancelTimer, Osiris_CancelTimer, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Scrpt_CreateTimer, Scrpt_CreateTimer, Osiris_CreateTimer, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(MSafe_DoPowerup, MSafe_DoPowerup, msafe_DoPowerup, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_MSAFE) \
	X(Obj_CreateFP, Obj_Create, osipf_ObjCreate, 7, 7, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Game_GetTime, Game_GetTime, osipf_GameTime, 0, 0, OSIRIS_IMPORT_RETURN_FLOAT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Game_GetFrameTime, Game_GetFrameTime, osipf_FrameTime, 0, 0, OSIRIS_IMPORT_RETURN_FLOAT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_WBValueFP, Obj_WBValue, osipf_ObjWBValue, 6, 6, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(Scrpt_TimerExists, Scrpt_TimerExists, Osiris_TimerExists, 1, 1, OSIRIS_IMPORT_RETURN_UBYTE, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_ValueFP, Obj_Value, osipf_ObjectValue, 5, 5, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(Matcen_ValueFP, Matcen_Value, osipf_MatcenValue, 5, 5, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(Matcen_Reset, Matcen_Reset, osipf_MatcenReset, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Matcen_Copy, Matcen_Copy, osipf_MatcenCopy, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Matcen_Create, Matcen_Create, osipf_MatcenCreate, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Matcen_FindID, Matcen_FindID, osipf_MatcenFindId, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Msn_FlagSet, Msn_FlagSet, osipf_MissionFlagSet, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Msn_FlagGet, Msn_FlagGet, osipf_MissionFlagGet, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Player_ValueFP, Player_Value, osipf_PlayerValue, 5, 5, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(Obj_SetCustomAnimFP, Obj_SetCustomAnim, osipf_ObjectCustomAnim, 7, 7, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Player_AddHudMessage, Player_AddHudMessage, osipf_PlayerAddHudMessage, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Obj_Ghost, Obj_Ghost, osipf_ObjGhost, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_BurningFP, Obj_Burning, osipf_ObjBurning, 3, 3, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_IsEffect, Obj_IsEffect, osipf_ObjIsEffect, 2, 2, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(File_Open, File_Open, osipf_CFopen, 2, 2, OSIRIS_IMPORT_RETURN_POINTER, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_Close, File_Close, osipf_CFclose, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_Tell, File_Tell, osipf_CFtell, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(File_eof, File_eof, osipf_CFeof, 1, 1, OSIRIS_IMPORT_RETURN_UBYTE, OSIRIS_IMPORT_BRIDGE_FILE_HANDLE) \
	X(Sound_StopFP, Sound_Stop, osipf_SoundStop, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Sound_Play2dFP, Sound_Play2d, osipf_SoundPlay2d, 3, 3, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Sound_Play3dFP, Sound_Play3d, osipf_SoundPlay3d, 3, 3, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Sound_FindId, Sound_FindId, osipf_SoundFindId, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(AI_IsObjFriend, AI_IsObjFriend, osipf_AIIsObjFriend, 2, 2, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_IsObjEnemy, AI_IsObjEnemy, osipf_AIIsObjEnemy, 2, 2, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_GoalValueFP, AI_GoalValue, osipf_AIGoalValue, 6, 6, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(AI_GetNearbyObjsFP, AI_GetNearbyObjs, osipf_AIGetNearbyObjs, 9, 8, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(AI_GetCurGoalIndex, AI_GetCurGoalIndex, osipf_AIGetCurGoalIndex, 1, 1, OSIRIS_IMPORT_RETURN_SBYTE, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(OMMS_Malloc, OMMS_Malloc, Osiris_OMMS_Malloc, 3, 3, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_OMMS) \
	X(OMMS_Attach, OMMS_Attach, Osiris_OMMS_Attach, 1, 1, OSIRIS_IMPORT_RETURN_POINTER, OSIRIS_IMPORT_BRIDGE_OMMS) \
	X(OMMS_Detach, OMMS_Detach, Osiris_OMMS_Detach, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_OMMS) \
	X(OMMS_Free, OMMS_Free, Osiris_OMMS_Free, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_OMMS) \
	X(OMMS_Find, OMMS_Find, Osiris_OMMS_Find, 2, 2, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_OMMS) \
	X(OMMS_GetInfo, OMMS_GetInfo, Osiris_OMMS_GetInfo, 5, 5, OSIRIS_IMPORT_RETURN_SBYTE, OSIRIS_IMPORT_BRIDGE_OMMS) \
	X(Cine_Start, Cine_Start, Cinematic_Start, 2, 2, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Cine_Stop, Cine_Stop, Cinematic_Stop, 0, 0, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Scrpt_FindSoundName, Scrpt_FindSoundName, osipf_FindSoundName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_FindRoomName, Scrpt_FindRoomName, osipf_FindRoomName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_FindTriggerName, Scrpt_FindTriggerName, osipf_FindTriggerName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_FindObjectName, Scrpt_FindObjectName, osipf_FindObjectName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_GetTriggerRoom, Scrpt_GetTriggerRoom, osipf_GetTriggerRoom, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Scrpt_GetTriggerFace, Scrpt_GetTriggerFace, osipf_GetTriggerFace, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Scrpt_FindDoorName, Scrpt_FindDoorName, osipf_FindDoorName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_FindTextureName, Scrpt_FindTextureName, osipf_FindTextureName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Game_CreateRandomSparksFP, Game_CreateRandomSparks, osipf_CreateRandomSparks, 5, 5, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_CancelTimerID, Scrpt_CancelTimerID, Osiris_CancelTimerID, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Obj_GetGroundPosFP, Obj_GetGroundPos, osipf_GetGroundPos, 4, 4, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Game_EnableShip, Game_EnableShip, osipf_EnableShip, 2, 2, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Game_IsShipEnabled, Game_IsShipEnabled, osipf_IsShipEnabled, 1, 1, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Path_GetInformationFP, Path_GetInformation, osipf_PathGetInformation, 5, 5, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Cine_StartCanned, Cine_StartCanned, Cinematic_StartCannedScript, 1, 1, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_FindMatcenName, Scrpt_FindMatcenName, osipf_FindMatcenName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_FindPathName, Scrpt_FindPathName, osipf_FindPathName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Scrpt_FindLevelGoalName, Scrpt_FindLevelGoalName, osipf_FindLevelGoalName, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Obj_FindType, Obj_FindType, osipf_ObjectFindType, 1, 1, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(LGoal_ValueFP, LGoal_Value, osipf_LGoalValue, 5, 5, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE) \
	X(Obj_MakeListOfType, Obj_MakeListOfType, osipf_ObjMakeListOfType, 7, 7, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_VM_POINTERS) \
	X(Obj_Kill, Obj_Kill, osipf_ObjKill, 6, 6, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_IsDestReachable, AI_IsDestReachable, osipf_AIIsDestReachable, 2, 2, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(AI_IsObjReachable, AI_IsObjReachable, osipf_AIIsObjReachable, 2, 2, OSIRIS_IMPORT_RETURN_BOOL, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Game_GetDiffLevel, Game_GetDiffLevel, osipf_GameGetDiffLevel, 0, 0, OSIRIS_IMPORT_RETURN_SBYTE, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Game_GetLanguage, Game_GetLanguage, osipf_GetLanguageSetting, 0, 0, OSIRIS_IMPORT_RETURN_INT, OSIRIS_IMPORT_BRIDGE_DIRECT) \
	X(Path_Value, Path_Value, osipf_PathValue, 5, 5, OSIRIS_IMPORT_RETURN_VOID, OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE)

static inline const OsirisImportSpec* Osiris_GetImportSpecTable(unsigned int* count)
{
	static const OsirisImportSpec kSpecs[] = {
#define OSIRIS_IMPORT_SPEC_ENTRY(guest_symbol, guest_type, host_symbol, abi_argc, test_argc, return_kind, bridge_kind) \
		{ #guest_symbol, #host_symbol, abi_argc, test_argc, return_kind, bridge_kind },
		OSIRIS_IMPORT_LIST(OSIRIS_IMPORT_SPEC_ENTRY)
#undef OSIRIS_IMPORT_SPEC_ENTRY
	};

	if (count)
	{
		*count = sizeof(kSpecs) / sizeof(kSpecs[0]);
	}

	return kSpecs;
}

static inline const OsirisImportSpec* Osiris_GetImportSpec(unsigned int slot)
{
	unsigned int count = 0;
	const OsirisImportSpec* specs = Osiris_GetImportSpecTable(&count);
	if (slot >= count)
	{
		return 0;
	}

	return &specs[slot];
}

static inline int Osiris_ImportReturnUsesX87(const OsirisImportSpec* spec)
{
	if (!spec)
	{
		return 0;
	}

	return spec->return_kind == OSIRIS_IMPORT_RETURN_FLOAT || spec->return_kind == OSIRIS_IMPORT_RETURN_DOUBLE;
}

static inline int Osiris_ImportReturnUsesStructSret(const OsirisImportSpec* spec)
{
	return spec && spec->return_kind == OSIRIS_IMPORT_RETURN_VECTOR;
}

static inline int Osiris_ImportNeedsVmPointerBridge(const OsirisImportSpec* spec)
{
	if (!spec)
	{
		return 0;
	}

	switch (spec->bridge_kind)
	{
	case OSIRIS_IMPORT_BRIDGE_VM_POINTERS:
	case OSIRIS_IMPORT_BRIDGE_MSAFE:
	case OSIRIS_IMPORT_BRIDGE_EVENT_INFO:
	case OSIRIS_IMPORT_BRIDGE_VALUE_STORAGE:
	case OSIRIS_IMPORT_BRIDGE_FILE_HANDLE:
	case OSIRIS_IMPORT_BRIDGE_SCRIPT_MEMORY:
	case OSIRIS_IMPORT_BRIDGE_OMMS:
	case OSIRIS_IMPORT_BRIDGE_VARIADIC_TEXT:
	case OSIRIS_IMPORT_BRIDGE_VARIADIC_GOAL:
		return 1;
	default:
		return 0;
	}
}

static inline int Osiris_ImportNeedsOpaqueHandleBridge(const OsirisImportSpec* spec)
{
	return spec && spec->bridge_kind == OSIRIS_IMPORT_BRIDGE_FILE_HANDLE;
}

static inline int Osiris_ImportReturnsGuestVisibleMemory(const OsirisImportSpec* spec)
{
	return spec && (spec->bridge_kind == OSIRIS_IMPORT_BRIDGE_SCRIPT_MEMORY || spec->bridge_kind == OSIRIS_IMPORT_BRIDGE_OMMS);
}

static inline int Osiris_ImportRequiresGuestThunk(const OsirisImportSpec* spec)
{
	return Osiris_ImportReturnUsesX87(spec) || Osiris_ImportReturnUsesStructSret(spec) || Osiris_ImportNeedsVmPointerBridge(spec);
}

static inline int Osiris_ImportUsesReducedTestArgc(const OsirisImportSpec* spec)
{
	return spec && spec->abi_argc != spec->test_argc;
}

#endif
