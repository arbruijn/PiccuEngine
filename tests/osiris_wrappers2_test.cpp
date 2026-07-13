#include "osiris_wrappers2_test.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

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

	static void OsirisWrappers2Logf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		AppendFormatted(format, args);
		va_end(args);
	}

	static void LogVector(const vector& v)
	{
		OsirisWrappers2Logf("(%.3f,%.3f,%.3f)", v.x, v.y, v.z);
	}

	static void LogMatrix(const matrix& m)
	{
		OsirisWrappers2Logf(
			"rvec=(%.3f,%.3f,%.3f) uvec=(%.3f,%.3f,%.3f) fvec=(%.3f,%.3f,%.3f)",
			m.rvec.x, m.rvec.y, m.rvec.z,
			m.uvec.x, m.uvec.y, m.uvec.z,
			m.fvec.x, m.fvec.y, m.fvec.z);
	}

	static void LogTypedValue(char kind, void* ptr)
	{
		switch (kind)
		{
		case 'i':
			OsirisWrappers2Logf("%d", ptr ? *static_cast<int*>(ptr) : 0);
			break;
		case 'f':
			OsirisWrappers2Logf("%.3f", ptr ? *static_cast<float*>(ptr) : 0.0f);
			break;
		case 'b':
			OsirisWrappers2Logf("%u", ptr ? static_cast<unsigned>(*static_cast<ubyte*>(ptr)) : 0u);
			break;
		case 'c':
			OsirisWrappers2Logf("'%c'", ptr ? *static_cast<char*>(ptr) : '?');
			break;
		case 's':
			OsirisWrappers2Logf("\"%s\"", ptr ? static_cast<char*>(ptr) : "<null>");
			break;
		case 'v':
			if (ptr)
			{
				LogVector(*static_cast<vector*>(ptr));
			}
			else
			{
				OsirisWrappers2Logf("<null>");
			}
			break;
		case 'm':
			if (ptr)
			{
				LogMatrix(*static_cast<matrix*>(ptr));
			}
			else
			{
				OsirisWrappers2Logf("<null>");
			}
			break;
		default:
			OsirisWrappers2Logf("%p", ptr);
			break;
		}
	}

	static void LogMsafe(const msafe_struct* ms)
	{
		if (!ms)
		{
			OsirisWrappers2Logf("<null>");
			return;
		}

		OsirisWrappers2Logf(
			"roomnum=%d objhandle=%u ithandle=%u scalar=%.3f interval=%.3f state=%u count=%d flags=%d pos=",
			ms->roomnum, ms->objhandle, ms->ithandle, ms->scalar, ms->interval,
			static_cast<unsigned>(ms->state), ms->count, ms->flags);
		LogVector(ms->pos);
		OsirisWrappers2Logf(" pos2=");
		LogVector(ms->pos2);
		OsirisWrappers2Logf(" message=\"%s\" message2=\"%s\"", ms->message, ms->message2);
	}

	static void LogVectorList(const int* handles, int count)
	{
		OsirisWrappers2Logf("[");
		for (int i = 0; i < count; ++i)
		{
			OsirisWrappers2Logf("%s%d", i ? "," : "", handles ? handles[i] : 0);
		}
		OsirisWrappers2Logf("]");
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

void OsirisWrappers2_ResetLog(void)
{
	g_log.clear();
}

bool OsirisWrappers2_WriteLog(const char* path)
{
	return WriteFile(path, g_log);
}

bool OsirisWrappers2_FileExists(const char* path)
{
	return FileExists(path);
}

void osipf_SoundTouch(char* str)
{
	OsirisWrappers2Logf("osipf_SoundTouch str=\"%s\"\n", str ? str : "<null>");
}

int osipf_ObjectFindID(char* name)
{
	OsirisWrappers2Logf("osipf_ObjectFindID name=\"%s\"\n", name ? name : "<null>");
	return 101;
}

int osipf_ObjectFindType(char* name)
{
	OsirisWrappers2Logf("osipf_ObjectFindType name=\"%s\"\n", name ? name : "<null>");
	return 102;
}

int osipf_WeaponFindID(char* name)
{
	OsirisWrappers2Logf("osipf_WeaponFindID name=\"%s\"\n", name ? name : "<null>");
	return 103;
}

void osipf_RoomValue(int roomnum, char op, char vhandle, void* ptr, int index)
{
	OsirisWrappers2Logf("osipf_RoomValue roomnum=%d op='%c' vhandle='%c' index=%d value=", roomnum, op, vhandle, index);
	LogTypedValue(vhandle, ptr);
	OsirisWrappers2Logf("\n");
}

ubyte osipf_IsRoomValid(int roomnum)
{
	OsirisWrappers2Logf("osipf_IsRoomValid roomnum=%d\n", roomnum);
	return 2;
}

int osipf_GetAttachParent(int childhandle)
{
	OsirisWrappers2Logf("osipf_GetAttachParent childhandle=%d\n", childhandle);
	return 204;
}

int osipf_AttachObjectAP(int parenthandle, char parent_ap, int childhandle, char child_ap, ubyte f_use_aligned)
{
	OsirisWrappers2Logf(
		"osipf_AttachObjectAP parenthandle=%d parent_ap='%c' childhandle=%d child_ap='%c' f_use_aligned=%u\n",
		parenthandle, parent_ap, childhandle, child_ap, static_cast<unsigned>(f_use_aligned));
	return 1;
}

int osipf_AttachObjectRad(int parenthandle, char parent_ap, int childhandle, float percent_rad)
{
	OsirisWrappers2Logf(
		"osipf_AttachObjectRad parenthandle=%d parent_ap='%c' childhandle=%d percent_rad=%.3f\n",
		parenthandle, parent_ap, childhandle, percent_rad);
	return 1;
}

void osipf_MatcenValue(int handle, char op, char vtype, void* ptr, int index)
{
	OsirisWrappers2Logf("osipf_MatcenValue handle=%d op='%c' vtype='%c' index=%d value=", handle, op, vtype, index);
	LogTypedValue(vtype, ptr);
	OsirisWrappers2Logf("\n");
}

void osipf_OsirisFreeMemory(void* memory_ptr)
{
	OsirisWrappers2Logf("osipf_OsirisFreeMemory memory_ptr=%p\n", memory_ptr);
}

void osipf_OsirisCancelTimer(int timer_id)
{
	OsirisWrappers2Logf("osipf_OsirisCancelTimer timer_id=%d\n", timer_id);
}

int osipf_ObjCreate(ubyte type, ushort id, int roomnum, vector* pos, const matrix* orient, int parent_handle, vector* initial_velocity)
{
	OsirisWrappers2Logf(
		"osipf_ObjCreate type=%u id=%u roomnum=%d pos=",
		static_cast<unsigned>(type), static_cast<unsigned>(id), roomnum);
	if (pos)
	{
		LogVector(*pos);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf(" orient=");
	if (orient)
	{
		LogMatrix(*orient);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf(" parent_handle=%d initial_velocity=", parent_handle);
	if (initial_velocity)
	{
		LogVector(*initial_velocity);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf("\n");
	return 5001;
}

float osipf_GameTime(void)
{
	OsirisWrappers2Logf("osipf_GameTime\n");
	return 123.5f;
}

float osipf_FrameTime(void)
{
	OsirisWrappers2Logf("osipf_FrameTime\n");
	return 0.25f;
}

void osipf_ObjWBValue(int obj_handle, char wb_index, char op, char vtype, void* ptr, char g_index)
{
	OsirisWrappers2Logf("osipf_ObjWBValue obj_handle=%d wb_index=%d op='%c' vtype='%c' g_index=%d value=", obj_handle, static_cast<int>(wb_index), op, vtype, static_cast<int>(g_index));
	LogTypedValue(vtype, ptr);
	OsirisWrappers2Logf("\n");
}

ubyte osipf_OsirisTimerExists(int handle)
{
	OsirisWrappers2Logf("osipf_OsirisTimerExists handle=%d\n", handle);
	return 1;
}

void osipf_MatcenReset(int handle)
{
	OsirisWrappers2Logf("osipf_MatcenReset handle=%d\n", handle);
}

void osipf_MatcenCopy(int dhandle, int shandle)
{
	OsirisWrappers2Logf("osipf_MatcenCopy dhandle=%d shandle=%d\n", dhandle, shandle);
}

int osipf_MatcenFindId(char* str)
{
	OsirisWrappers2Logf("osipf_MatcenFindId str=\"%s\"\n", str ? str : "<null>");
	return 77;
}

int osipf_MatcenCreate(char* name)
{
	OsirisWrappers2Logf("osipf_MatcenCreate name=\"%s\"\n", name ? name : "<null>");
	return 78;
}

void osipf_MissionFlagSet(int flag, ubyte value)
{
	OsirisWrappers2Logf("osipf_MissionFlagSet flag=%d value=%u\n", flag, static_cast<unsigned>(value));
}

int osipf_MissionFlagGet(int flag)
{
	OsirisWrappers2Logf("osipf_MissionFlagGet flag=%d\n", flag);
	return 1;
}

void osipf_PlayerValue(int obj_handle, char op, char vhandle, void* ptr, int index)
{
	OsirisWrappers2Logf("osipf_PlayerValue obj_handle=%d op='%c' vhandle='%c' index=%d value=", obj_handle, op, vhandle, index);
	LogTypedValue(vhandle, ptr);
	OsirisWrappers2Logf("\n");
}

void osipf_ObjSetCustomAnim(int handle, float start, float end, float time, char flags, int sound_handle, char next_anim_type)
{
	OsirisWrappers2Logf(
		"osipf_ObjSetCustomAnim handle=%d start=%.3f end=%.3f time=%.3f flags='%c' sound_handle=%d next_anim_type='%c'\n",
		handle, start, end, time, flags, sound_handle, next_anim_type);
}

void osipf_PlayerAddHudMessage(int handle, char* str)
{
	OsirisWrappers2Logf("osipf_PlayerAddHudMessage handle=%d str=\"%s\"\n", handle, str ? str : "<null>");
}

void osipf_ObjGhost(int handle, bool f_ghost)
{
	OsirisWrappers2Logf("osipf_ObjGhost handle=%d f_ghost=%u\n", handle, f_ghost ? 1u : 0u);
}

void osipf_ObjBurning(int handle, float time, float damage_per_second)
{
	OsirisWrappers2Logf("osipf_ObjBurning handle=%d time=%.3f damage_per_second=%.3f\n", handle, time, damage_per_second);
}

bool osipf_ObjIsEffect(int handle, int type_flag)
{
	OsirisWrappers2Logf("osipf_ObjIsEffect handle=%d type_flag=%d\n", handle, type_flag);
	return (type_flag & 1) != 0;
}

void osipf_SoundStop(int s_handle, bool f_immediately)
{
	OsirisWrappers2Logf("osipf_SoundStop s_handle=%d f_immediately=%u\n", s_handle, f_immediately ? 1u : 0u);
}

int osipf_SoundPlay2d(int obj_handle, int s_id, float volume)
{
	OsirisWrappers2Logf("osipf_SoundPlay2d obj_handle=%d s_id=%d volume=%.3f\n", obj_handle, s_id, volume);
	return 321;
}

int osipf_SoundPlay3d(int obj_handle, int s_id, float volume)
{
	OsirisWrappers2Logf("osipf_SoundPlay3d obj_handle=%d s_id=%d volume=%.3f\n", obj_handle, s_id, volume);
	return 322;
}

int osipf_SoundFindId(char* s_name)
{
	OsirisWrappers2Logf("osipf_SoundFindId s_name=\"%s\"\n", s_name ? s_name : "<null>");
	return 33;
}

void osipf_AIGoalValue(int obj_handle, char g_index, char op, char vtype, void* ptr, char index)
{
	OsirisWrappers2Logf("osipf_AIGoalValue obj_handle=%d g_index=%d op='%c' vtype='%c' index=%d value=", obj_handle, static_cast<int>(g_index), op, vtype, static_cast<int>(index));
	LogTypedValue(vtype, ptr);
	OsirisWrappers2Logf("\n");
}

int osipf_AIGetNearbyObjs(vector* pos, int init_roomnum, float rad, int* object_handle_list, int max_elements, bool f_lightmap_only, bool f_only_players_and_ais, bool f_include_non_collide_objects, bool f_stop_at_closed_doors)
{
	OsirisWrappers2Logf(
		"osipf_AIGetNearbyObjs pos=");
	if (pos)
	{
		LogVector(*pos);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf(
		" init_roomnum=%d rad=%.3f max_elements=%d f_lightmap_only=%u f_only_players_and_ais=%u f_include_non_collide_objects=%u f_stop_at_closed_doors=%u handles=",
		init_roomnum, rad, max_elements,
		f_lightmap_only ? 1u : 0u, f_only_players_and_ais ? 1u : 0u,
		f_include_non_collide_objects ? 1u : 0u, f_stop_at_closed_doors ? 1u : 0u);
	LogVectorList(object_handle_list, max_elements);
	OsirisWrappers2Logf("\n");
	if (object_handle_list)
	{
		for (int i = 0; i < max_elements; ++i)
		{
			object_handle_list[i] = 100 + i;
		}
	}
	return 3;
}

void osipf_CinematicStop(void)
{
	OsirisWrappers2Logf("osipf_CinematicStop\n");
}

int osipf_GetTriggerRoom(int trigger_id)
{
	OsirisWrappers2Logf("osipf_GetTriggerRoom trigger_id=%d\n", trigger_id);
	return 808;
}

int osipf_GetTriggerFace(int trigger_id)
{
	OsirisWrappers2Logf("osipf_GetTriggerFace trigger_id=%d\n", trigger_id);
	return 909;
}

void osipf_GameCreateRandomSparks(int num_sparks, vector* pos, int roomnum, int which_index, float force_scalar)
{
	OsirisWrappers2Logf("osipf_GameCreateRandomSparks num_sparks=%d pos=", num_sparks);
	if (pos)
	{
		LogVector(*pos);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf(" roomnum=%d which_index=%d force_scalar=%.3f\n", roomnum, which_index, force_scalar);
}

void osipf_OsirisCancelTimerID(int id)
{
	OsirisWrappers2Logf("osipf_OsirisCancelTimerID id=%d\n", id);
}

void osipf_EnableShip(char* ship_name, bool enable)
{
	OsirisWrappers2Logf("osipf_EnableShip ship_name=\"%s\" enable=%u\n", ship_name ? ship_name : "<null>", enable ? 1u : 0u);
}

bool osipf_IsShipEnabled(char* ship_name)
{
	OsirisWrappers2Logf("osipf_IsShipEnabled ship_name=\"%s\"\n", ship_name ? ship_name : "<null>");
	return true;
}

bool osipf_PathGetInformation(int pathid, int point, vector* pos, int* room, matrix* orient)
{
	OsirisWrappers2Logf("osipf_PathGetInformation pathid=%d point=%d pos=", pathid, point);
	if (pos)
	{
		LogVector(*pos);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf(" room=");
	if (room)
	{
		OsirisWrappers2Logf("%d", *room);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf(" orient=");
	if (orient)
	{
		LogMatrix(*orient);
	}
	else
	{
		OsirisWrappers2Logf("<null>");
	}
	OsirisWrappers2Logf("\n");
	return true;
}

void osipf_LGoalValue(char op, char vtype, void* ptr, int g_index, int i_index)
{
	OsirisWrappers2Logf("osipf_LGoalValue op='%c' vtype='%c' g_index=%d i_index=%d value=", op, vtype, g_index, i_index);
	LogTypedValue(vtype, ptr);
	OsirisWrappers2Logf("\n");
}

void osipf_ObjKill(int handle, int killer_handle, float damage, int flags, float min_time, float max_time)
{
	OsirisWrappers2Logf(
		"osipf_ObjKill handle=%d killer_handle=%d damage=%.3f flags=%d min_time=%.3f max_time=%.3f\n",
		handle, killer_handle, damage, flags, min_time, max_time);
}

char osipf_GameGetDiffLevel(void)
{
	OsirisWrappers2Logf("osipf_GameGetDiffLevel\n");
	return 4;
}

int osipf_GetLanguageSetting(void)
{
	OsirisWrappers2Logf("osipf_GetLanguageSetting\n");
	return 2;
}

void osipf_PathValue(int path_id, int node_id, char op, int changes, void* ptr)
{
	OsirisWrappers2Logf("osipf_PathValue path_id=%d node_id=%d op='%c' changes=%d value=", path_id, node_id, op, changes);
	LogTypedValue('i', ptr);
	OsirisWrappers2Logf("\n");
}
