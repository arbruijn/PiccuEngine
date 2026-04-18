#include "osiris_event_test.h"

#include "mono.h"

#ifdef OSIRIS_EVENT_USE_LOGGER_CALLBACK
extern "C" void OSIRIS_CALLEVENT_CDECL OsirisEventTestLog(const char* format, ...);
#undef mprintf
#define mprintf(args) OsirisEventTestLog args
#endif

namespace
{
	const char* GetOsirisEventName(int event)
	{
		switch (event)
		{
		case EVT_INTERVAL: return "EVT_INTERVAL";
		case EVT_AI_FRAME: return "EVT_AI_FRAME";
		case EVT_DAMAGED: return "EVT_DAMAGED";
		case EVT_COLLIDE: return "EVT_COLLIDE";
		case EVT_CREATED: return "EVT_CREATED";
		case EVT_DESTROY: return "EVT_DESTROY";
		case EVT_TIMER: return "EVT_TIMER";
		case EVT_USE: return "EVT_USE";
		case EVT_AI_NOTIFY: return "EVT_AI_NOTIFY";
		case EVT_AI_INIT: return "EVT_AI_INIT";
		case EVT_CHANGESEG: return "EVT_CHANGESEG";
		case EVT_SAVESTATE: return "EVT_SAVESTATE";
		case EVT_RESTORESTATE: return "EVT_RESTORESTATE";
		case EVT_MEMRESTORE: return "EVT_MEMRESTORE";
		case EVT_TIMERCANCEL: return "EVT_TIMERCANCEL";
		case EVT_AIN_OBJKILLED: return "EVT_AIN_OBJKILLED";
		case EVT_AIN_SEEPLAYER: return "EVT_AIN_SEEPLAYER";
		case EVT_AIN_WHITOBJECT: return "EVT_AIN_WHITOBJECT";
		case EVT_AIN_GOALCOMPLETE: return "EVT_AIN_GOALCOMPLETE";
		case EVT_AIN_GOALFAIL: return "EVT_AIN_GOALFAIL";
		case EVT_AIN_MELEE_HIT: return "EVT_AIN_MELEE_HIT";
		case EVT_AIN_MELEE_ATTACK_FRAME: return "EVT_AIN_MELEE_ATTACK_FRAME";
		case EVT_AIN_MOVIE_START: return "EVT_AIN_MOVIE_START";
		case EVT_AIN_MOVIE_END: return "EVT_AIN_MOVIE_END";
		case EVT_MATCEN_CREATE: return "EVT_MATCEN_CREATE";
		case EVT_DOOR_ACTIVATE: return "EVT_DOOR_ACTIVATE";
		case EVT_DOOR_CLOSE: return "EVT_DOOR_CLOSE";
		case EVT_CHILD_DIED: return "EVT_CHILD_DIED";
		case EVT_LEVEL_GOAL_COMPLETE: return "EVT_LEVEL_GOAL_COMPLETE";
		case EVT_ALL_LEVEL_GOALS_COMPLETE: return "EVT_ALL_LEVEL_GOALS_COMPLETE";
		case EVT_LEVEL_GOAL_ITEM_COMPLETE: return "EVT_LEVEL_GOAL_ITEM_COMPLETE";
		case EVT_PLAYER_MOVIE_START: return "EVT_PLAYER_MOVIE_START";
		case EVT_PLAYER_MOVIE_END: return "EVT_PLAYER_MOVIE_END";
		case EVT_PLAYER_RESPAWN: return "EVT_PLAYER_RESPAWN";
		case EVT_PLAYER_DIES: return "EVT_PLAYER_DIES";
		default: return "UNKNOWN_EVT";
		}
	}
}

short OSIRIS_CALLEVENT_CDECL CallInstanceEvent(int id, void* ptr, int event, tOSIRISEventInfo* data)
{
	(void)id;
	(void)ptr;
	OsirisEventCall_Test(event, data);
	return CONTINUE_CHAIN | CONTINUE_DEFAULT;
}

void OsirisEventCall_Test(int event, tOSIRISEventInfo* data)
{
	if (!data)
	{
		mprintf((0, "OsirisEventCall_Test: event=%d data=<null>\n", event));
		return;
	}

	switch (event)
	{
	case EVT_INTERVAL:
		mprintf((0, "%s type=%d me_handle=%d frame_time=%.3f game_time=%.3f\n",
			GetOsirisEventName(event), event, data->me_handle,
			data->evt_interval.frame_time, data->evt_interval.game_time));
		break;
	case EVT_AI_FRAME:
		mprintf((0, "%s type=%d me_handle=%d\n", GetOsirisEventName(event), event, data->me_handle));
		break;
	case EVT_DAMAGED:
		mprintf((0, "%s type=%d me_handle=%d damage=%.3f it_handle=%d weapon_handle=%d damage_type=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_damaged.damage,
			data->evt_damaged.it_handle, data->evt_damaged.weapon_handle, data->evt_damaged.damage_type));
		break;
	case EVT_COLLIDE:
		mprintf((0, "%s type=%d me_handle=%d it_handle=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_collide.it_handle));
		break;
	case EVT_CREATED:
		mprintf((0, "%s type=%d me_handle=%d\n", GetOsirisEventName(event), event, data->me_handle));
		break;
	case EVT_DESTROY:
		mprintf((0, "%s type=%d me_handle=%d is_dying=%u\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_destroy.is_dying));
		break;
	case EVT_TIMER:
		mprintf((0, "%s type=%d me_handle=%d id=%d game_time=%.3f\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_timer.id, data->evt_timer.game_time));
		break;
	case EVT_USE:
		mprintf((0, "%s type=%d me_handle=%d it_handle=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_use.it_handle));
		break;
	case EVT_AI_NOTIFY:
		mprintf((0, "%s type=%d me_handle=%d notify_type=%d it_handle=%d goal_num=%d goal_uid=%d enabler_num=%d attack_num=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_ai_notify.notify_type,
			data->evt_ai_notify.it_handle, data->evt_ai_notify.goal_num, data->evt_ai_notify.goal_uid,
			data->evt_ai_notify.enabler_num, data->evt_ai_notify.attack_num));
		break;
	case EVT_AI_INIT:
		mprintf((0, "%s type=%d me_handle=%d\n", GetOsirisEventName(event), event, data->me_handle));
		break;
	case EVT_CHANGESEG:
		mprintf((0, "%s type=%d me_handle=%d room_num=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_changeseg.room_num));
		break;
	case EVT_SAVESTATE:
		mprintf((0, "%s type=%d me_handle=%d fileptr=%p\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_savestate.fileptr));
		break;
	case EVT_RESTORESTATE:
		mprintf((0, "%s type=%d me_handle=%d fileptr=%p\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_restorestate.fileptr));
		break;
	case EVT_MEMRESTORE:
		mprintf((0, "%s type=%d me_handle=%d id=%d memory_ptr=%p\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_memrestore.id, data->evt_memrestore.memory_ptr));
		break;
	case EVT_TIMERCANCEL:
		mprintf((0, "%s type=%d me_handle=%d handle=%d detonated=%u\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_timercancel.handle, data->evt_timercancel.detonated));
		break;
	case EVT_AIN_OBJKILLED:
	case EVT_AIN_SEEPLAYER:
	case EVT_AIN_WHITOBJECT:
	case EVT_AIN_GOALCOMPLETE:
	case EVT_AIN_GOALFAIL:
	case EVT_AIN_MELEE_HIT:
	case EVT_AIN_MELEE_ATTACK_FRAME:
	case EVT_AIN_MOVIE_START:
	case EVT_AIN_MOVIE_END:
		mprintf((0, "%s type=%d me_handle=%d notify_type=%d it_handle=%d goal_num=%d goal_uid=%d enabler_num=%d attack_num=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_ai_notify.notify_type,
			data->evt_ai_notify.it_handle, data->evt_ai_notify.goal_num, data->evt_ai_notify.goal_uid,
			data->evt_ai_notify.enabler_num, data->evt_ai_notify.attack_num));
		break;
	case EVT_MATCEN_CREATE:
		mprintf((0, "%s type=%d me_handle=%d it_handle=%d id=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_matcen_create.it_handle, data->evt_matcen_create.id));
		break;
	case EVT_DOOR_ACTIVATE:
	case EVT_DOOR_CLOSE:
		mprintf((0, "%s type=%d me_handle=%d\n", GetOsirisEventName(event), event, data->me_handle));
		break;
	case EVT_CHILD_DIED:
		mprintf((0, "%s type=%d me_handle=%d it_handle=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_child_died.it_handle));
		break;
	case EVT_LEVEL_GOAL_COMPLETE:
		mprintf((0, "%s type=%d me_handle=%d level_goal_index=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_level_goal_complete.level_goal_index));
		break;
	case EVT_ALL_LEVEL_GOALS_COMPLETE:
		mprintf((0, "%s type=%d me_handle=%d\n", GetOsirisEventName(event), event, data->me_handle));
		break;
	case EVT_LEVEL_GOAL_ITEM_COMPLETE:
		mprintf((0, "%s type=%d me_handle=%d level_goal_index=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_level_goal_item_complete.level_goal_index));
		break;
	case EVT_PLAYER_MOVIE_START:
	case EVT_PLAYER_MOVIE_END:
		mprintf((0, "%s type=%d me_handle=%d\n", GetOsirisEventName(event), event, data->me_handle));
		break;
	case EVT_PLAYER_RESPAWN:
		mprintf((0, "%s type=%d me_handle=%d it_handle=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_player_respawn.it_handle));
		break;
	case EVT_PLAYER_DIES:
		mprintf((0, "%s type=%d me_handle=%d it_handle=%d\n",
			GetOsirisEventName(event), event, data->me_handle, data->evt_player_dies.it_handle));
		break;
	default:
		mprintf((0, "UNKNOWN_EVT type=%d me_handle=%d\n", event, data->me_handle));
		break;
	}
}
