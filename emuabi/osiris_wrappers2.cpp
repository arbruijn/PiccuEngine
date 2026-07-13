#define INCLUDED_FROM_D3
#include "emu86.h"
#include "emuint.h"
#include "pstypes.h"
#include "eventinfo.h"
#include "osiris_predefs.h"
#include "osiris_dll.h"
#include "gamecinematics.h"
#include "vecmat_external.h"
#include "osiris_common.h"
#include "gamecinematics_external.h"
#include "multisafe.h"
#include <cstdint>

void Osiris_CancelTimerID(int id);

namespace
{
template <typename T>
static T *vm_ptr(Emu86FunCtx& ctx, emu_ptr_t addr)
{
	if (!ctx.emu86 || !ctx.emu86->as.base)
		return nullptr;
	if (addr >= ctx.emu86->as.size)
		return nullptr;
	return reinterpret_cast<T *>(ctx.emu86->as.base + addr);
}
}

void emucall_osipf_CallObjectEvent(Emu86FunCtx& ctx, void *)
{
	int objnum = ctx.arg<int>(0);
	int event = ctx.arg<int>(1);
	emu_ptr_t event_info_ptr = ctx.arg<emu_ptr_t>(2);
	tOSIRISEventInfo event_info;
	void *host_event_info = vm_ptr<void>(ctx, event_info_ptr);
	emuabi::VmPtrDecoder vm = { ctx.emu86 ? ctx.emu86->as.base : nullptr };
	emuabi::decode_event_info(event, host_event_info, event_info, vm);
	ctx.set_return(osipf_CallObjectEvent(objnum, event, &event_info));
	if (host_event_info && ctx.emu86 && ctx.emu86->as.base)
	{
		emuabi::VmPtrEncoder encoder = { ctx.emu86->as.base };
		emuabi::encode_event_info(event, event_info, host_event_info, encoder);
	}
}

void emucall_osipf_CallTriggerEvent(Emu86FunCtx& ctx, void *)
{
	int trignum = ctx.arg<int>(0);
	int event = ctx.arg<int>(1);
	emu_ptr_t event_info_ptr = ctx.arg<emu_ptr_t>(2);
	tOSIRISEventInfo event_info;
	void *host_event_info = vm_ptr<void>(ctx, event_info_ptr);
	emuabi::VmPtrDecoder vm = { ctx.emu86 ? ctx.emu86->as.base : nullptr };
	emuabi::decode_event_info(event, host_event_info, event_info, vm);
	ctx.set_return(osipf_CallTriggerEvent(trignum, event, &event_info));
	if (host_event_info && ctx.emu86 && ctx.emu86->as.base)
	{
		emuabi::VmPtrEncoder encoder = { ctx.emu86->as.base };
		emuabi::encode_event_info(event, event_info, host_event_info, encoder);
	}
}

void emucall_osipf_SoundTouch(Emu86FunCtx& ctx, void *)
{
	char *sound_name = ctx.arg<char *>(0);
	osipf_SoundTouch(sound_name);
}

void emucall_osipf_ObjectFindID(Emu86FunCtx& ctx, void *)
{
	char *object_name = ctx.arg<char *>(0);
	ctx.set_return(osipf_ObjectFindID(object_name));
}

void emucall_osipf_ObjectFindType(Emu86FunCtx& ctx, void *)
{
	char *object_name = ctx.arg<char *>(0);
	ctx.set_return(osipf_ObjectFindType(object_name));
}

void emucall_osipf_WeaponFindID(Emu86FunCtx& ctx, void *)
{
	char *weapon_name = ctx.arg<char *>(0);
	ctx.set_return(osipf_WeaponFindID(weapon_name));
}

void emucall_Room_Value(Emu86FunCtx& ctx, void *)
{
	int roomnum = ctx.arg<int>(0);
	char op = ctx.arg<char>(1);
	char vhandle = ctx.arg<char>(2);
	void *ptr = ctx.arg<void *>(3);
	int index = ctx.arg<int>(4);
	osipf_RoomValue(roomnum, op, vhandle, ptr, index);
}

void emucall_osipf_IsRoomValid(Emu86FunCtx& ctx, void *)
{
	int roomnum = ctx.arg<int>(0);
	ctx.set_return(osipf_IsRoomValid(roomnum));
}

void emucall_osipf_GetAttachParent(Emu86FunCtx& ctx, void *)
{
	int childhandle = ctx.arg<int>(0);
	ctx.set_return(osipf_GetAttachParent(childhandle));
}

void emucall_osipf_AttachObjectAP(Emu86FunCtx& ctx, void *)
{
	int parenthandle = ctx.arg<int>(0);
	char parent_ap = ctx.arg<char>(1);
	int childhandle = ctx.arg<int>(2);
	char child_ap = ctx.arg<char>(3);
	ubyte f_use_aligned = static_cast<ubyte>(ctx.arg<int>(4));
	ctx.set_return(osipf_AttachObjectAP(parenthandle, parent_ap, childhandle, child_ap, f_use_aligned));
}

void emucall_osipf_AttachObjectRad(Emu86FunCtx& ctx, void *)
{
	int parenthandle = ctx.arg<int>(0);
	char parent_ap = ctx.arg<char>(1);
	int childhandle = ctx.arg<int>(2);
	float percent_rad = ctx.arg<float>(3);
	ctx.set_return(osipf_AttachObjectRad(parenthandle, parent_ap, childhandle, percent_rad));
}

void emucall_Matcen_Value(Emu86FunCtx& ctx, void *)
{
	int matcen_handle = ctx.arg<int>(0);
	char op = ctx.arg<char>(1);
	char vtype = ctx.arg<char>(2);
	void *ptr = ctx.arg<void *>(3);
	int prod_index = ctx.arg<int>(4);
	osipf_MatcenValue(matcen_handle, op, vtype, ptr, prod_index);
}

void emucall_Osiris_FreeMemory(Emu86FunCtx& ctx, void *)
{
	void *memory_ptr = ctx.arg<void *>(0);
	Osiris_FreeMemory(memory_ptr);
}

void emucall_Osiris_CancelTimer(Emu86FunCtx& ctx, void *)
{
	int timer_id = ctx.arg<int>(0);
	Osiris_CancelTimer(timer_id);
}

void emucall_Obj_Create(Emu86FunCtx& ctx, void *)
{
	ubyte type = static_cast<ubyte>(ctx.arg<int>(0));
	ushort id = static_cast<ushort>(ctx.arg<int>(1));
	int roomnum = ctx.arg<int>(2);
	vector *pos = ctx.arg<vector *>(3);
	const matrix *orient = ctx.arg<const matrix *>(4);
	int parent_handle = ctx.arg<int>(5);
	vector *initial_velocity = ctx.arg<vector *>(6);
	ctx.set_return(osipf_ObjCreate(type, id, roomnum, pos, orient, parent_handle, initial_velocity));
}

void emucall_osipf_GameTime(Emu86FunCtx& ctx, void *)
{
	ctx.set_return(osipf_GameTime());
}

void emucall_osipf_FrameTime(Emu86FunCtx& ctx, void *)
{
	ctx.set_return(osipf_FrameTime());
}

void emucall_Obj_WBValue(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	char wb_index = ctx.arg<char>(1);
	char op = ctx.arg<char>(2);
	char vtype = ctx.arg<char>(3);
	void *ptr = ctx.arg<void *>(4);
	char g_index = ctx.arg<char>(5);
	osipf_ObjWBValue(obj_handle, wb_index, op, vtype, ptr, g_index);
}

void emucall_Osiris_TimerExists(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	ctx.set_return(Osiris_TimerExists(handle));
}

void emucall_osipf_MatcenReset(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	osipf_MatcenReset(handle);
}

void emucall_osipf_MatcenCopy(Emu86FunCtx& ctx, void *)
{
	int dhandle = ctx.arg<int>(0);
	int shandle = ctx.arg<int>(1);
	osipf_MatcenCopy(dhandle, shandle);
}

void emucall_osipf_MatcenFindId(Emu86FunCtx& ctx, void *)
{
	char *str = ctx.arg<char *>(0);
	ctx.set_return(osipf_MatcenFindId(str));
}

void emucall_osipf_MissionFlagSet(Emu86FunCtx& ctx, void *)
{
	int flag = ctx.arg<int>(0);
	ubyte value = static_cast<ubyte>(ctx.arg<int>(1));
	osipf_MissionFlagSet(flag, value);
}

void emucall_osipf_MissionFlagGet(Emu86FunCtx& ctx, void *)
{
	int flag = ctx.arg<int>(0);
	ctx.set_return(osipf_MissionFlagGet(flag));
}

void emucall_Player_Value(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	char op = ctx.arg<char>(1);
	char vhandle = ctx.arg<char>(2);
	void *ptr = ctx.arg<void *>(3);
	int index = ctx.arg<int>(4);
	osipf_PlayerValue(obj_handle, op, vhandle, ptr, index);
}

void emucall_Obj_SetCustomAnim(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	float start = ctx.arg<float>(1);
	float end = ctx.arg<float>(2);
	float time = ctx.arg<float>(3);
	char flags = ctx.arg<char>(4);
	int sound_handle = ctx.arg<int>(5);
	char next_anim_type = ctx.arg<char>(6);
	osipf_ObjectCustomAnim(handle, start, end, time, flags, sound_handle, next_anim_type);
}

void emucall_osipf_PlayerAddHudMessage(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	char *str = ctx.arg<char *>(1);
	osipf_PlayerAddHudMessage(handle, str);
}

void emucall_osipf_ObjGhost(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	bool f_ghost = ctx.arg<bool>(1);
	osipf_ObjGhost(handle, f_ghost);
}

void emucall_Obj_Burning(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	float time = ctx.arg<float>(1);
	float damage_per_second = ctx.arg<float>(2);
	osipf_ObjBurning(handle, time, damage_per_second);
}

void emucall_osipf_ObjIsEffect(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	int type_flag = ctx.arg<int>(1);
	ctx.set_return(osipf_ObjIsEffect(handle, type_flag));
}

void emucall_Sound_Stop(Emu86FunCtx& ctx, void *)
{
	int s_handle = ctx.arg<int>(0);
	bool f_immediately = ctx.arg<bool>(1);
	osipf_SoundStop(s_handle, f_immediately);
}

void emucall_Sound_Play2d(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	int s_id = ctx.arg<int>(1);
	float volume = ctx.arg<float>(2);
	ctx.set_return(osipf_SoundPlay2d(obj_handle, s_id, volume));
}

void emucall_Sound_Play3d(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	int s_id = ctx.arg<int>(1);
	float volume = ctx.arg<float>(2);
	ctx.set_return(osipf_SoundPlay3d(obj_handle, s_id, volume));
}

void emucall_osipf_SoundFindId(Emu86FunCtx& ctx, void *)
{
	char *s_name = ctx.arg<char *>(0);
	ctx.set_return(osipf_SoundFindId(s_name));
}

void emucall_AI_GoalValue(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	char g_index = ctx.arg<char>(1);
	char op = ctx.arg<char>(2);
	char vtype = ctx.arg<char>(3);
	void *ptr = ctx.arg<void *>(4);
	char index = ctx.arg<char>(5);
	osipf_AIGoalValue(obj_handle, g_index, op, vtype, ptr, index);
	ctx.set_return(1);
}

void emucall_AI_GetNearbyObjs(Emu86FunCtx& ctx, void *)
{
	vector *pos = ctx.arg<vector *>(0);
	int init_roomnum = ctx.arg<int>(1);
	float rad = ctx.arg<float>(2);
	int *object_handle_list = ctx.arg<int *>(3);
	int max_elements = ctx.arg<int>(4);
	bool f_lightmap_only = ctx.arg<bool>(5);
	bool f_only_players_and_ais = ctx.arg<bool>(6);
	bool f_include_non_collide_objects = ctx.arg<bool>(7);
	bool f_stop_at_closed_doors = ctx.arg<bool>(8);
	ctx.set_return(osipf_AIGetNearbyObjs(pos, init_roomnum, rad, object_handle_list, max_elements, f_lightmap_only, f_only_players_and_ais, f_include_non_collide_objects, f_stop_at_closed_doors));
}

void emucall_Cinematic_Stop(Emu86FunCtx& ctx, void *)
{
	Cinematic_Stop();
}

void emucall_osipf_GetTriggerRoom(Emu86FunCtx& ctx, void *)
{
	int trigger_id = ctx.arg<int>(0);
	ctx.set_return(osipf_GetTriggerRoom(trigger_id));
}

void emucall_osipf_GetTriggerFace(Emu86FunCtx& ctx, void *)
{
	int trigger_id = ctx.arg<int>(0);
	ctx.set_return(osipf_GetTriggerFace(trigger_id));
}

void emucall_Game_CreateRandomSparks(Emu86FunCtx& ctx, void *)
{
	int num_sparks = ctx.arg<int>(0);
	vector *pos = ctx.arg<vector *>(1);
	int roomnum = ctx.arg<int>(2);
	int which_index = ctx.arg<int>(3);
	float force_scalar = ctx.arg<float>(4);
	osipf_CreateRandomSparks(num_sparks, pos, roomnum, which_index, force_scalar);
}

void emucall_Osiris_CancelTimerID(Emu86FunCtx& ctx, void *)
{
	int id = ctx.arg<int>(0);
	Osiris_CancelTimerID(id);
}

void emucall_osipf_EnableShip(Emu86FunCtx& ctx, void *)
{
	char *ship_name = ctx.arg<char *>(0);
	bool enable = ctx.arg<bool>(1);
	osipf_EnableShip(ship_name, enable);
}

void emucall_osipf_IsShipEnabled(Emu86FunCtx& ctx, void *)
{
	char *ship_name = ctx.arg<char *>(0);
	ctx.set_return(osipf_IsShipEnabled(ship_name));
}

void emucall_Path_GetInformation(Emu86FunCtx& ctx, void *)
{
	int pathid = ctx.arg<int>(0);
	int point = ctx.arg<int>(1);
	vector *pos = ctx.arg<vector *>(2);
	int *room = ctx.arg<int *>(3);
	matrix *orient = ctx.arg<matrix *>(4);
	ctx.set_return(osipf_PathGetInformation(pathid, point, pos, room, orient));
}

void emucall_LGoal_Value(Emu86FunCtx& ctx, void *)
{
	char op = ctx.arg<char>(0);
	char vtype = ctx.arg<char>(1);
	void *ptr = ctx.arg<void *>(2);
	int g_index = ctx.arg<int>(3);
	int i_index = ctx.arg<int>(4);
	osipf_LGoalValue(op, vtype, ptr, g_index, i_index);
}

void emucall_osipf_ObjKill(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	int killer_handle = ctx.arg<int>(1);
	float damage = ctx.arg<float>(2);
	int flags = ctx.arg<int>(3);
	float min_time = ctx.arg<float>(4);
	float max_time = ctx.arg<float>(5);
	osipf_ObjKill(handle, killer_handle, damage, flags, min_time, max_time);
}

void emucall_osipf_GameGetDiffLevel(Emu86FunCtx& ctx, void *)
{
	ctx.set_return(osipf_GameGetDiffLevel());
}

void emucall_osipf_GetLanguageSetting(Emu86FunCtx& ctx, void *)
{
	ctx.set_return(osipf_GetLanguageSetting());
}

void emucall_osipf_PathValue(Emu86FunCtx& ctx, void *)
{
	int path_id = ctx.arg<int>(0);
	int node_id = ctx.arg<int>(1);
	char op = ctx.arg<char>(2);
	int changes = ctx.arg<int>(3);
	void *ptr = ctx.arg<void *>(4);
	osipf_PathValue(path_id, node_id, op, changes, ptr);
}

emu86_ctx_fun_t kOsirisWrapperFuns2[] = {
	{"osipf_SoundTouch", emucall_osipf_SoundTouch, 1, nullptr},
	{"osipf_ObjectFindID", emucall_osipf_ObjectFindID, 1, nullptr},
	{"osipf_ObjectFindType", emucall_osipf_ObjectFindType, 1, nullptr},
	{"osipf_WeaponFindID", emucall_osipf_WeaponFindID, 1, nullptr},
	{"osipf_CallObjectEvent", emucall_osipf_CallObjectEvent, 3, nullptr},
	{"osipf_CallTriggerEvent", emucall_osipf_CallTriggerEvent, 3, nullptr},
	{"Room_Value", emucall_Room_Value, 5, nullptr},
	{"osipf_RoomValue", emucall_Room_Value, 5, nullptr},
	{"osipf_IsRoomValid", emucall_osipf_IsRoomValid, 1, nullptr},
	{"osipf_GetAttachParent", emucall_osipf_GetAttachParent, 1, nullptr},
	{"osipf_AttachObjectAP", emucall_osipf_AttachObjectAP, 5, nullptr},
	{"osipf_AttachObjectRad", emucall_osipf_AttachObjectRad, 4, nullptr},
	{"Matcen_Value", emucall_Matcen_Value, 5, nullptr},
	{"osipf_MatcenValue", emucall_Matcen_Value, 5, nullptr},
	{"Osiris_FreeMemory", emucall_Osiris_FreeMemory, 1, nullptr},
	{"Osiris_CancelTimer", emucall_Osiris_CancelTimer, 1, nullptr},
	{"Obj_Create", emucall_Obj_Create, 7, nullptr},
	{"osipf_ObjCreate", emucall_Obj_Create, 7, nullptr},
	{"osipf_GameTime", emucall_osipf_GameTime, 0, nullptr},
	{"osipf_FrameTime", emucall_osipf_FrameTime, 0, nullptr},
	{"Obj_WBValue", emucall_Obj_WBValue, 6, nullptr},
	{"osipf_ObjWBValue", emucall_Obj_WBValue, 6, nullptr},
	{"Osiris_TimerExists", emucall_Osiris_TimerExists, 1, nullptr},
	{"osipf_MatcenReset", emucall_osipf_MatcenReset, 1, nullptr},
	{"osipf_MatcenCopy", emucall_osipf_MatcenCopy, 2, nullptr},
	{"osipf_MatcenFindId", emucall_osipf_MatcenFindId, 1, nullptr},
	{"osipf_MissionFlagSet", emucall_osipf_MissionFlagSet, 2, nullptr},
	{"osipf_MissionFlagGet", emucall_osipf_MissionFlagGet, 1, nullptr},
	{"Player_Value", emucall_Player_Value, 5, nullptr},
	{"osipf_PlayerValue", emucall_Player_Value, 5, nullptr},
	{"Obj_SetCustomAnim", emucall_Obj_SetCustomAnim, 7, nullptr},
	{"osipf_ObjectCustomAnim", emucall_Obj_SetCustomAnim, 7, nullptr},
	{"osipf_PlayerAddHudMessage", emucall_osipf_PlayerAddHudMessage, 2, nullptr},
	{"osipf_ObjGhost", emucall_osipf_ObjGhost, 2, nullptr},
	{"Obj_Burning", emucall_Obj_Burning, 3, nullptr},
	{"osipf_ObjBurning", emucall_Obj_Burning, 3, nullptr},
	{"osipf_ObjIsEffect", emucall_osipf_ObjIsEffect, 2, nullptr},
	{"Sound_Stop", emucall_Sound_Stop, 2, nullptr},
	{"osipf_SoundStop", emucall_Sound_Stop, 2, nullptr},
	{"Sound_Play2d", emucall_Sound_Play2d, 3, nullptr},
	{"osipf_SoundPlay2d", emucall_Sound_Play2d, 3, nullptr},
	{"Sound_Play3d", emucall_Sound_Play3d, 3, nullptr},
	{"osipf_SoundPlay3d", emucall_Sound_Play3d, 3, nullptr},
	{"osipf_SoundFindId", emucall_osipf_SoundFindId, 1, nullptr},
	{"AI_GoalValue", emucall_AI_GoalValue, 6, nullptr},
	{"osipf_AIGoalValue", emucall_AI_GoalValue, 6, nullptr},
	{"AI_GetNearbyObjs", emucall_AI_GetNearbyObjs, 9, nullptr},
	{"osipf_AIGetNearbyObjs", emucall_AI_GetNearbyObjs, 9, nullptr},
	{"Cinematic_Stop", emucall_Cinematic_Stop, 0, nullptr},
	{"osipf_GetTriggerRoom", emucall_osipf_GetTriggerRoom, 1, nullptr},
	{"osipf_GetTriggerFace", emucall_osipf_GetTriggerFace, 1, nullptr},
	{"Game_CreateRandomSparks", emucall_Game_CreateRandomSparks, 5, nullptr},
	{"osipf_CreateRandomSparks", emucall_Game_CreateRandomSparks, 5, nullptr},
	{"Osiris_CancelTimerID", emucall_Osiris_CancelTimerID, 1, nullptr},
	{"osipf_EnableShip", emucall_osipf_EnableShip, 2, nullptr},
	{"osipf_IsShipEnabled", emucall_osipf_IsShipEnabled, 1, nullptr},
	{"Path_GetInformation", emucall_Path_GetInformation, 5, nullptr},
	{"osipf_PathGetInformation", emucall_Path_GetInformation, 5, nullptr},
	{"LGoal_Value", emucall_LGoal_Value, 5, nullptr},
	{"osipf_LGoalValue", emucall_LGoal_Value, 5, nullptr},
	{"osipf_ObjKill", emucall_osipf_ObjKill, 6, nullptr},
	{"osipf_GameGetDiffLevel", emucall_osipf_GameGetDiffLevel, 0, nullptr},
	{"osipf_GetLanguageSetting", emucall_osipf_GetLanguageSetting, 0, nullptr},
	{"osipf_PathValue", emucall_osipf_PathValue, 5, nullptr},
};

size_t kOsirisWrapperFuns2Count = sizeof(kOsirisWrapperFuns2) / sizeof(kOsirisWrapperFuns2[0]);
