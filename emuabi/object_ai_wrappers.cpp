#define INCLUDED_FROM_D3
#include "emu86.h"
#include "emuint.h"
#include "pstypes.h"
#include "osiris_predefs.h"
#include "vecmat_external.h"
#include <cstdint>
#include <cstring>

//template <>
//vector *emu_arg<vector *>(Emu *emu86, int n) { return static_cast<vector *>(emu_arg<void *>(emu86, n)); }

template <typename T>
static T *vm_ptr(Emu86FunCtx& ctx, emu_ptr_t addr)
{
	if (!ctx.emu86 || !ctx.emu86->as.base)
		return nullptr;
	if (addr >= ctx.emu86->as.size)
		return nullptr;
	return reinterpret_cast<T *>(ctx.emu86->as.base + addr);
}


// float osipf_ObjectGetTimeLived(int objhandle);
void emucall_osipf_ObjectGetTimeLived(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	ctx.set_return(osipf_ObjectGetTimeLived(objhandle));
}

// void Obj_GetGunPos(int objhandle, int gun_number, vector *gun_pnt, vector *gun_normal);
void emucall_Obj_GetGunPos(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int gun_number = ctx.arg<int>(1);
	vector *gun_pnt = ctx.arg<vector *>(2, sizeof(vector));
	vector *gun_normal = ctx.arg<vector *>(3, sizeof(vector));
	osipf_GetGunPos(objhandle, gun_number, gun_pnt, gun_normal);
}

// void Obj_GetGroundPos(int objhandle, int ground_number, vector *ground_pnt, vector *ground_normal);
void emucall_Obj_GetGroundPos(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int ground_number = ctx.arg<int>(1);
	vector *ground_pnt = ctx.arg<vector *>(2, sizeof(vector));
	vector *ground_normal = ctx.arg<vector *>(3, sizeof(vector));
	osipf_GetGroundPos(objhandle, ground_number, ground_pnt, ground_normal);
}

// int osipf_GetNumAttachSlots(int objhandle);
void emucall_osipf_GetNumAttachSlots(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	ctx.set_return(osipf_GetNumAttachSlots(objhandle));
}

// int osipf_GetAttachChildHandle(int objhandle, char attachpoint);
void emucall_osipf_GetAttachChildHandle(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	char attachpoint = ctx.arg<char>(1);
	ctx.set_return(osipf_GetAttachChildHandle(objhandle, attachpoint));
}

// void osipf_UnattachFromParent(int objhandle);
void emucall_osipf_UnattachFromParent(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	osipf_UnattachFromParent(objhandle);
}

// void osipf_UnattachChild(int objhandle, char parent_ap);
void emucall_osipf_UnattachChild(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	char parent_ap = ctx.arg<char>(1);
	osipf_UnattachChild(objhandle, parent_ap);
}

// void osipf_UnattachChildren(int objhandle);
void emucall_osipf_UnattachChildren(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	osipf_UnattachChildren(objhandle);
}

// int osipf_RayCast(int objhandle, vector *p0, vector *p1, int start_roomnum, float rad, int flags, ray_info *ri);
void emucall_osipf_RayCast(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	vector *p0 = ctx.arg<vector *>(1);
	vector *p1 = ctx.arg<vector *>(2);
	int start_roomnum = ctx.arg<int>(3);
	float rad = ctx.arg<float>(4);
	int flags = ctx.arg<int>(5);
	ray_info *ri = ctx.arg<ray_info *>(6);
	ctx.set_return(osipf_RayCast(objhandle, p0, p1, start_roomnum, rad, flags, ri));
}

// int osipf_AIGetPathID(char *string);
void emucall_osipf_AIGetPathID(Emu86FunCtx& ctx, void *)
{
	emu86_check_cstr(ctx.emu86, ctx.arg<emu_ptr_t>(0));
	char *string = ctx.arg<char *>(0);
	ctx.set_return(osipf_AIGetPathID(string));
}

// int AI_GoalFollowPathSimple(int objhandle, int path_id, int guid, int flags, int slot);
void emucall_AI_GoalFollowPathSimple(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int path_id = ctx.arg<int>(1);
	int guid = ctx.arg<int>(2);
	int flags = ctx.arg<int>(3);
	int slot = ctx.arg<int>(4);
	ctx.set_return(osipf_AIGoalFollowPathSimple(objhandle, path_id, guid, flags, slot));
}

// int osipf_AIPowerSwitch(int objhandle, ubyte f_power_on);
void emucall_osipf_AIPowerSwitch(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	ubyte f_power_on = ctx.arg<ubyte>(1);
	ctx.set_return(osipf_AIPowerSwitch(objhandle, f_power_on));
}

// void osipf_AIValue(int objhandle, char op, char vtype, void *ptr);
void emucall_osipf_AIValue(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	char op = ctx.arg<char>(1);
	char vtype = ctx.arg<char>(2);
	void *ptr = ctx.arg<void *>(3, 4);
	osipf_AIValue(objhandle, op, vtype, ptr);
}

// void Obj_Value(int objhandle, char op, char vtype, void *ptr, int index);
void emucall_Obj_Value(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	char op = ctx.arg<char>(1);
	char vtype = ctx.arg<char>(2);
	void *ptr = ctx.arg<void *>(3,
		vtype == OBJV_V_POS ? sizeof(vector) : vtype == OBJV_M_ORIENT ? sizeof(matrix) : sizeof(emu_ptr_t));
	int index = ctx.arg<int>(4);
	osipf_ObjectValue(objhandle, op, vtype, ptr, index);
}

// ubyte osipf_AITurnTowardsVectors(int objhandle, vector *fvec, vector *uvec);
void emucall_osipf_AITurnTowardsVectors(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	vector *fvec = ctx.arg<vector *>(1, sizeof(vector));
	vector *uvec = ctx.arg<vector *>(2, sizeof(vector));
	ctx.set_return(osipf_AITurnTowardsVectors(objhandle, fvec, uvec));
}

// void osipf_AISetType(int objhandle, int type);
void emucall_osipf_AISetType(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int type = ctx.arg<int>(1);
	osipf_AISetType(objhandle, type);
}

// vector osipf_AIFindHidePos(int hideobjhandle, int viewobjhandle, float time, int *hide_room);
// Hidden pointer is arg(4), return value is the hidden pointer
void emucall_osipf_AIFindHidePos(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t ret = ctx.arg<emu_ptr_t>(0);
	vector *ret_vector = vm_ptr<vector>(ctx, ret);
	int hideobjhandle = ctx.arg<int>(1);
	int viewobjhandle = ctx.arg<int>(2);
	float time = ctx.arg<float>(3);
	int *hide_room = ctx.arg<int *>(4, sizeof(emu_ptr_t));
	if (!ret_vector)
		return;
	*ret_vector = osipf_AIFindHidePos(hideobjhandle, viewobjhandle, time, hide_room);
	ctx.set_return(ret);
}

// int osipf_AIGoalAddEnabler(int objhandle, int goal_index, int enabler_type, float percent, float interval, void *ptr);
void emucall_osipf_AIGoalAddEnabler(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int goal_index = ctx.arg<int>(1);
	int enabler_type = ctx.arg<int>(2);
	float percent = ctx.arg<float>(3);
	float interval = ctx.arg<float>(4);
	void *ptr = ctx.arg<void *>(5);
	ctx.set_return(osipf_AIGoalAddEnabler(objhandle, goal_index, enabler_type, percent, interval, ptr));
}

void emucall_osipf_AIGoalAdd(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int goal_type = ctx.arg<int>(1);
	int level = ctx.arg<int>(2);
	float influence = ctx.arg<float>(3);
	int guid = ctx.arg<int>(4);
	int flags = ctx.arg<int>(5);

	switch (goal_type)
	{
	case AIG_GET_AWAY_FROM_OBJ:
	case AIG_GET_TO_OBJ:
	case AIG_GUARD_OBJ:
	case AIG_DODGE_OBJ:
	case AIG_MOVE_AROUND_OBJ:
	case AIG_MOVE_RELATIVE_OBJ:
	case AIG_GET_AROUND_OBJ:
	{
		int objref = ctx.arg<int>(6);
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, objref));
		return;
	}

	case AIG_FOLLOW_PATH:
	{
		int path_id = ctx.arg<int>(6);
		int start_node = ctx.arg<int>(7);
		int end_node = ctx.arg<int>(8);
		int next_node = ctx.arg<int>(9);
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags,
			path_id, start_node, end_node, next_node));
		return;
	}

	case AIG_ATTACH_TO_OBJ:
	case AIG_PLACE_OBJ_ON_OBJ:
	{
		int handle = ctx.arg<int>(6);
		char parent_ap = ctx.arg<char>(7);
		char child_ap = ctx.arg<char>(8);
		float rad = static_cast<float>(ctx.arg<double>(9));
		int aligned = ctx.arg<int>(11);
		int sphere = ctx.arg<int>(12);

		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags,
			handle, parent_ap, child_ap, rad, aligned, sphere));
		return;
	}

	case AIG_FIRE_AT_OBJ:
	{
		short wb = static_cast<short>(ctx.arg<int>(6));
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, wb));
		return;
	}

	case AIG_MOVE_RELATIVE_OBJ_VEC:
	{
		int handle = ctx.arg<int>(6);
		int i_value = ctx.arg<int>(7);
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, handle, i_value));
		return;
	}

	case AIG_HIDE_FROM_OBJ:
	{
		int handle = ctx.arg<int>(6);
		int time = ctx.arg<int>(7);
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, handle, time));
		return;
	}

	case AIG_GUARD_AREA:
	case AIG_GET_TO_POS:
	{
		vector *pos = ctx.arg<vector *>(6);
		int roomnum = ctx.arg<int>(7);
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, pos, roomnum));
		return;
	}

	case AIG_MELEE_TARGET:
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags));
		return;

	case AIG_SET_ANIM:
	case AIG_DO_MELEE_ANIM:
	case AIG_USE_MOVEMENT_TYPE:
	case AIG_SCRIPTED:
	{
		int i_value = ctx.arg<int>(6);
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, i_value));
		return;
	}

	case AIG_WANDER_AROUND:
	{
		int i_value = ctx.arg<int>(6);
		int ignored = ctx.arg<int>(7);
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, i_value, ignored));
		return;
	}

	case AIG_FACE_DIR:
	{
		vector *v_value = ctx.arg<vector *>(6, sizeof(vector));
		ctx.set_return(osipf_AIGoalAdd(objhandle, goal_type, level, influence, guid, flags, v_value));
		return;
	}

	default:
		ctx.set_return(-1);
		return;
	}
}

// void osipf_AIGoalClear(int objhandle, int goal_index);
void emucall_osipf_AIGoalClear(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int goal_index = ctx.arg<int>(1);
	osipf_AIGoalClear(objhandle, goal_index);
}

// int AI_FindObjOfType(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle);
void emucall_AI_FindObjOfType(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int type = ctx.arg<int>(1);
	int id = ctx.arg<int>(2);
	bool f_ignore_init_room = ctx.arg<bool>(3);
	int parent_handle = ctx.arg<int>(4);
	ctx.set_return(osipf_AIFindObjOfType(objhandle, type, id, f_ignore_init_room, parent_handle));
}

// vector osipf_AIGetRoomPathPoint(int roomnum);
// Hidden pointer is arg(1), return value is the hidden pointer
void emucall_osipf_AIGetRoomPathPoint(Emu86FunCtx& ctx, void *)
{
	emu_ptr_t ret = ctx.arg<emu_ptr_t>(0);
	vector *ret_vector = vm_ptr<vector>(ctx, ret);
	int roomnum = ctx.arg<int>(1);
	if (!ret_vector)
		return;
	*ret_vector = osipf_AIGetRoomPathPoint(roomnum);
	ctx.set_return(ret);
}

// int osipf_AIFindEnergyCenter(int objhandle);
void emucall_osipf_AIFindEnergyCenter(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	ctx.set_return(osipf_AIFindEnergyCenter(objhandle));
}

// float osipf_AIGetDistToObj(int objhandle, int otherobjhandle);
void emucall_osipf_AIGetDistToObj(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int otherobjhandle = ctx.arg<int>(1);
	ctx.set_return(osipf_AIGetDistToObj(objhandle, otherobjhandle));
}

// int osipf_AISetGoalFlags(int objhandle, int goal_handle, int flags, ubyte f_enable);
void emucall_osipf_AISetGoalFlags(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int goal_handle = ctx.arg<int>(1);
	int flags = ctx.arg<int>(2);
	ubyte f_enable = ctx.arg<ubyte>(3);
	ctx.set_return(osipf_AISetGoalFlags(objhandle, goal_handle, flags, f_enable));
}

// void osipf_AISetGoalCircleDist(int objhandle, int goal_handle, float dist);
void emucall_osipf_AISetGoalCircleDist(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int goal_handle = ctx.arg<int>(1);
	float dist = ctx.arg<float>(2);
	osipf_AISetGoalCircleDist(objhandle, goal_handle, dist);
}

// bool osipf_AIIsObjFriend(int obj_handle, int it_handle);
void emucall_osipf_AIIsObjFriend(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	int it_handle = ctx.arg<int>(1);
	ctx.set_return(osipf_AIIsObjFriend(obj_handle, it_handle));
}

// bool osipf_AIIsObjEnemy(int obj_handle, int it_handle);
void emucall_osipf_AIIsObjEnemy(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	int it_handle = ctx.arg<int>(1);
	ctx.set_return(osipf_AIIsObjEnemy(obj_handle, it_handle));
}

// char osipf_AIGetCurGoalIndex(int obj_handle);
void emucall_osipf_AIGetCurGoalIndex(Emu86FunCtx& ctx, void *)
{
	int obj_handle = ctx.arg<int>(0);
	ctx.set_return(osipf_AIGetCurGoalIndex(obj_handle));
}

// int osipf_ObjMakeListOfType(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle, int max_recorded, int *handles);
void emucall_osipf_ObjMakeListOfType(Emu86FunCtx& ctx, void *)
{
	int objhandle = ctx.arg<int>(0);
	int type = ctx.arg<int>(1);
	int id = ctx.arg<int>(2);
	bool f_ignore_init_room = ctx.arg<bool>(3);
	int parent_handle = ctx.arg<int>(4);
	int max_recorded = ctx.arg<int>(5);
	int *handles = ctx.arg<int *>(6);
	ctx.set_return(osipf_ObjMakeListOfType(objhandle, type, id, f_ignore_init_room, parent_handle, max_recorded, handles));
}

// bool osipf_AIIsDestReachable(int handle, int room);
void emucall_osipf_AIIsDestReachable(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	int room = ctx.arg<int>(1);
	ctx.set_return(osipf_AIIsDestReachable(handle, room));
}

// bool osipf_AIIsObjReachable(int handle, int target);
void emucall_osipf_AIIsObjReachable(Emu86FunCtx& ctx, void *)
{
	int handle = ctx.arg<int>(0);
	int target = ctx.arg<int>(1);
	ctx.set_return(osipf_AIIsObjReachable(handle, target));
}
emu86_ctx_fun_t kObjectAIWrapperFuns[] = {
	{"osipf_ObjectGetTimeLived", emucall_osipf_ObjectGetTimeLived, 1, nullptr},
	{"Obj_GetGunPos", emucall_Obj_GetGunPos, 4, nullptr},
	{"osipf_GetGunPos", emucall_Obj_GetGunPos, 4, nullptr},
	{"Obj_GetGroundPos", emucall_Obj_GetGroundPos, 4, nullptr},
	{"osipf_GetGroundPos", emucall_Obj_GetGroundPos, 4, nullptr},
	{"osipf_GetNumAttachSlots", emucall_osipf_GetNumAttachSlots, 1, nullptr},
	{"osipf_GetAttachChildHandle", emucall_osipf_GetAttachChildHandle, 2, nullptr},
	{"osipf_UnattachFromParent", emucall_osipf_UnattachFromParent, 1, nullptr},
	{"osipf_UnattachChild", emucall_osipf_UnattachChild, 2, nullptr},
	{"osipf_UnattachChildren", emucall_osipf_UnattachChildren, 1, nullptr},
	{"osipf_RayCast", emucall_osipf_RayCast, 7, nullptr},
	{"osipf_AIGetPathID", emucall_osipf_AIGetPathID, 1, nullptr},
	{"AI_GoalFollowPathSimple", emucall_AI_GoalFollowPathSimple, 5, nullptr},
	{"osipf_AIGoalFollowPathSimple", emucall_AI_GoalFollowPathSimple, 5, nullptr},
	{"osipf_AIPowerSwitch", emucall_osipf_AIPowerSwitch, 2, nullptr},
	{"osipf_AIValue", emucall_osipf_AIValue, 4, nullptr},
	{"Obj_Value", emucall_Obj_Value, 5, nullptr},
	{"osipf_ObjectValue", emucall_Obj_Value, 5, nullptr},
	{"osipf_AITurnTowardsVectors", emucall_osipf_AITurnTowardsVectors, 3, nullptr},
	{"osipf_AISetType", emucall_osipf_AISetType, 2, nullptr},
	{"osipf_AIFindHidePos", emucall_osipf_AIFindHidePos, 5, nullptr},
	{"osipf_AIGoalAddEnabler", emucall_osipf_AIGoalAddEnabler, 6, nullptr},
	{"osipf_AIGoalAdd", emucall_osipf_AIGoalAdd, 0, nullptr},
	{"osipf_AIGoalClear", emucall_osipf_AIGoalClear, 2, nullptr},
	{"AI_FindObjOfType", emucall_AI_FindObjOfType, 5, nullptr},
	{"osipf_AIFindObjOfType", emucall_AI_FindObjOfType, 5, nullptr},
	{"osipf_AIGetRoomPathPoint", emucall_osipf_AIGetRoomPathPoint, 2, nullptr},
	{"osipf_AIFindEnergyCenter", emucall_osipf_AIFindEnergyCenter, 1, nullptr},
	{"osipf_AIGetDistToObj", emucall_osipf_AIGetDistToObj, 2, nullptr},
	{"osipf_AISetGoalFlags", emucall_osipf_AISetGoalFlags, 4, nullptr},
	{"osipf_AISetGoalCircleDist", emucall_osipf_AISetGoalCircleDist, 3, nullptr},
	{"osipf_AIIsObjFriend", emucall_osipf_AIIsObjFriend, 2, nullptr},
	{"osipf_AIIsObjEnemy", emucall_osipf_AIIsObjEnemy, 2, nullptr},
	{"osipf_AIGetCurGoalIndex", emucall_osipf_AIGetCurGoalIndex, 1, nullptr},
	{"osipf_ObjMakeListOfType", emucall_osipf_ObjMakeListOfType, 7, nullptr},
	{"osipf_AIIsDestReachable", emucall_osipf_AIIsDestReachable, 2, nullptr},
	{"osipf_AIIsObjReachable", emucall_osipf_AIIsObjReachable, 2, nullptr},
};

size_t kObjectAIWrapperFunsCount = sizeof(kObjectAIWrapperFuns) / sizeof(kObjectAIWrapperFuns[0]);
