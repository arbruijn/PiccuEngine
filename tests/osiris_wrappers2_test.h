#ifndef TESTS_OSIRIS_WRAPPERS2_TEST_H
#define TESTS_OSIRIS_WRAPPERS2_TEST_H

#include "osiris_common.h"
#include "multisafe.h"

#include <stdbool.h>

void OsirisWrappers2_ResetLog(void);
bool OsirisWrappers2_WriteLog(const char* path);
bool OsirisWrappers2_FileExists(const char* path);

#define OSIRIS_WRAPPERS2_TEST_CALLERS(X) \
	X(osipf_SoundTouch, void, (char* str)) \
	X(osipf_ObjectFindID, int, (char* name)) \
	X(osipf_ObjectFindType, int, (char* name)) \
	X(osipf_WeaponFindID, int, (char* name)) \
	X(Room_Value, void, (int roomnum, char op, char vhandle, void* ptr, int index)) \
	X(osipf_IsRoomValid, ubyte, (int roomnum)) \
	X(osipf_GetAttachParent, int, (int childhandle)) \
	X(osipf_AttachObjectAP, int, (int parenthandle, char parent_ap, int childhandle, char child_ap, ubyte f_use_aligned)) \
	X(osipf_AttachObjectRad, int, (int parenthandle, char parent_ap, int childhandle, float percent_rad)) \
	X(Matcen_Value, void, (int handle, char op, char vtype, void* ptr, int index)) \
	X(Osiris_FreeMemory, void, (void* memory_ptr)) \
	X(Osiris_CancelTimer, void, (int timer_id)) \
	X(Obj_Create, int, (ubyte type, ushort id, int roomnum, vector* pos, const matrix* orient, int parent_handle, vector* initial_velocity)) \
	X(osipf_GameTime, float, (void)) \
	X(osipf_FrameTime, float, (void)) \
	X(Obj_WBValue, void, (int obj_handle, char wb_index, char op, char vtype, void* ptr, char g_index)) \
	X(Osiris_TimerExists, ubyte, (int handle)) \
	X(osipf_MatcenReset, void, (int handle)) \
	X(osipf_MatcenCopy, void, (int dhandle, int shandle)) \
	X(Matcen_Create, int, (char* name)) \
	X(osipf_MatcenFindId, int, (char* str)) \
	X(osipf_MissionFlagSet, void, (int flag, ubyte value)) \
	X(osipf_MissionFlagGet, int, (int flag)) \
	X(Player_Value, void, (int obj_handle, char op, char vhandle, void* ptr, int index)) \
	X(Obj_SetCustomAnim, void, (int handle, float start, float end, float time, char flags, int sound_handle, char next_anim_type)) \
	X(osipf_PlayerAddHudMessage, void, (int handle, char* str)) \
	X(osipf_ObjGhost, void, (int handle, bool f_ghost)) \
	X(Obj_Burning, void, (int handle, float time, float damage_per_second)) \
	X(osipf_ObjIsEffect, bool, (int handle, int type_flag)) \
	X(Sound_Stop, void, (int s_handle, bool f_immediately)) \
	X(Sound_Play2d, int, (int obj_handle, int s_id, float volume)) \
	X(Sound_Play3d, int, (int obj_handle, int s_id, float volume)) \
	X(osipf_SoundFindId, int, (char* s_name)) \
	X(AI_GoalValue, bool, (int obj_handle, char g_index, char op, char vtype, void* ptr, char index)) \
	X(AI_GetNearbyObjs, int, (vector* pos, int init_roomnum, float rad, int* object_handle_list, int max_elements, bool f_lightmap_only, bool f_only_players_and_ais, bool f_include_non_collide_objects, bool f_stop_at_closed_doors)) \
	X(Cinematic_Stop, void, (void)) \
	X(osipf_GetTriggerRoom, int, (int trigger_id)) \
	X(osipf_GetTriggerFace, int, (int trigger_id)) \
	X(Game_CreateRandomSparks, void, (int num_sparks, vector* pos, int roomnum, int which_index, float force_scalar)) \
	X(Osiris_CancelTimerID, void, (int id)) \
	X(osipf_EnableShip, void, (char* ship_name, bool enable)) \
	X(osipf_IsShipEnabled, bool, (char* ship_name)) \
	X(Path_GetInformation, bool, (int pathid, int point, vector* pos, int* room, matrix* orient)) \
	X(LGoal_Value, void, (char op, char vtype, void* ptr, int g_index, int i_index)) \
	X(osipf_ObjKill, void, (int handle, int killer_handle, float damage, int flags, float min_time, float max_time)) \
	X(osipf_GameGetDiffLevel, char, (void)) \
	X(osipf_GetLanguageSetting, int, (void)) \
	X(osipf_PathValue, void, (int path_id, int node_id, char op, int changes, void* ptr))

#ifdef OSIRIS_WRAPPERS2_IS_POINTER

#ifdef _WIN32
#define DLLCALL __stdcall
#else
#define DLLCALL
#endif

#define DECLARE_OSIRIS_WRAPPERS2_POINTER(name, rettype, args) \
        typedef rettype DLLCALL (*name##_fp) args; \
	extern name##_fp name;

OSIRIS_WRAPPERS2_TEST_CALLERS(DECLARE_OSIRIS_WRAPPERS2_POINTER)

#undef DECLARE_OSIRIS_WRAPPERS2_POINTER

#else

void osipf_SoundTouch(char* str);
int osipf_ObjectFindID(char* name);
int osipf_ObjectFindType(char* name);
int osipf_WeaponFindID(char* name);
void osipf_RoomValue(int roomnum, char op, char vhandle, void* ptr, int index);
ubyte osipf_IsRoomValid(int roomnum);
int osipf_GetAttachParent(int childhandle);
int osipf_AttachObjectAP(int parenthandle, char parent_ap, int childhandle, char child_ap, ubyte f_use_aligned);
int osipf_AttachObjectRad(int parenthandle, char parent_ap, int childhandle, float percent_rad);
void osipf_MatcenValue(int handle, char op, char vtype, void* ptr, int index);
void osipf_OsirisFreeMemory(void* memory_ptr);
void osipf_OsirisCancelTimer(int timer_id);
int osipf_ObjCreate(ubyte type, ushort id, int roomnum, vector* pos, const matrix* orient, int parent_handle, vector* initial_velocity);
float osipf_GameTime(void);
float osipf_FrameTime(void);
void osipf_ObjWBValue(int obj_handle, char wb_index, char op, char vtype, void* ptr, char g_index);
ubyte osipf_OsirisTimerExists(int handle);
void osipf_MatcenReset(int handle);
void osipf_MatcenCopy(int dhandle, int shandle);
int osipf_MatcenCreate(char* name);
int osipf_MatcenFindId(char* str);
void osipf_MissionFlagSet(int flag, ubyte value);
int osipf_MissionFlagGet(int flag);
void osipf_PlayerValue(int obj_handle, char op, char vhandle, void* ptr, int index);
void osipf_ObjSetCustomAnim(int handle, float start, float end, float time, char flags, int sound_handle, char next_anim_type);
void osipf_PlayerAddHudMessage(int handle, char* str);
void osipf_ObjGhost(int handle, bool f_ghost);
void osipf_ObjBurning(int handle, float time, float damage_per_second);
bool osipf_ObjIsEffect(int handle, int type_flag);
void osipf_SoundStop(int s_handle, bool f_immediately);
int osipf_SoundPlay2d(int obj_handle, int s_id, float volume);
int osipf_SoundPlay3d(int obj_handle, int s_id, float volume);
int osipf_SoundFindId(char* s_name);
void osipf_AIGoalValue(int obj_handle, char g_index, char op, char vtype, void* ptr, char index);
int osipf_AIGetNearbyObjs(vector* pos, int init_roomnum, float rad, int* object_handle_list, int max_elements, bool f_lightmap_only, bool f_only_players_and_ais, bool f_include_non_collide_objects, bool f_stop_at_closed_doors);
void osipf_CinematicStop(void);
int osipf_GetTriggerRoom(int trigger_id);
int osipf_GetTriggerFace(int trigger_id);
void osipf_GameCreateRandomSparks(int num_sparks, vector* pos, int roomnum, int which_index, float force_scalar);
void osipf_OsirisCancelTimerID(int id);
void osipf_EnableShip(char* ship_name, bool enable);
bool osipf_IsShipEnabled(char* ship_name);
bool osipf_PathGetInformation(int pathid, int point, vector* pos, int* room, matrix* orient);
void osipf_LGoalValue(char op, char vtype, void* ptr, int g_index, int i_index);
void osipf_ObjKill(int handle, int killer_handle, float damage, int flags, float min_time, float max_time);
char osipf_GameGetDiffLevel(void);
int osipf_GetLanguageSetting(void);
void osipf_PathValue(int path_id, int node_id, char op, int changes, void* ptr);

inline void Room_Value(int roomnum, char op, char vhandle, void* ptr, int index = 0)
{
	osipf_RoomValue(roomnum, op, vhandle, ptr, index);
}

inline void Matcen_Value(int handle, char op, char vtype, void* ptr, int index = 0)
{
	osipf_MatcenValue(handle, op, vtype, ptr, index);
}

inline void Osiris_FreeMemory(void* memory_ptr)
{
	osipf_OsirisFreeMemory(memory_ptr);
}

inline void Osiris_CancelTimer(int timer_id)
{
	osipf_OsirisCancelTimer(timer_id);
}

inline int Matcen_Create(char* name)
{
	return osipf_MatcenCreate(name);
}

inline int Obj_Create(ubyte type, ushort id, int roomnum, vector* pos, const matrix* orient = 0, int parent_handle = OBJECT_HANDLE_NONE, vector* initial_velocity = 0)
{
	return osipf_ObjCreate(type, id, roomnum, pos, orient, parent_handle, initial_velocity);
}

inline void Obj_WBValue(int obj_handle, char wb_index, char op, char vtype, void* ptr, char g_index = 0)
{
	osipf_ObjWBValue(obj_handle, wb_index, op, vtype, ptr, g_index);
}

inline ubyte Osiris_TimerExists(int handle)
{
	return osipf_OsirisTimerExists(handle);
}

inline void Player_Value(int obj_handle, char op, char vhandle, void* ptr, int index = 0)
{
	osipf_PlayerValue(obj_handle, op, vhandle, ptr, index);
}

inline void Obj_SetCustomAnim(int handle, float start, float end, float time, char flags, int sound_handle = -1, char next_anim_type = -1)
{
	osipf_ObjSetCustomAnim(handle, start, end, time, flags, sound_handle, next_anim_type);
}

inline void Obj_Burning(int handle, float time, float damage_per_second = 1.0f)
{
	osipf_ObjBurning(handle, time, damage_per_second);
}

inline void Sound_Stop(int s_handle, bool f_immediately = false)
{
	osipf_SoundStop(s_handle, f_immediately);
}

inline int Sound_Play2d(int obj_handle, int s_id, float volume = 1.0f)
{
	return osipf_SoundPlay2d(obj_handle, s_id, volume);
}

inline int Sound_Play3d(int obj_handle, int s_id, float volume = 1.0f)
{
	return osipf_SoundPlay3d(obj_handle, s_id, volume);
}

inline bool AI_GoalValue(int obj_handle, char g_index, char op, char vtype, void* ptr, char index = 0)
{
	osipf_AIGoalValue(obj_handle, g_index, op, vtype, ptr, index);
	return true;
}

inline int AI_GetNearbyObjs(vector* pos, int init_roomnum, float rad, int* object_handle_list, int max_elements, bool f_lightmap_only, bool f_only_players_and_ais = true, bool f_include_non_collide_objects = false, bool f_stop_at_closed_doors = true)
{
	return osipf_AIGetNearbyObjs(pos, init_roomnum, rad, object_handle_list, max_elements, f_lightmap_only, f_only_players_and_ais, f_include_non_collide_objects, f_stop_at_closed_doors);
}

inline void Cinematic_Stop(void)
{
	osipf_CinematicStop();
}

inline void Game_CreateRandomSparks(int num_sparks, vector* pos, int roomnum, int which_index = -1, float force_scalar = 1.0f)
{
	osipf_GameCreateRandomSparks(num_sparks, pos, roomnum, which_index, force_scalar);
}

inline void Osiris_CancelTimerID(int id)
{
	osipf_OsirisCancelTimerID(id);
}

inline bool Path_GetInformation(int pathid, int point, vector* pos = 0, int* room = 0, matrix* orient = 0)
{
	return osipf_PathGetInformation(pathid, point, pos, room, orient);
}

inline void LGoal_Value(char op, char vtype, void* ptr, int g_index = -1, int i_index = -1)
{
	osipf_LGoalValue(op, vtype, ptr, g_index, i_index);
}

#endif

#endif
