#ifndef TESTS_OBJECT_AI_WRAPPERS_TEST_H
#define TESTS_OBJECT_AI_WRAPPERS_TEST_H

#include "osiris_common.h"

void ObjectAI_ResetLog(void);
bool ObjectAI_WriteLog(const char* path);
bool ObjectAI_FileExists(const char* path);

#ifdef OBJECT_AI_WRAPPERS_IS_POINTER
#ifdef _WIN32
#define DLLCALL __stdcall
#else
#define DLLCALL
#endif
typedef void DLLCALL (*Obj_GetGunPos_fp)(int objhandle, int gun_number, vector* gun_pnt, vector* gun_normal);
typedef void DLLCALL (*Obj_GetGroundPos_fp)(int objhandle, int ground_number, vector* ground_pnt, vector* ground_normal);
typedef int DLLCALL (*AI_GoalFollowPathSimple_fp)(int objhandle, int path_id, int guid, int flags, int slot);
typedef void DLLCALL (*Obj_Value_fp)(int objhandle, char op, char vtype, void* ptr, int index);
typedef int DLLCALL (*AI_FindObjOfType_fp)(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle);

typedef float DLLCALL (*osipf_ObjectGetTimeLived_fp)(int objhandle);
typedef int DLLCALL (*osipf_GetNumAttachSlots_fp)(int objhandle);
typedef int DLLCALL (*osipf_GetAttachChildHandle_fp)(int objhandle, char attachpoint);
typedef void DLLCALL (*osipf_UnattachFromParent_fp)(int objhandle);
typedef void DLLCALL (*osipf_UnattachChild_fp)(int objhandle, char parent_ap);
typedef void DLLCALL (*osipf_UnattachChildren_fp)(int objhandle);
typedef int DLLCALL (*osipf_RayCast_fp)(int objhandle, vector* p0, vector* p1, int start_roomnum, float rad, int flags, ray_info* ri);
typedef int DLLCALL (*osipf_AIGetPathID_fp)(char* string);
typedef int DLLCALL (*osipf_AIGoalFollowPathSimple_fp)(int objhandle, int path_id, int guid, int flags, int slot);
typedef int DLLCALL (*osipf_AIPowerSwitch_fp)(int objhandle, ubyte f_power_on);
typedef void DLLCALL (*osipf_AIValue_fp)(int objhandle, char op, char vtype, void* ptr);
typedef void DLLCALL (*osipf_ObjectValue_fp)(int objhandle, char op, char vtype, void* ptr, int index);
typedef ubyte DLLCALL (*osipf_AITurnTowardsVectors_fp)(int objhandle, vector* fvec, vector* uvec);
typedef void DLLCALL (*osipf_AISetType_fp)(int objhandle, int type);
typedef vector DLLCALL (*osipf_AIFindHidePos_fp)(int hideobjhandle, int viewobjhandle, float time, int* hide_room);
typedef int DLLCALL (*osipf_AIGoalAddEnabler_fp)(int objhandle, int goal_index, int enabler_type, float percent, float interval, void* ptr);
typedef int DLLCALL (*osipf_AIGoalAdd_fp)(int objhandle, int goal_type, int level, float influence, int guid, int flags, ...);
typedef void DLLCALL (*osipf_AIGoalClear_fp)(int objhandle, int goal_index);
typedef int DLLCALL (*osipf_AIFindObjOfType_fp)(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle);
typedef vector DLLCALL (*osipf_AIGetRoomPathPoint_fp)(int roomnum);
typedef int DLLCALL (*osipf_AIFindEnergyCenter_fp)(int objhandle);
typedef float DLLCALL (*osipf_AIGetDistToObj_fp)(int objhandle, int otherobjhandle);
typedef int DLLCALL (*osipf_AISetGoalFlags_fp)(int objhandle, int goal_handle, int flags, ubyte f_enable);
typedef void DLLCALL (*osipf_AISetGoalCircleDist_fp)(int objhandle, int goal_handle, float dist);
typedef bool DLLCALL (*osipf_AIIsObjFriend_fp)(int obj_handle, int it_handle);
typedef bool DLLCALL (*osipf_AIIsObjEnemy_fp)(int obj_handle, int it_handle);
typedef char DLLCALL (*osipf_AIGetCurGoalIndex_fp)(int obj_handle);
typedef int DLLCALL (*osipf_ObjMakeListOfType_fp)(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle, int max_recorded, int* handles);
typedef bool DLLCALL (*osipf_AIIsDestReachable_fp)(int handle, int room);
typedef bool DLLCALL (*osipf_AIIsObjReachable_fp)(int handle, int target);

extern osipf_ObjectGetTimeLived_fp osipf_ObjectGetTimeLived;
extern osipf_GetNumAttachSlots_fp osipf_GetNumAttachSlots;
extern osipf_GetAttachChildHandle_fp osipf_GetAttachChildHandle;
extern osipf_UnattachFromParent_fp osipf_UnattachFromParent;
extern osipf_UnattachChild_fp osipf_UnattachChild;
extern osipf_UnattachChildren_fp osipf_UnattachChildren;
extern osipf_RayCast_fp osipf_RayCast;
extern osipf_AIGetPathID_fp osipf_AIGetPathID;
extern osipf_AIGoalFollowPathSimple_fp osipf_AIGoalFollowPathSimple;
extern osipf_AIPowerSwitch_fp osipf_AIPowerSwitch;
extern osipf_AIValue_fp osipf_AIValue;
extern osipf_ObjectValue_fp osipf_ObjectValue;
extern osipf_AITurnTowardsVectors_fp osipf_AITurnTowardsVectors;
extern osipf_AISetType_fp osipf_AISetType;
extern osipf_AIFindHidePos_fp osipf_AIFindHidePos;
extern osipf_AIGoalAddEnabler_fp osipf_AIGoalAddEnabler;
extern osipf_AIGoalAdd_fp osipf_AIGoalAdd;
extern osipf_AIGoalClear_fp osipf_AIGoalClear;
extern osipf_AIFindObjOfType_fp osipf_AIFindObjOfType;
extern osipf_AIGetRoomPathPoint_fp osipf_AIGetRoomPathPoint;
extern osipf_AIFindEnergyCenter_fp osipf_AIFindEnergyCenter;
extern osipf_AIGetDistToObj_fp osipf_AIGetDistToObj;
extern osipf_AISetGoalFlags_fp osipf_AISetGoalFlags;
extern osipf_AISetGoalCircleDist_fp osipf_AISetGoalCircleDist;
extern osipf_AIIsObjFriend_fp osipf_AIIsObjFriend;
extern osipf_AIIsObjEnemy_fp osipf_AIIsObjEnemy;
extern osipf_AIGetCurGoalIndex_fp osipf_AIGetCurGoalIndex;
extern osipf_ObjMakeListOfType_fp osipf_ObjMakeListOfType;
extern osipf_AIIsDestReachable_fp osipf_AIIsDestReachable;
extern osipf_AIIsObjReachable_fp osipf_AIIsObjReachable;
extern Obj_GetGunPos_fp Obj_GetGunPos;
extern Obj_GetGroundPos_fp Obj_GetGroundPos;
extern AI_GoalFollowPathSimple_fp AI_GoalFollowPathSimple;
extern Obj_Value_fp Obj_Value;
extern AI_FindObjOfType_fp AI_FindObjOfType;
#else
float osipf_ObjectGetTimeLived(int objhandle);
void osipf_GetGunPos(int objhandle, int gun_number, vector* gun_pnt, vector* gun_normal);
void osipf_GetGroundPos(int objhandle, int ground_number, vector* ground_pnt, vector* ground_normal);
int osipf_GetNumAttachSlots(int objhandle);
int osipf_GetAttachChildHandle(int objhandle, char attachpoint);
void osipf_UnattachFromParent(int objhandle);
void osipf_UnattachChild(int objhandle, char parent_ap);
void osipf_UnattachChildren(int objhandle);
int osipf_RayCast(int objhandle, vector* p0, vector* p1, int start_roomnum, float rad, int flags, ray_info* ri);
int osipf_AIGetPathID(char* string);
int osipf_AIGoalFollowPathSimple(int objhandle, int path_id, int guid, int flags, int slot);
int osipf_AIPowerSwitch(int objhandle, ubyte f_power_on);
void osipf_AIValue(int objhandle, char op, char vtype, void* ptr);
void osipf_ObjectValue(int objhandle, char op, char vtype, void* ptr, int index);
ubyte osipf_AITurnTowardsVectors(int objhandle, vector* fvec, vector* uvec);
void osipf_AISetType(int objhandle, int type);
vector osipf_AIFindHidePos(int hideobjhandle, int viewobjhandle, float time, int* hide_room);
int osipf_AIGoalAddEnabler(int objhandle, int goal_index, int enabler_type, float percent, float interval, void* ptr);
int osipf_AIGoalAdd(int objhandle, int goal_type, int level, float influence, int guid, int flags, ...);
void osipf_AIGoalClear(int objhandle, int goal_index);
int osipf_AIFindObjOfType(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle);
vector osipf_AIGetRoomPathPoint(int roomnum);
int osipf_AIFindEnergyCenter(int objhandle);
float osipf_AIGetDistToObj(int objhandle, int otherobjhandle);
int osipf_AISetGoalFlags(int objhandle, int goal_handle, int flags, ubyte f_enable);
void osipf_AISetGoalCircleDist(int objhandle, int goal_handle, float dist);
bool osipf_AIIsObjFriend(int obj_handle, int it_handle);
bool osipf_AIIsObjEnemy(int obj_handle, int it_handle);
char osipf_AIGetCurGoalIndex(int obj_handle);
int osipf_ObjMakeListOfType(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle, int max_recorded, int* handles);
bool osipf_AIIsDestReachable(int handle, int room);
bool osipf_AIIsObjReachable(int handle, int target);

inline void Obj_GetGunPos(int objhandle, int gun_number, vector* gun_pnt, vector* gun_normal)
{
	osipf_GetGunPos(objhandle, gun_number, gun_pnt, gun_normal);
}

inline void Obj_GetGroundPos(int objhandle, int ground_number, vector* ground_pnt, vector* ground_normal)
{
	osipf_GetGroundPos(objhandle, ground_number, ground_pnt, ground_normal);
}

inline int AI_GoalFollowPathSimple(int objhandle, int path_id, int guid, int flags, int slot)
{
	return osipf_AIGoalFollowPathSimple(objhandle, path_id, guid, flags, slot);
}

inline void Obj_Value(int objhandle, char op, char vtype, void* ptr, int index)
{
	osipf_ObjectValue(objhandle, op, vtype, ptr, index);
}

inline int AI_FindObjOfType(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle)
{
	return osipf_AIFindObjOfType(objhandle, type, id, f_ignore_init_room, parent_handle);
}
#endif

#endif
