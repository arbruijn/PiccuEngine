/*
* Descent 3
* Copyright (C) 2024 Parallax Software
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "autopilot.h"

#include <math.h>
#include <string.h>

#include "controls.h"
#include "game.h"
#include "gamesequence.h"
#include "hud.h"
#include "levelgoal.h"
#include "object.h"
#include "player.h"
#include "powerup.h"
#include "room.h"
#include "terrain.h"
#include "trigger.h"
#include "weapon.h"
#include "doorway.h"
#include "aipath.h"
#include "AIMain.h"
#include "BOA.h"
#include "findintersection.h"
#include "args.h"

extern int AIAltPath[MAX_ROOMS];
extern int AIAltPathNumNodes;

namespace {

constexpr int AP_MAX_PATH_POINTS = 256;
constexpr float AP_GOAL_RESCAN_INTERVAL = 0.25f;
constexpr float AP_PATH_REBUILD_INTERVAL = 0.4f;
constexpr float AP_POWERUP_SCAN_RADIUS = 90.0f;
constexpr float AP_WEAPON_SCAN_RADIUS = 220.0f;
constexpr float AP_NODE_REACHED_DIST = 14.0f;
constexpr float AP_POWERUP_DETOUR_DIST = 140.0f;
constexpr float AP_TRIGGER_STANDOFF = 26.0f;
constexpr float AP_DOOR_STANDOFF = 36.0f;
constexpr float AP_ROBOT_STANDOFF = 55.0f;
constexpr float AP_SHOT_LOOKAHEAD = 1.1f;

enum ap_target_kind {
	APTK_NONE,
	APTK_GOAL,
	APTK_POWERUP
};

enum ap_shoot_kind {
	APSK_NONE,
	APSK_GOAL,
	APSK_ROBOT,
	APSK_DOOR
};

struct ap_target {
	ap_target_kind kind;
	int goal_index;
	int item_index;
	char item_type;
	int handle;
	int roomnum;
	int goal_flags;
	int trigger_activator;
	vector nav_pos;
	vector aim_pos;
};

struct ap_path_point {
	vector pos;
	int roomnum;
};

struct ap_shoot_target {
	ap_shoot_kind kind;
	int handle;
	vector aim_pos;
	float standoff_dist;
};

struct ap_state {
	bool active;
	bool command_line_enable_consumed;
	float next_goal_scan_time;
	float next_path_rebuild_time;
	ap_target target;
	int path_count;
	int path_index;
	ap_path_point path[AP_MAX_PATH_POINTS];
};

ap_state Autopilot_state = {};
char Autopilot_arg[] = "-autopilot";

float ap_min(float a, float b)
{
	return (a < b) ? a : b;
}

float ap_max(float a, float b)
{
	return (a > b) ? a : b;
}

float ap_clamp(float value, float min_value, float max_value)
{
	if (value < min_value)
		return min_value;
	if (value > max_value)
		return max_value;
	return value;
}

void ap_zero_controls(game_controls *controls)
{
	memset(controls, 0, sizeof(*controls));
}

bool ap_valid_room(int roomnum)
{
	if (ROOMNUM_OUTSIDE(roomnum))
	{
		int cell = CELLNUM(roomnum);
		return cell >= 0 && cell < TERRAIN_WIDTH * TERRAIN_DEPTH;
	}

	return roomnum >= 0 && roomnum <= Highest_room_index && Rooms[roomnum].used;
}

bool ap_valid_object(const object *obj)
{
	return obj != NULL && obj->type != OBJ_NONE && !(obj->flags & (OF_DEAD | OF_DYING)) && obj->render_type != RT_NONE;
}

void ap_reset_state()
{
	Autopilot_state.next_goal_scan_time = 0.0f;
	Autopilot_state.next_path_rebuild_time = 0.0f;
	Autopilot_state.target.kind = APTK_NONE;
	Autopilot_state.target.goal_index = -1;
	Autopilot_state.target.item_index = -1;
	Autopilot_state.target.item_type = -1;
	Autopilot_state.target.handle = OBJECT_HANDLE_NONE;
	Autopilot_state.target.roomnum = -1;
	Autopilot_state.target.goal_flags = 0;
	Autopilot_state.target.trigger_activator = 0;
	Autopilot_state.target.nav_pos = Zero_vector;
	Autopilot_state.target.aim_pos = Zero_vector;
	Autopilot_state.path_count = 0;
	Autopilot_state.path_index = 0;
}

float ap_distance_to_room(int roomnum)
{
	object *player_obj = Player_object;
	if (!player_obj || !ap_valid_room(player_obj->roomnum) || !ap_valid_room(roomnum))
		return 0.0f;

	if (BOA_INDEX(player_obj->roomnum) == BOA_INDEX(roomnum))
		return 0.0f;

	float path_dist = 0.0f;
	if (!AIFindAltPath(player_obj, player_obj->roomnum, roomnum, &path_dist))
		return 9999999.0f;

	return path_dist;
}

bool ap_same_target(const ap_target &lhs, const ap_target &rhs)
{
	return lhs.kind == rhs.kind &&
		lhs.goal_index == rhs.goal_index &&
		lhs.item_index == rhs.item_index &&
		lhs.item_type == rhs.item_type &&
		lhs.handle == rhs.handle &&
		lhs.roomnum == rhs.roomnum;
}

void ap_append_path_point(const vector &pos, int roomnum)
{
	if (Autopilot_state.path_count >= AP_MAX_PATH_POINTS)
		return;

	if (!ap_valid_room(roomnum))
		return;

	if (Autopilot_state.path_count > 0)
	{
		vector delta = pos - Autopilot_state.path[Autopilot_state.path_count - 1].pos;
		if (vm_GetMagnitudeFast(&delta) < 3.0f)
			return;
	}

	Autopilot_state.path[Autopilot_state.path_count].pos = pos;
	Autopilot_state.path[Autopilot_state.path_count].roomnum = roomnum;
	Autopilot_state.path_count++;
}

void ap_compute_trigger_target(int trigger_index, ap_target *target)
{
	ASSERT(trigger_index >= 0 && trigger_index < Num_triggers);

	trigger *tp = &Triggers[trigger_index];
	room *rp = &Rooms[tp->roomnum];
	face *fp = &rp->faces[tp->facenum];

	vector face_center = Zero_vector;
	ComputeCenterPointOnFace(&face_center, rp, tp->facenum);

	vector room_dir = rp->path_pnt - face_center;
	if (vm_GetMagnitudeFast(&room_dir) < 1.0f)
	{
		room_dir = rp->faces[tp->facenum].normal;
	}
	if (vm_GetMagnitudeFast(&room_dir) < 1.0f)
	{
		room_dir = Player_object ? Player_object->orient.fvec : Identity_matrix.fvec;
	}
	vm_NormalizeVector(&room_dir);

	target->trigger_activator = tp->activator;
	target->aim_pos = face_center;

	if (tp->activator & AF_PLAYER_WEAPON)
	{
		target->roomnum = tp->roomnum;
		target->nav_pos = face_center + room_dir * AP_TRIGGER_STANDOFF;
	}
	else if (tp->activator & AF_PLAYER)
	{
		if (fp->portal_num != -1)
		{
			int next_room = rp->portals[fp->portal_num].croom;
			if (ap_valid_room(next_room))
			{
				vector portal_dir = Rooms[next_room].path_pnt - face_center;
				if (vm_GetMagnitudeFast(&portal_dir) < 1.0f)
					portal_dir = -room_dir;
				vm_NormalizeVector(&portal_dir);
				target->roomnum = next_room;
				target->nav_pos = face_center + portal_dir * 10.0f;
				return;
			}
		}

		target->roomnum = tp->roomnum;
		target->nav_pos = face_center + room_dir * 6.0f;
	}
	else
	{
		target->roomnum = tp->roomnum;
		target->nav_pos = face_center + room_dir * AP_TRIGGER_STANDOFF;
	}
}

bool ap_build_goal_item_target(int goal_index, int item_index, ap_target *target)
{
	bool done = false;
	int handle = -1;
	char item_type = -1;
	int goal_flags = 0;

	if (!Level_goals.GoalStatus(goal_index, LO_GET_SPECIFIED, &goal_flags))
		return false;

	if (!Level_goals.GoalItemInfo(goal_index, item_index, LO_GET_SPECIFIED, &item_type, &handle, &done) || done)
		return false;

	if (goal_flags & (LGF_COMPLETED | LGF_FAILED))
		return false;

	memset(target, 0, sizeof(*target));
	target->kind = APTK_GOAL;
	target->goal_index = goal_index;
	target->item_index = item_index;
	target->item_type = item_type;
	target->handle = handle;
	target->goal_flags = goal_flags;
	target->trigger_activator = 0;

	switch (item_type)
	{
	case LIT_OBJECT:
	{
		object *obj = ObjGet(handle);
		if (!ap_valid_object(obj))
			return false;

		target->roomnum = obj->roomnum;
		target->nav_pos = obj->pos;
		target->aim_pos = obj->pos;
		return true;
	}

	case LIT_INTERNAL_ROOM:
		if (!ap_valid_room(handle) || ROOMNUM_OUTSIDE(handle))
			return false;

		target->roomnum = handle;
		target->nav_pos = Rooms[handle].path_pnt;
		target->aim_pos = target->nav_pos;
		return true;

	case LIT_TRIGGER:
		if (handle < 0 || handle >= Num_triggers)
			return false;
		ap_compute_trigger_target(handle, target);
		return ap_valid_room(target->roomnum);

	case LIT_TERRAIN_CELL:
		if (handle < 0 || handle >= TERRAIN_WIDTH * TERRAIN_DEPTH)
			return false;
		target->roomnum = MAKE_ROOMNUM(handle);
		ComputeTerrainSegmentCenter(&target->nav_pos, handle);
		target->aim_pos = target->nav_pos;
		return true;

	case LIT_ANY_MINE:
	default:
		return false;
	}
}

bool ap_goal_requires_weapon(const ap_target &target)
{
	if (target.item_type == LIT_TRIGGER)
		return (target.trigger_activator & AF_PLAYER_WEAPON) != 0;

	return (target.goal_flags & (LGF_COMP_DESTROY | LGF_COMP_PLAYER_WEAPON)) != 0;
}

bool ap_goal_requires_touch(const ap_target &target)
{
	if (target.item_type == LIT_TRIGGER)
		return (target.trigger_activator & AF_PLAYER) != 0;

	return (target.goal_flags & (LGF_COMP_PLAYER | LGF_COMP_ENTER | LGF_COMP_ACTIVATE)) != 0;
}

bool ap_resolve_goal_from_list(int goal_index, ap_target *best_target)
{
	float best_dist = 9999999.0f;
	bool found = false;

	int num_items = Level_goals.GoalGetNumItems(goal_index);
	for (int item_index = 0; item_index < num_items; item_index++)
	{
		ap_target candidate = {};
		if (!ap_build_goal_item_target(goal_index, item_index, &candidate))
			continue;

		float path_dist = ap_distance_to_room(candidate.roomnum);
		if (path_dist >= 9999999.0f)
			continue;

		float world_dist = 0.0f;
		if (Player_object && BOA_INDEX(candidate.roomnum) == BOA_INDEX(Player_object->roomnum))
			world_dist = vm_VectorDistance(&Player_object->pos, &candidate.nav_pos);

		float score = path_dist + world_dist;
		if (!found || score < best_dist)
		{
			best_dist = score;
			*best_target = candidate;
			found = true;
		}
	}

	return found;
}

bool ap_find_goal_target(ap_target *target)
{
	int num_primary = Level_goals.GetNumActivePrimaryGoals();
	for (int i = 0; i < num_primary; i++)
	{
		if (ap_resolve_goal_from_list(Level_goals.GetActivePrimaryGoal(i), target))
			return true;
	}

	int num_secondary = Level_goals.GetNumActiveSecondaryGoals();
	for (int i = 0; i < num_secondary; i++)
	{
		if (ap_resolve_goal_from_list(Level_goals.GetActiveSecondaryGoal(i), target))
			return true;
	}

	return false;
}

bool ap_find_powerup_target(ap_target *target)
{
	if (!Player_object || !ap_valid_room(Player_object->roomnum))
		return false;

	short nearby[64];
	int num_nearby = fvi_QuickDistObjectList(&Player_object->pos, Player_object->roomnum, AP_POWERUP_SCAN_RADIUS, nearby, 64, false, false, false, true);
	int best_index = -1;
	float best_dist = AP_POWERUP_DETOUR_DIST;

	for (int i = 0; i < num_nearby; i++)
	{
		object *obj = &Objects[nearby[i]];
		if (obj->type != OBJ_POWERUP || !ap_valid_object(obj))
			continue;

		float dist = vm_VectorDistanceQuick(&Player_object->pos, &obj->pos);
		if (dist < best_dist)
		{
			best_dist = dist;
			best_index = nearby[i];
		}
	}

	if (best_index < 0)
		return false;

	memset(target, 0, sizeof(*target));
	target->kind = APTK_POWERUP;
	target->goal_index = -1;
	target->item_index = -1;
	target->item_type = LIT_OBJECT;
	target->handle = Objects[best_index].handle;
	target->roomnum = Objects[best_index].roomnum;
	target->goal_flags = LGF_COMP_PLAYER;
	target->trigger_activator = 0;
	target->nav_pos = Objects[best_index].pos;
	target->aim_pos = Objects[best_index].pos;
	return true;
}

bool ap_trace_line(const vector &from, int start_room, const vector &to, int ignore_objnum, fvi_info *hit_info)
{
	fvi_query fq;
	fq.p0 = const_cast<vector *>(&from);
	fq.startroom = start_room;
	fq.p1 = const_cast<vector *>(&to);
	fq.rad = ap_max(Player_object ? (Player_object->size - 0.1f) : 0.0f, 0.1f);
	fq.thisobjnum = OBJNUM(Player_object);
	fq.ignore_obj_list = NULL;
	fq.flags = FQ_CHECK_OBJS | FQ_NO_RELINK | FQ_IGNORE_WEAPONS | FQ_IGNORE_POWERUPS;

	int fate = fvi_FindIntersection(&fq, hit_info);
	return fate == HIT_NONE || ((fate == HIT_OBJECT || fate == HIT_SPHERE_2_POLY_OBJECT) && hit_info->hit_object[0] == ignore_objnum);
}

bool ap_should_use_alt_path(const vector &start_pos, int start_room, const vector &end_pos, int end_room, int ignore_objnum, bool *bline_ok)
{
	*bline_ok = false;

	if (BOA_GetNextRoom(start_room, end_room) == BOA_NO_PATH)
		return false;

	fvi_info hit_info;
	if (ap_trace_line(start_pos, start_room, end_pos, ignore_objnum, &hit_info))
	{
		*bline_ok = true;
		return false;
	}

	if (BOA_HasPossibleBlockage(start_room, end_room))
	{
		int next_room = BOA_INDEX(start_room);
		do
		{
			if (next_room != BOA_INDEX(start_room) && BOA_LockedDoor(Player_object, next_room))
				return true;

			int last_room = next_room;
			next_room = BOA_GetNextRoom(next_room, end_room);

			if (BOA_INDEX(last_room) != BOA_INDEX(next_room) && next_room != BOA_NO_PATH)
			{
				if (BOA_DetermineStartRoomPortal(last_room, NULL, next_room, NULL) == -1)
					return true;
			}
		} while (BOA_INDEX(next_room) != BOA_INDEX(start_room) && BOA_INDEX(next_room) != BOA_INDEX(end_room) && next_room != BOA_NO_PATH);
	}

	return false;
}

void ap_generate_alt_boa_path()
{
	for (int x = 0; x < AIAltPathNumNodes - 1; x++)
	{
		int cur_room = AIAltPath[x];
		int next_room = AIAltPath[x + 1];

		if (next_room >= 0 && next_room <= Highest_room_index)
		{
			vector *pos = NULL;
			int portal_index = -1;

			if (cur_room > Highest_room_index)
			{
				portal_index = BOA_DetermineStartRoomPortal(next_room, NULL, cur_room, NULL, false, false, NULL);
				if (portal_index >= 0)
					pos = &Rooms[next_room].portals[portal_index].path_pnt;
			}
			else
			{
				for (portal_index = 0; portal_index < Rooms[next_room].num_portals; portal_index++)
				{
					if (Rooms[next_room].portals[portal_index].croom == cur_room)
					{
						pos = &Rooms[next_room].portals[portal_index].path_pnt;
						break;
					}
				}
			}

			if (pos)
				ap_append_path_point(*pos, next_room);

			ap_append_path_point(Rooms[next_room].path_pnt, next_room);
		}
		else if (BOA_INDEX(cur_room) <= Highest_room_index && cur_room != next_room)
		{
			int portal_index = BOA_DetermineStartRoomPortal(cur_room, NULL, next_room, NULL);
			if (portal_index >= 0)
				ap_append_path_point(Rooms[cur_room].portals[portal_index].path_pnt, cur_room);
		}
	}
}

bool ap_generate_boa_path(int start_room, int end_room)
{
	int next_room = start_room;

	if (!ROOMNUM_OUTSIDE(next_room))
		ap_append_path_point(Rooms[start_room].path_pnt, start_room);

	if (ROOMNUM_OUTSIDE(next_room) && ROOMNUM_OUTSIDE(end_room) &&
		(TERRAIN_REGION(next_room) == TERRAIN_REGION(end_room) || TERRAIN_REGION(end_room) == 0 || TERRAIN_REGION(next_room) == 0))
	{
		next_room = end_room;
	}

	while (BOA_INDEX(next_room) != BOA_GetNextRoom(next_room, end_room) && next_room != BOA_NO_PATH)
	{
		int cur_room = next_room;
		next_room = BOA_GetNextRoom(next_room, end_room);

		if (next_room != cur_room && next_room != BOA_NO_PATH && BOA_INDEX(next_room) <= Highest_room_index)
		{
			vector *pos = NULL;
			int portal_index;

			for (portal_index = 0; portal_index < Rooms[next_room].num_portals; portal_index++)
			{
				if (Rooms[next_room].portals[portal_index].croom == cur_room)
				{
					pos = &Rooms[next_room].portals[portal_index].path_pnt;
					break;
				}

				if (Rooms[Rooms[next_room].portals[portal_index].croom].flags & RF_EXTERNAL)
				{
					int cell = GetTerrainCellFromPos(&Rooms[next_room].portals[portal_index].path_pnt);
					int region = TERRAIN_REGION(cell);
					if (Highest_room_index + region + 1 == BOA_INDEX(cur_room))
					{
						pos = &Rooms[next_room].portals[portal_index].path_pnt;
						break;
					}
				}
			}

			if (!pos)
				return false;

			ap_append_path_point(*pos, next_room);
			ap_append_path_point(Rooms[next_room].path_pnt, next_room);
		}
		else
		{
			if (next_room == BOA_NO_PATH)
				return false;

			if (BOA_INDEX(cur_room) <= Highest_room_index && cur_room != next_room)
			{
				int portal_index = BOA_DetermineStartRoomPortal(cur_room, NULL, next_room, NULL);
				if (portal_index >= 0)
					ap_append_path_point(Rooms[cur_room].portals[portal_index].path_pnt, cur_room);
			}
		}
	}

	return true;
}

bool ap_rebuild_path()
{
	if (!Player_object || !ap_valid_room(Player_object->roomnum) || !ap_valid_room(Autopilot_state.target.roomnum))
		return false;

	Autopilot_state.path_count = 0;
	Autopilot_state.path_index = 0;

	if (BOA_INDEX(Player_object->roomnum) == BOA_INDEX(Autopilot_state.target.roomnum))
	{
		ap_append_path_point(Autopilot_state.target.nav_pos, Autopilot_state.target.roomnum);
		return Autopilot_state.path_count > 0;
	}

	bool bline_ok = false;
	int ignore_objnum = -1;
	if (Autopilot_state.target.kind == APTK_GOAL && Autopilot_state.target.item_type == LIT_OBJECT)
	{
		object *goal_obj = ObjGet(Autopilot_state.target.handle);
		if (goal_obj)
			ignore_objnum = OBJNUM(goal_obj);
	}

	bool use_alt = ap_should_use_alt_path(Player_object->pos, Player_object->roomnum, Autopilot_state.target.nav_pos, Autopilot_state.target.roomnum, ignore_objnum, &bline_ok);

	if (bline_ok)
	{
		ap_append_path_point(Autopilot_state.target.nav_pos, Autopilot_state.target.roomnum);
	}
	else if (use_alt)
	{
		if (!AIFindAltPath(Player_object, Player_object->roomnum, Autopilot_state.target.roomnum))
			return false;
		ap_generate_alt_boa_path();
		ap_append_path_point(Autopilot_state.target.nav_pos, Autopilot_state.target.roomnum);
	}
	else
	{
		if (!ap_generate_boa_path(Player_object->roomnum, Autopilot_state.target.roomnum))
		{
			if (!AIFindAltPath(Player_object, Player_object->roomnum, Autopilot_state.target.roomnum))
				return false;
			ap_generate_alt_boa_path();
		}
		ap_append_path_point(Autopilot_state.target.nav_pos, Autopilot_state.target.roomnum);
	}

	while (Autopilot_state.path_index < Autopilot_state.path_count)
	{
		vector delta = Autopilot_state.path[Autopilot_state.path_index].pos - Player_object->pos;
		if (vm_GetMagnitudeFast(&delta) >= AP_NODE_REACHED_DIST)
			break;
		Autopilot_state.path_index++;
	}

	if (Autopilot_state.path_index >= Autopilot_state.path_count)
	{
		Autopilot_state.path_count = 0;
		Autopilot_state.path_index = 0;
		return false;
	}

	return true;
}

bool ap_resolve_target()
{
	ap_target goal_target = {};
	if (!ap_find_goal_target(&goal_target))
		return false;

	ap_target final_target = goal_target;
	ap_target powerup_target = {};
	if (ap_find_powerup_target(&powerup_target))
	{
		final_target = powerup_target;
	}

	bool target_changed = !ap_same_target(final_target, Autopilot_state.target);
	if (target_changed)
	{
		Autopilot_state.target = final_target;
		Autopilot_state.next_path_rebuild_time = 0.0f;
	}

	if (Autopilot_state.path_count == 0 || target_changed || Gametime >= Autopilot_state.next_path_rebuild_time)
	{
		if (!ap_rebuild_path())
			return false;
		Autopilot_state.next_path_rebuild_time = Gametime + AP_PATH_REBUILD_INTERVAL;
	}

	return true;
}

bool ap_goal_target_visible(const ap_target &target)
{
	fvi_query fq;
	fvi_info hit_info;

	fq.p0 = &Player_object->pos;
	fq.startroom = Player_object->roomnum;
	fq.p1 = const_cast<vector *>(&target.aim_pos);
	fq.rad = 0.0f;
	fq.thisobjnum = OBJNUM(Player_object);
	fq.ignore_obj_list = NULL;
	fq.flags = FQ_CHECK_OBJS | FQ_NO_RELINK | FQ_IGNORE_WEAPONS | FQ_IGNORE_POWERUPS;

	int fate = fvi_FindIntersection(&fq, &hit_info);

	if (target.item_type == LIT_TRIGGER)
	{
		return fate == HIT_WALL &&
			hit_info.hit_face_room[0] == Triggers[target.handle].roomnum &&
			hit_info.hit_face[0] == Triggers[target.handle].facenum;
	}

	if (target.item_type == LIT_OBJECT)
	{
		object *goal_obj = ObjGet(target.handle);
		return goal_obj && (fate == HIT_OBJECT || fate == HIT_SPHERE_2_POLY_OBJECT) && hit_info.hit_object[0] == OBJNUM(goal_obj);
	}

	return fate == HIT_NONE;
}

bool ap_find_path_obstacle(const vector &nav_pos, ap_shoot_target *shoot_target)
{
	fvi_query fq;
	fvi_info hit_info;

	fq.p0 = &Player_object->pos;
	fq.startroom = Player_object->roomnum;
	fq.p1 = const_cast<vector *>(&nav_pos);
	fq.rad = ap_max(Player_object->size - 0.1f, 0.1f);
	fq.thisobjnum = OBJNUM(Player_object);
	fq.ignore_obj_list = NULL;
	fq.flags = FQ_CHECK_OBJS | FQ_NO_RELINK | FQ_IGNORE_WEAPONS | FQ_IGNORE_POWERUPS;

	int fate = fvi_FindIntersection(&fq, &hit_info);
	if (fate != HIT_OBJECT && fate != HIT_SPHERE_2_POLY_OBJECT)
		return false;

	object *hit_obj = &Objects[hit_info.hit_object[0]];

	if (hit_obj->type == OBJ_DOOR && DoorwayGetPosition(hit_obj->handle) < 0.95f)
	{
		shoot_target->kind = APSK_DOOR;
		shoot_target->handle = hit_obj->handle;
		shoot_target->aim_pos = hit_obj->pos;
		shoot_target->standoff_dist = AP_DOOR_STANDOFF;
		return true;
	}

	if (hit_obj->type == OBJ_ROBOT || (hit_obj->type == OBJ_BUILDING && hit_obj->ai_info))
	{
		shoot_target->kind = APSK_ROBOT;
		shoot_target->handle = hit_obj->handle;
		shoot_target->aim_pos = hit_obj->pos;
		shoot_target->standoff_dist = AP_ROBOT_STANDOFF;
		return true;
	}

	return false;
}

bool ap_get_shoot_target(const vector &nav_pos, ap_shoot_target *shoot_target)
{
	memset(shoot_target, 0, sizeof(*shoot_target));

	if (ap_goal_requires_weapon(Autopilot_state.target) && ap_goal_target_visible(Autopilot_state.target))
	{
		shoot_target->kind = APSK_GOAL;
		shoot_target->handle = Autopilot_state.target.handle;
		shoot_target->aim_pos = Autopilot_state.target.aim_pos;
		shoot_target->standoff_dist = AP_TRIGGER_STANDOFF;

		if (Autopilot_state.target.item_type == LIT_OBJECT)
			shoot_target->standoff_dist = AP_ROBOT_STANDOFF;

		return true;
	}

	return ap_find_path_obstacle(nav_pos, shoot_target);
}

bool ap_compute_evade_dir(vector *evade_dir)
{
	short nearby[64];
	int num_nearby = fvi_QuickDistObjectList(&Player_object->pos, Player_object->roomnum, AP_WEAPON_SCAN_RADIUS, nearby, 64, false, false, false, true);
	bool found = false;
	*evade_dir = Zero_vector;

	for (int i = 0; i < num_nearby; i++)
	{
		object *weapon_obj = &Objects[nearby[i]];
		if (weapon_obj->type != OBJ_WEAPON || (weapon_obj->flags & (OF_DEAD | OF_DYING)))
			continue;

		object *parent_obj = ObjGetUltimateParent(ObjGet(weapon_obj->parent_handle));
		if (!parent_obj || parent_obj == Player_object || !AIObjEnemy(Player_object, parent_obj))
			continue;

		vector rel_vel = weapon_obj->mtype.phys_info.velocity - Player_object->mtype.phys_info.velocity;
		float rel_speed_sq = rel_vel * rel_vel;
		if (rel_speed_sq < 1.0f)
			continue;

		vector to_player = Player_object->pos - weapon_obj->pos;
		if ((to_player * rel_vel) <= 0.0f)
			continue;

		float time_to_closest = (to_player * rel_vel) / rel_speed_sq;
		time_to_closest = ap_clamp(time_to_closest, 0.0f, AP_SHOT_LOOKAHEAD);

		vector closest_offset = to_player - rel_vel * time_to_closest;
		float closest_dist = vm_GetMagnitude(&closest_offset);
		float safe_dist = Player_object->size + weapon_obj->size + 10.0f;

		if (closest_dist > safe_dist)
			continue;

		vector dodge = closest_offset;
		if (vm_GetMagnitudeFast(&dodge) < 1.0f)
		{
			dodge = weapon_obj->orient.rvec;
			if (vm_GetMagnitudeFast(&dodge) < 1.0f)
				dodge = Player_object->orient.rvec;
		}

		vm_NormalizeVector(&dodge);
		dodge *= ap_max(0.4f, (safe_dist - closest_dist) / safe_dist);
		*evade_dir += dodge;
		found = true;
	}

	if (found && vm_GetMagnitudeFast(evade_dir) > 0.001f)
		vm_NormalizeVector(evade_dir);

	return found;
}

void ap_advance_path()
{
	while (Autopilot_state.path_index < Autopilot_state.path_count)
	{
		vector delta = Autopilot_state.path[Autopilot_state.path_index].pos - Player_object->pos;
		if (vm_GetMagnitudeFast(&delta) >= AP_NODE_REACHED_DIST)
			break;
		Autopilot_state.path_index++;
	}
}

vector ap_current_nav_pos()
{
	if (Autopilot_state.path_index < Autopilot_state.path_count)
		return Autopilot_state.path[Autopilot_state.path_index].pos;

	return Autopilot_state.target.nav_pos;
}

void ap_apply_aim_controls(const vector &aim_dir, game_controls *controls)
{
	vector local;
	local.x = aim_dir * Player_object->orient.rvec;
	local.y = aim_dir * Player_object->orient.uvec;
	local.z = aim_dir * Player_object->orient.fvec;

	float heading = atan2f(local.x, ap_max(local.z, 0.05f));
	float pitch = -atan2f(local.y, ap_max(local.z, 0.05f));

	if (local.z < 0.0f)
	{
		heading = (local.x >= 0.0f) ? 1.0f : -1.0f;
		pitch = (local.y >= 0.0f) ? -0.75f : 0.75f;
	}

	controls->heading_thrust = ap_clamp(heading * 2.2f, -1.0f, 1.0f);
	controls->pitch_thrust = ap_clamp(pitch * 2.2f, -0.75f, 0.75f);
	controls->bank_thrust = 0.0f;
}

void ap_apply_move_controls(const vector &move_dir, float move_scale, game_controls *controls)
{
	vector local;
	local.x = move_dir * Player_object->orient.rvec;
	local.y = move_dir * Player_object->orient.uvec;
	local.z = move_dir * Player_object->orient.fvec;

	controls->sideways_thrust = ap_clamp(local.x * move_scale * 1.3f, -1.0f, 1.0f);
	controls->vertical_thrust = ap_clamp(local.y * move_scale * 1.3f, -1.0f, 1.0f);
	controls->forward_thrust = ap_clamp(local.z * move_scale, -0.5f, 1.0f);
}

void ap_update_controls(game_controls *controls)
{
	ap_zero_controls(controls);

	if (!Player_object || Player_object->type != OBJ_PLAYER)
		return;

	ap_advance_path();

	vector nav_pos = ap_current_nav_pos();
	vector to_nav = nav_pos - Player_object->pos;
	float nav_dist = vm_GetMagnitude(&to_nav);
	if (nav_dist > 0.01f)
		to_nav /= nav_dist;
	else
		to_nav = Player_object->orient.fvec;

	vector evade_dir = Zero_vector;
	bool dodging = ap_compute_evade_dir(&evade_dir);

	ap_shoot_target shoot_target = {};
	bool has_shoot_target = ap_get_shoot_target(nav_pos, &shoot_target);

	vector desired_move = to_nav;
	if (dodging)
	{
		desired_move += evade_dir * 1.6f;
		if (vm_GetMagnitudeFast(&desired_move) > 0.001f)
			vm_NormalizeVector(&desired_move);
	}

	float move_scale = 1.0f;
	vector aim_dir = to_nav;

	if (has_shoot_target)
	{
		vector to_shoot = shoot_target.aim_pos - Player_object->pos;
		float shoot_dist = vm_GetMagnitude(&to_shoot);
		if (shoot_dist > 0.01f)
			aim_dir = to_shoot / shoot_dist;

		float standoff = shoot_target.standoff_dist;
		if (shoot_dist < standoff * 0.65f)
		{
			desired_move = -aim_dir;
			move_scale = 0.5f;
		}
		else if (shoot_dist < standoff)
		{
			desired_move = dodging ? evade_dir : Zero_vector;
			move_scale = dodging ? 0.9f : 0.0f;
		}
		else
		{
			desired_move = to_nav;
			if (dodging)
			{
				desired_move += evade_dir * 1.4f;
				vm_NormalizeVector(&desired_move);
			}
			move_scale = 0.6f;
		}

		float aim_dot = Player_object->orient.fvec * aim_dir;
		if (aim_dot > 0.985f)
		{
			controls->fire_primary_down_state = true;
			controls->fire_primary_down_count = 1;
			controls->fire_primary_down_time = Frametime;
		}
	}
	else
	{
		if (nav_dist < 30.0f)
			move_scale = ap_clamp(nav_dist / 30.0f, 0.2f, 1.0f);

		vector target_dir = Autopilot_state.target.nav_pos - Player_object->pos;
		if (vm_GetMagnitudeFast(&target_dir) > 0.01f)
		{
			vm_NormalizeVector(&target_dir);
			aim_dir = target_dir;
		}
	}

	ap_apply_aim_controls(aim_dir, controls);
	ap_apply_move_controls(desired_move, move_scale, controls);

	if (!has_shoot_target && nav_dist > 140.0f && (Player_object->orient.fvec * desired_move) > 0.9f)
		controls->afterburn_thrust = 1.0f;
}

bool ap_can_run()
{
	if (GetGameState() != GAMESTATE_LVLPLAYING)
		return false;

	if (GetScreenMode() != SM_GAME)
		return false;

	if (Game_mode & GM_MULTI)
		return false;

	if (Game_interface_mode != GAME_INTERFACE)
		return false;

	if (!Player_object || Player_object->type != OBJ_PLAYER)
		return false;

	if (Players[Player_num].flags & (PLAYER_FLAGS_DYING | PLAYER_FLAGS_DEAD))
		return false;

	if (!ap_valid_room(Player_object->roomnum))
		return false;

	return true;
}

void ap_set_active_internal(bool active, bool announce)
{
	if (active == Autopilot_state.active)
		return;

	Autopilot_state.active = active;
	ap_reset_state();

	if (!announce)
		return;

	if (active)
		AddHUDMessage("Autopilot testing enabled.");
	else
		AddHUDMessage("Autopilot testing disabled.");
}

}

bool AutopilotIsActive()
{
	return Autopilot_state.active;
}

void AutopilotSetActive(bool active)
{
	if (active && Game_mode & GM_MULTI)
	{
		AddHUDMessage("Autopilot is single-player only.");
		return;
	}

	ap_set_active_internal(active, true);
}

void AutopilotToggle()
{
	AutopilotSetActive(!Autopilot_state.active);
}

void AutopilotReset()
{
	ap_set_active_internal(false, false);
}

void AutopilotApply(game_controls *controls)
{
	if (!Autopilot_state.command_line_enable_consumed && FindArg(Autopilot_arg))
	{
		Autopilot_state.command_line_enable_consumed = true;
		AutopilotSetActive(true);
	}

	if (!Autopilot_state.active)
		return;

	if (!ap_can_run())
	{
		AutopilotSetActive(false);
		return;
	}

	if (Gametime >= Autopilot_state.next_goal_scan_time || Autopilot_state.target.kind == APTK_NONE)
	{
		if (!ap_resolve_target())
		{
			AddHUDMessage("Autopilot has no reachable active goal.");
			AutopilotSetActive(false);
			return;
		}

		Autopilot_state.next_goal_scan_time = Gametime + AP_GOAL_RESCAN_INTERVAL;
	}

	ap_update_controls(controls);
}
