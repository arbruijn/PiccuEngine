#include "object_ai_wrappers_callers.h"

#include "object_ai_wrappers_test.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

bool Debug_break = false;
bool Debug_print_block = false;

namespace
{
	static std::string g_log;

	static void AppendFormatted(const char* format, va_list args)
	{
		char buffer[4096];
		va_list copy;
		va_copy(copy, args);
		const int written = vsnprintf(buffer, sizeof(buffer), format, copy);
		va_end(copy);

		if (written < 0)
		{
			return;
		}

		if (written < static_cast<int>(sizeof(buffer)))
		{
			g_log.append(buffer, written);
			return;
		}

		std::string dynamic_buffer;
		dynamic_buffer.resize(static_cast<size_t>(written) + 1);
		vsnprintf(&dynamic_buffer[0], dynamic_buffer.size(), format, args);
		g_log.append(dynamic_buffer.c_str(), static_cast<size_t>(written));
	}

	static void ObjectAILogf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		AppendFormatted(format, args);
		va_end(args);
	}

	static void LogVector(const char* label, const vector& v)
	{
		ObjectAILogf("%s=(%.3f,%.3f,%.3f)", label, v.x, v.y, v.z);
	}

	static void LogRayInfo(const ray_info& ri)
	{
		ObjectAILogf(
			"fate=%d hit_room=%d hit_point=(%.3f,%.3f,%.3f) hit_object=%d hit_subobject=%d hit_face=%d "
			"hit_face_pnt=(%.3f,%.3f,%.3f) hit_face_room=%d hit_wallnorm=(%.3f,%.3f,%.3f)",
			ri.fate, ri.hit_room, ri.hit_point.x, ri.hit_point.y, ri.hit_point.z,
			ri.hit_object, ri.hit_subobject, ri.hit_face,
			ri.hit_face_pnt.x, ri.hit_face_pnt.y, ri.hit_face_pnt.z,
			ri.hit_face_room, ri.hit_wallnorm.x, ri.hit_wallnorm.y, ri.hit_wallnorm.z);
	}

	static void LogTypedValue(char vtype, void* ptr)
	{
		switch (vtype)
		{
		case 'i':
			ObjectAILogf("%d", ptr ? *static_cast<int*>(ptr) : 0);
			break;
		case 'f':
			ObjectAILogf("%.3f", ptr ? *static_cast<float*>(ptr) : 0.0f);
			break;
		case 'b':
			ObjectAILogf("%u", ptr ? static_cast<unsigned>(*static_cast<ubyte*>(ptr)) : 0u);
			break;
		case 'c':
			ObjectAILogf("'%c'", ptr ? *static_cast<char*>(ptr) : '?');
			break;
		default:
			ObjectAILogf("<unsupported>");
			break;
		}
	}

	static bool FileExists(const char* path)
	{
		FILE* file = fopen(path, "rb");
		if (!file)
		{
			return false;
		}
		fclose(file);
		return true;
	}

	static bool WriteFile(const char* path, const std::string& contents)
	{
		FILE* file = fopen(path, "wb");
		if (!file)
		{
			return false;
		}

		const size_t written = fwrite(contents.data(), 1, contents.size(), file);
		fclose(file);
		return written == contents.size();
	}
}

void Debug_ConsolePrintf(int n, char* format, ...)
{
	(void)n;
	va_list args;
	va_start(args, format);
	AppendFormatted(format, args);
	va_end(args);
}

void Debug_ConsolePrintf(int n, int row, int col, char* format, ...)
{
	(void)n;
	(void)row;
	(void)col;
	va_list args;
	va_start(args, format);
	AppendFormatted(format, args);
	va_end(args);
}

void ObjectAI_ResetLog(void)
{
	g_log.clear();
}

bool ObjectAI_WriteLog(const char* path)
{
	return WriteFile(path, g_log);
}

bool ObjectAI_FileExists(const char* path)
{
	return FileExists(path);
}

float osipf_ObjectGetTimeLived(int objhandle)
{
	ObjectAILogf("osipf_ObjectGetTimeLived objhandle=%d\n", objhandle);
	return 42.0f;
}

void osipf_GetGunPos(int objhandle, int gun_number, vector* gun_pnt, vector* gun_normal)
{
	ObjectAILogf("osipf_GetGunPos objhandle=%d gun_number=%d gun_pnt=(%.3f,%.3f,%.3f) gun_normal=(%.3f,%.3f,%.3f)\n",
		objhandle, gun_number,
		gun_pnt ? gun_pnt->x : 0.0f, gun_pnt ? gun_pnt->y : 0.0f, gun_pnt ? gun_pnt->z : 0.0f,
		gun_normal ? gun_normal->x : 0.0f, gun_normal ? gun_normal->y : 0.0f, gun_normal ? gun_normal->z : 0.0f);
}

void osipf_GetGroundPos(int objhandle, int ground_number, vector* ground_pnt, vector* ground_normal)
{
	ObjectAILogf("osipf_GetGroundPos objhandle=%d ground_number=%d ground_pnt=(%.3f,%.3f,%.3f) ground_normal=(%.3f,%.3f,%.3f)\n",
		objhandle, ground_number,
		ground_pnt ? ground_pnt->x : 0.0f, ground_pnt ? ground_pnt->y : 0.0f, ground_pnt ? ground_pnt->z : 0.0f,
		ground_normal ? ground_normal->x : 0.0f, ground_normal ? ground_normal->y : 0.0f, ground_normal ? ground_normal->z : 0.0f);
}

int osipf_GetNumAttachSlots(int objhandle)
{
	ObjectAILogf("osipf_GetNumAttachSlots objhandle=%d\n", objhandle);
	return 4;
}

int osipf_GetAttachChildHandle(int objhandle, char attachpoint)
{
	ObjectAILogf("osipf_GetAttachChildHandle objhandle=%d attachpoint='%c'\n", objhandle, attachpoint);
	return 2001;
}

void osipf_UnattachFromParent(int objhandle)
{
	ObjectAILogf("osipf_UnattachFromParent objhandle=%d\n", objhandle);
}

void osipf_UnattachChild(int objhandle, char parent_ap)
{
	ObjectAILogf("osipf_UnattachChild objhandle=%d parent_ap='%c'\n", objhandle, parent_ap);
}

void osipf_UnattachChildren(int objhandle)
{
	ObjectAILogf("osipf_UnattachChildren objhandle=%d\n", objhandle);
}

int osipf_RayCast(int objhandle, vector* p0, vector* p1, int start_roomnum, float rad, int flags, ray_info* ri)
{
	ObjectAILogf(
		"osipf_RayCast objhandle=%d p0=(%.3f,%.3f,%.3f) p1=(%.3f,%.3f,%.3f) start_roomnum=%d rad=%.3f flags=%d ",
		objhandle,
		p0 ? p0->x : 0.0f, p0 ? p0->y : 0.0f, p0 ? p0->z : 0.0f,
		p1 ? p1->x : 0.0f, p1 ? p1->y : 0.0f, p1 ? p1->z : 0.0f,
		start_roomnum, rad, flags);
	if (ri)
	{
		LogRayInfo(*ri);
	}
	ObjectAILogf("\n");
	return 1;
}

int osipf_AIGetPathID(char* string)
{
	ObjectAILogf("osipf_AIGetPathID string=\"%s\"\n", string ? string : "<null>");
	return 77;
}

int osipf_AIGoalFollowPathSimple(int objhandle, int path_id, int guid, int flags, int slot)
{
	ObjectAILogf("osipf_AIGoalFollowPathSimple objhandle=%d path_id=%d guid=%d flags=%d slot=%d\n",
		objhandle, path_id, guid, flags, slot);
	return 1;
}

int osipf_AIPowerSwitch(int objhandle, ubyte f_power_on)
{
	ObjectAILogf("osipf_AIPowerSwitch objhandle=%d f_power_on=%u\n", objhandle, static_cast<unsigned>(f_power_on));
	return 1;
}

void osipf_AIValue(int objhandle, char op, char vtype, void* ptr)
{
	ObjectAILogf("osipf_AIValue objhandle=%d op='%c' vtype='%c' value=", objhandle, op, vtype);
	LogTypedValue(vtype, ptr);
	ObjectAILogf("\n");
}

void osipf_ObjectValue(int objhandle, char op, char vtype, void* ptr, int index)
{
	ObjectAILogf("osipf_ObjectValue objhandle=%d op='%c' vtype='%c' index=%d value=", objhandle, op, vtype, index);
	LogTypedValue(vtype, ptr);
	ObjectAILogf("\n");
}

ubyte osipf_AITurnTowardsVectors(int objhandle, vector* fvec, vector* uvec)
{
	ObjectAILogf("osipf_AITurnTowardsVectors objhandle=%d fvec=(%.3f,%.3f,%.3f) uvec=(%.3f,%.3f,%.3f)\n",
		objhandle,
		fvec ? fvec->x : 0.0f, fvec ? fvec->y : 0.0f, fvec ? fvec->z : 0.0f,
		uvec ? uvec->x : 0.0f, uvec ? uvec->y : 0.0f, uvec ? uvec->z : 0.0f);
	return 1;
}

void osipf_AISetType(int objhandle, int type)
{
	ObjectAILogf("osipf_AISetType objhandle=%d type=%d\n", objhandle, type);
}

vector osipf_AIFindHidePos(int hideobjhandle, int viewobjhandle, float time, int* hide_room)
{
	vector hidden = { 1.0f, 2.0f, 3.0f };
	if (hide_room)
	{
		*hide_room = 7;
	}

	ObjectAILogf("osipf_AIFindHidePos hideobjhandle=%d viewobjhandle=%d time=%.3f hide_room=%d hidden=(%.3f,%.3f,%.3f)\n",
		hideobjhandle, viewobjhandle, time, hide_room ? *hide_room : -1, hidden.x, hidden.y, hidden.z);
	return hidden;
}

int osipf_AIGoalAddEnabler(int objhandle, int goal_index, int enabler_type, float percent, float interval, void* ptr)
{
	ObjectAILogf("osipf_AIGoalAddEnabler objhandle=%d goal_index=%d enabler_type=%d percent=%.3f interval=%.3f ptr_value=%d\n",
		objhandle, goal_index, enabler_type, percent, interval, ptr ? *static_cast<int*>(ptr) : 0);
	return 1;
}

int osipf_AIGoalAdd(int objhandle, int goal_type, int level, float influence, int guid, int flags)
{
	ObjectAILogf("osipf_AIGoalAdd objhandle=%d goal_type=%d level=%d influence=%.3f guid=%d flags=%d\n",
		objhandle, goal_type, level, influence, guid, flags);
	return 123;
}

void osipf_AIGoalClear(int objhandle, int goal_index)
{
	ObjectAILogf("osipf_AIGoalClear objhandle=%d goal_index=%d\n", objhandle, goal_index);
}

int osipf_AIFindObjOfType(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle)
{
	ObjectAILogf("osipf_AIFindObjOfType objhandle=%d type=%d id=%d f_ignore_init_room=%u parent_handle=%d\n",
		objhandle, type, id, f_ignore_init_room ? 1u : 0u, parent_handle);
	return 456;
}

vector osipf_AIGetRoomPathPoint(int roomnum)
{
	vector path_point = { 4.0f, 5.0f, 6.0f };
	ObjectAILogf("osipf_AIGetRoomPathPoint roomnum=%d path_point=(%.3f,%.3f,%.3f)\n",
		roomnum, path_point.x, path_point.y, path_point.z);
	return path_point;
}

int osipf_AIFindEnergyCenter(int objhandle)
{
	ObjectAILogf("osipf_AIFindEnergyCenter objhandle=%d\n", objhandle);
	return 789;
}

float osipf_AIGetDistToObj(int objhandle, int otherobjhandle)
{
	ObjectAILogf("osipf_AIGetDistToObj objhandle=%d otherobjhandle=%d\n", objhandle, otherobjhandle);
	return 12.5f;
}

int osipf_AISetGoalFlags(int objhandle, int goal_handle, int flags, ubyte f_enable)
{
	ObjectAILogf("osipf_AISetGoalFlags objhandle=%d goal_handle=%d flags=%d f_enable=%u\n",
		objhandle, goal_handle, flags, static_cast<unsigned>(f_enable));
	return flags;
}

void osipf_AISetGoalCircleDist(int objhandle, int goal_handle, float dist)
{
	ObjectAILogf("osipf_AISetGoalCircleDist objhandle=%d goal_handle=%d dist=%.3f\n", objhandle, goal_handle, dist);
}

bool osipf_AIIsObjFriend(int obj_handle, int it_handle)
{
	ObjectAILogf("osipf_AIIsObjFriend obj_handle=%d it_handle=%d\n", obj_handle, it_handle);
	return true;
}

bool osipf_AIIsObjEnemy(int obj_handle, int it_handle)
{
	ObjectAILogf("osipf_AIIsObjEnemy obj_handle=%d it_handle=%d\n", obj_handle, it_handle);
	return false;
}

char osipf_AIGetCurGoalIndex(int obj_handle)
{
	ObjectAILogf("osipf_AIGetCurGoalIndex obj_handle=%d\n", obj_handle);
	return 3;
}

int osipf_ObjMakeListOfType(int objhandle, int type, int id, bool f_ignore_init_room, int parent_handle, int max_recorded, int* handles)
{
	ObjectAILogf("osipf_ObjMakeListOfType objhandle=%d type=%d id=%d f_ignore_init_room=%u parent_handle=%d max_recorded=%d ",
		objhandle, type, id, f_ignore_init_room ? 1u : 0u, parent_handle, max_recorded);

	const int num_recorded = max_recorded < 3 ? max_recorded : 3;
	for (int i = 0; i < num_recorded; ++i)
	{
		handles[i] = 4000 + i;
	}

	ObjectAILogf("handles=[");
	for (int i = 0; i < num_recorded; ++i)
	{
		ObjectAILogf("%s%d", i ? "," : "", handles[i]);
	}
	ObjectAILogf("]\n");

	return num_recorded;
}

bool osipf_AIIsDestReachable(int handle, int room)
{
	ObjectAILogf("osipf_AIIsDestReachable handle=%d room=%d\n", handle, room);
	return true;
}

bool osipf_AIIsObjReachable(int handle, int target)
{
	ObjectAILogf("osipf_AIIsObjReachable handle=%d target=%d\n", handle, target);
	return false;
}
