#include "msafe_callfunction_test.h"

#include "pserror.h"

#ifndef MSAFE_CALLFUNCTION_IS_POINTER
void MSAFE_CALLFUNCTION_CDECL msafe_CallFunction(ubyte type, msafe_struct* mstruct)
{
	msafe_CallFunction_Test(type, mstruct);
}
#endif

void msafe_CallFunction_Test(ubyte type, msafe_struct* mstruct)
{
	if (!mstruct)
	{
		mprintf((0, "msafe_CallFunction_Test: type=%u mstruct=<null>\n", type));
		return;
	}

	switch (type)
	{
	case MSAFE_WEATHER_RAIN:
		mprintf((0, "MSAFE_WEATHER_RAIN type=%u state=%u scalar=%.3f\n", type, mstruct->state, mstruct->scalar));
		break;
	case MSAFE_WEATHER_SNOW:
		mprintf((0, "MSAFE_WEATHER_SNOW type=%u state=%u scalar=%.3f\n", type, mstruct->state, mstruct->scalar));
		break;
	case MSAFE_WEATHER_LIGHTNING:
		mprintf((0, "MSAFE_WEATHER_LIGHTNING type=%u state=%u scalar=%.3f randval=%d\n", type, mstruct->state, mstruct->scalar, mstruct->randval));
		break;
	case MSAFE_WEATHER_LIGHTNING_BOLT:
		mprintf((0, "MSAFE_WEATHER_LIGHTNING_BOLT type=%u texnum=%d flags=%d objhandle=%u ithandle=%u roomnum=%d "
			"pos=(%.3f,%.3f,%.3f) pos2=(%.3f,%.3f,%.3f) lifetime=%.3f color=%d size=%.3f state=%u count=%d interval=%.3f index=%d g1=%.3f g2=%.3f\n",
			type, mstruct->texnum, mstruct->flags, mstruct->objhandle, mstruct->ithandle, mstruct->roomnum,
			XYZ(&mstruct->pos), XYZ(&mstruct->pos2), mstruct->lifetime, mstruct->color, mstruct->size, mstruct->state,
			mstruct->count, mstruct->interval, mstruct->index, mstruct->g1, mstruct->g2));
		break;
	case MSAFE_ROOM_TEXTURE:
		mprintf((0, "MSAFE_ROOM_TEXTURE type=%u roomnum=%d facenum=%d index=%d\n", type, mstruct->roomnum, mstruct->facenum, mstruct->index));
		break;
	case MSAFE_ROOM_WIND:
		mprintf((0, "MSAFE_ROOM_WIND type=%u roomnum=%d wind=(%.3f,%.3f,%.3f)\n", type, mstruct->roomnum, XYZ(&mstruct->wind)));
		break;
	case MSAFE_ROOM_FOG:
		mprintf((0, "MSAFE_ROOM_FOG type=%u roomnum=%d fog_r=%.3f fog_g=%.3f fog_b=%.3f fog_depth=%.3f\n",
			type, mstruct->roomnum, mstruct->fog_r, mstruct->fog_g, mstruct->fog_b, mstruct->fog_depth));
		break;
	case MSAFE_ROOM_FOG_STATE:
		mprintf((0, "MSAFE_ROOM_FOG_STATE type=%u roomnum=%d state=%u\n", type, mstruct->roomnum, mstruct->state));
		break;
	case MSAFE_ROOM_LIGHT_PULSE:
		mprintf((0, "MSAFE_ROOM_LIGHT_PULSE type=%u roomnum=%d pulse_time=%u pulse_offset=%u\n",
			type, mstruct->roomnum, mstruct->pulse_time, mstruct->pulse_offset));
		break;
	case MSAFE_ROOM_LIGHT_FLICKER:
		mprintf((0, "MSAFE_ROOM_LIGHT_FLICKER type=%u roomnum=%d state=%u\n", type, mstruct->roomnum, mstruct->state));
		break;
	case MSAFE_ROOM_LIGHT_STROBE:
		mprintf((0, "MSAFE_ROOM_LIGHT_STROBE type=%u roomnum=%d state=%u\n", type, mstruct->roomnum, mstruct->state));
		break;
	case MSAFE_ROOM_REFUEL:
		mprintf((0, "MSAFE_ROOM_REFUEL type=%u roomnum=%d state=%u\n", type, mstruct->roomnum, mstruct->state));
		break;
	case MSAFE_ROOM_CHANGING_FOG:
		mprintf((0, "MSAFE_ROOM_CHANGING_FOG type=%u roomnum=%d fog_r=%.3f fog_g=%.3f fog_b=%.3f fog_depth=%.3f interval=%.3f\n",
			type, mstruct->roomnum, mstruct->fog_r, mstruct->fog_g, mstruct->fog_b, mstruct->fog_depth, mstruct->interval));
		break;
	case MSAFE_ROOM_CHANGING_WIND:
		mprintf((0, "MSAFE_ROOM_CHANGING_WIND type=%u roomnum=%d wind=(%.3f,%.3f,%.3f) interval=%.3f\n",
			type, mstruct->roomnum, XYZ(&mstruct->wind), mstruct->interval));
		break;
	case MSAFE_ROOM_PORTAL_RENDER:
		mprintf((0, "MSAFE_ROOM_PORTAL_RENDER type=%u roomnum=%d portalnum=%d state=%u flags=%d\n",
			type, mstruct->roomnum, mstruct->portalnum, mstruct->state, mstruct->flags));
		break;
	case MSAFE_ROOM_PORTAL_BLOCK:
		mprintf((0, "MSAFE_ROOM_PORTAL_BLOCK type=%u roomnum=%d portalnum=%d state=%u\n",
			type, mstruct->roomnum, mstruct->portalnum, mstruct->state));
		break;
	case MSAFE_ROOM_BREAK_GLASS:
		mprintf((0, "MSAFE_ROOM_BREAK_GLASS type=%u roomnum=%d portalnum=%d\n", type, mstruct->roomnum, mstruct->portalnum));
		break;
	case MSAFE_ROOM_DAMAGE:
		mprintf((0, "MSAFE_ROOM_DAMAGE type=%u roomnum=%d amount=%.3f index=%d\n", type, mstruct->roomnum, mstruct->amount, mstruct->index));
		break;
	case MSAFE_ROOM_REMOVE_ALL_POWERUPS:
		mprintf((0, "MSAFE_ROOM_REMOVE_ALL_POWERUPS type=%u roomnum=%d\n", type, mstruct->roomnum));
		break;
	case MSAFE_OBJECT_FIRE_WEAPON:
		mprintf((0, "MSAFE_OBJECT_FIRE_WEAPON type=%u objhandle=%u index=%d gunpoint=%d\n", type, mstruct->objhandle, mstruct->index, mstruct->gunpoint));
		break;
	case MSAFE_OBJECT_SETONFIRE:
		mprintf((0, "MSAFE_OBJECT_SETONFIRE type=%u objhandle=%u longevity=%.3f interval=%.3f ithandle=%u\n",
			type, mstruct->objhandle, mstruct->longevity, mstruct->interval, mstruct->ithandle));
		break;
	case MSAFE_OBJECT_SHIELDS:
		mprintf((0, "MSAFE_OBJECT_SHIELDS type=%u objhandle=%u shields=%.3f\n", type, mstruct->objhandle, mstruct->shields));
		break;
	case MSAFE_OBJECT_ENERGY:
		mprintf((0, "MSAFE_OBJECT_ENERGY type=%u objhandle=%u energy=%.3f\n", type, mstruct->objhandle, mstruct->energy));
		break;
	case MSAFE_SHOW_ENABLED_CONTROLS:
		mprintf((0, "MSAFE_SHOW_ENABLED_CONTROLS type=%u objhandle=%u state=%u\n", type, mstruct->objhandle, mstruct->state));
		break;
	case MSAFE_OBJECT_PLAYER_CMASK:
		mprintf((0, "MSAFE_OBJECT_PLAYER_CMASK type=%u objhandle=%u control_mask=%u control_val=%u\n",
			type, mstruct->objhandle, mstruct->control_mask, mstruct->control_val));
		break;
	case MSAFE_OBJECT_PLAYER_KEY:
		mprintf((0, "MSAFE_OBJECT_PLAYER_KEY type=%u ithandle=%u index=%d objhandle=%u message=\"%s\"\n",
			type, mstruct->ithandle, mstruct->index, mstruct->objhandle, mstruct->message));
		break;
	case MSAFE_OBJECT_MOVEMENT_SCALAR:
		mprintf((0, "MSAFE_OBJECT_MOVEMENT_SCALAR type=%u objhandle=%u scalar=%.3f\n", type, mstruct->objhandle, mstruct->scalar));
		break;
	case MSAFE_OBJECT_RECHARGE_SCALAR:
		mprintf((0, "MSAFE_OBJECT_RECHARGE_SCALAR type=%u objhandle=%u scalar=%.3f\n", type, mstruct->objhandle, mstruct->scalar));
		break;
	case MSAFE_OBJECT_WSPEED_SCALAR:
		mprintf((0, "MSAFE_OBJECT_WSPEED_SCALAR type=%u objhandle=%u scalar=%.3f\n", type, mstruct->objhandle, mstruct->scalar));
		break;
	case MSAFE_OBJECT_ARMOR_SCALAR:
		mprintf((0, "MSAFE_OBJECT_ARMOR_SCALAR type=%u objhandle=%u scalar=%.3f\n", type, mstruct->objhandle, mstruct->scalar));
		break;
	case MSAFE_OBJECT_DAMAGE_SCALAR:
		mprintf((0, "MSAFE_OBJECT_DAMAGE_SCALAR type=%u objhandle=%u scalar=%.3f\n", type, mstruct->objhandle, mstruct->scalar));
		break;
	case MSAFE_OBJECT_ADD_WEAPON:
		mprintf((0, "MSAFE_OBJECT_ADD_WEAPON type=%u objhandle=%u index=%d ammo=%.3f\n", type, mstruct->objhandle, mstruct->index, mstruct->ammo));
		break;
	case MSAFE_OBJECT_DAMAGE_OBJECT:
		mprintf((0, "MSAFE_OBJECT_DAMAGE_OBJECT type=%u objhandle=%u killer_handle=%u damage_type=%u amount=%.3f\n",
			type, mstruct->objhandle, mstruct->killer_handle, mstruct->damage_type, mstruct->amount));
		break;
	case MSAFE_OBJECT_START_SPEW:
		mprintf((0, "MSAFE_OBJECT_START_SPEW type=%u objhandle=%u gunpoint=%d random=%u is_real=%u effect_type=%u phys_info=%u "
			"drag=%.3f mass=%.3f interval=%.3f longevity=%.3f lifetime=%.3f size=%.3f speed=%.3f id=%d\n",
			type, mstruct->objhandle, mstruct->gunpoint, mstruct->random, mstruct->is_real, mstruct->effect_type, mstruct->phys_info,
			mstruct->drag, mstruct->mass, mstruct->interval, mstruct->longevity, mstruct->lifetime, mstruct->size, mstruct->speed, mstruct->id));
		break;
	case MSAFE_OBJECT_STOP_SPEW:
		mprintf((0, "MSAFE_OBJECT_STOP_SPEW type=%u id=%d\n", type, mstruct->id));
		break;
	case MSAFE_OBJECT_NO_RENDER:
		mprintf((0, "MSAFE_OBJECT_NO_RENDER type=%u objhandle=%u\n", type, mstruct->objhandle));
		break;
	case MSAFE_OBJECT_GHOST:
		mprintf((0, "MSAFE_OBJECT_GHOST type=%u objhandle=%u\n", type, mstruct->objhandle));
		break;
	case MSAFE_OBJECT_UNGHOST:
		mprintf((0, "MSAFE_OBJECT_UNGHOST type=%u objhandle=%u\n", type, mstruct->objhandle));
		break;
	case MSAFE_OBJECT_REMOVE:
		mprintf((0, "MSAFE_OBJECT_REMOVE type=%u objhandle=%u playsound=%u\n", type, mstruct->objhandle, mstruct->playsound));
		break;
	case MSAFE_OBJECT_INVULNERABLE:
		mprintf((0, "MSAFE_OBJECT_INVULNERABLE type=%u objhandle=%u state=%u lifetime=%.3f\n", type, mstruct->objhandle, mstruct->state, mstruct->lifetime));
		break;
	case MSAFE_OBJECT_CLOAKALLPLAYERS:
		mprintf((0, "MSAFE_OBJECT_CLOAKALLPLAYERS type=%u state=%u lifetime=%.3f\n", type, mstruct->state, mstruct->lifetime));
		break;
	case MSAFE_OBJECT_CLOAK:
		mprintf((0, "MSAFE_OBJECT_CLOAK type=%u objhandle=%u state=%u lifetime=%.3f\n", type, mstruct->objhandle, mstruct->state, mstruct->lifetime));
		break;
	case MSAFE_OBJECT_LIGHT_DIST:
		mprintf((0, "MSAFE_OBJECT_LIGHT_DIST type=%u objhandle=%u light_distance=%.3f\n", type, mstruct->objhandle, mstruct->light_distance));
		break;
	case MSAFE_OBJECT_LIGHT_COLOR:
		mprintf((0, "MSAFE_OBJECT_LIGHT_COLOR type=%u objhandle=%u r1=%.3f g1=%.3f b1=%.3f\n", type, mstruct->objhandle, mstruct->r1, mstruct->g1, mstruct->b1));
		break;
	case MSAFE_OBJECT_DEFORM:
		mprintf((0, "MSAFE_OBJECT_DEFORM type=%u objhandle=%u amount=%.3f lifetime=%.3f\n", type, mstruct->objhandle, mstruct->amount, mstruct->lifetime));
		break;
	case MSAFE_OBJECT_SPARKS:
		mprintf((0, "MSAFE_OBJECT_SPARKS type=%u objhandle=%u amount=%.3f lifetime=%.3f\n", type, mstruct->objhandle, mstruct->amount, mstruct->lifetime));
		break;
	case MSAFE_OBJECT_VIEWER_SHAKE:
		mprintf((0, "MSAFE_OBJECT_VIEWER_SHAKE type=%u amount=%.3f\n", type, mstruct->amount));
		break;
	case MSAFE_OBJECT_SHAKE_AREA:
		mprintf((0, "MSAFE_OBJECT_SHAKE_AREA type=%u objhandle=%u scalar=%.3f amount=%.3f\n", type, mstruct->objhandle, mstruct->scalar, mstruct->amount));
		break;
	case MSAFE_OBJECT_WORLD_POSITION:
		mprintf((0, "MSAFE_OBJECT_WORLD_POSITION type=%u objhandle=%u roomnum=%d pos=(%.3f,%.3f,%.3f) "
			"orient_rvec=(%.3f,%.3f,%.3f) orient_uvec=(%.3f,%.3f,%.3f) orient_fvec=(%.3f,%.3f,%.3f)\n",
			type, mstruct->objhandle, mstruct->roomnum, XYZ(&mstruct->pos),
			XYZ(&mstruct->orient.rvec), XYZ(&mstruct->orient.uvec), XYZ(&mstruct->orient.fvec)));
		break;
	case MSAFE_OBJECT_PLAYER_CONTROLAI:
		mprintf((0, "MSAFE_OBJECT_PLAYER_CONTROLAI type=%u slot=%d state=%u\n", type, mstruct->slot, mstruct->state));
		break;
	case MSAFE_OBJECT_DESTROY_ROBOTS_EXCEPT:
		mprintf((0, "MSAFE_OBJECT_DESTROY_ROBOTS_EXCEPT type=%u list=%p count=%d\n", type, mstruct->list, mstruct->count));
		break;
	case MSAFE_SOUND_STREAMING:
		mprintf((0, "MSAFE_SOUND_STREAMING type=%u state=%u objhandle=%u name=\"%s\" volume=%.3f flags=%d sound_handle=%d slot=%d\n",
			type, mstruct->state, mstruct->objhandle, mstruct->name, mstruct->volume, mstruct->flags, mstruct->sound_handle, mstruct->slot));
		break;
	case MSAFE_SOUND_2D:
		mprintf((0, "MSAFE_SOUND_2D type=%u state=%u objhandle=%u index=%d volume=%.3f sound_handle=%d slot=%d\n",
			type, mstruct->state, mstruct->objhandle, mstruct->index, mstruct->volume, mstruct->sound_handle, mstruct->slot));
		break;
	case MSAFE_SOUND_OBJECT:
		mprintf((0, "MSAFE_SOUND_OBJECT type=%u objhandle=%u index=%d sound_handle=%d\n", type, mstruct->objhandle, mstruct->index, mstruct->sound_handle));
		break;
	case MSAFE_SOUND_STOP:
		mprintf((0, "MSAFE_SOUND_STOP type=%u\n", type));
		break;
	case MSAFE_SOUND_STOP_OBJ:
		mprintf((0, "MSAFE_SOUND_STOP_OBJ type=%u objhandle=%u\n", type, mstruct->objhandle));
		break;
	case MSAFE_SOUND_VOLUME_OBJ:
		mprintf((0, "MSAFE_SOUND_VOLUME_OBJ type=%u objhandle=%u volume=%.3f\n", type, mstruct->objhandle, mstruct->volume));
		break;
	case MSAFE_MUSIC_REGION:
		mprintf((0, "MSAFE_MUSIC_REGION type=%u state=%u objhandle=%u index=%d slot=%d\n", type, mstruct->state, mstruct->objhandle, mstruct->index, mstruct->slot));
		break;
	case MSAFE_MISC_LEVELGOAL:
		mprintf((0, "MSAFE_MISC_LEVELGOAL type=%u index=%d message=\"%s\" value_type=%d count=%d\n", type, mstruct->index, mstruct->message, mstruct->type, mstruct->count));
		break;
	case MSAFE_MISC_WAYPOINT:
		mprintf((0, "MSAFE_MISC_WAYPOINT type=%u index=%d\n", type, mstruct->index));
		break;
	case MSAFE_MISC_ENABLE_SHIP:
		mprintf((0, "MSAFE_MISC_ENABLE_SHIP type=%u name=\"%s\" state=%u\n", type, mstruct->name, mstruct->state));
		break;
	case MSAFE_MISC_FILTERED_HUD_MESSAGE:
		mprintf((0, "MSAFE_MISC_FILTERED_HUD_MESSAGE type=%u state=%u objhandle=%u message=\"%s\" color=%d slot=%d\n",
			type, mstruct->state, mstruct->objhandle, mstruct->message, mstruct->color, mstruct->slot));
		break;
	case MSAFE_MISC_HUD_MESSAGE:
		mprintf((0, "MSAFE_MISC_HUD_MESSAGE type=%u state=%u objhandle=%u message=\"%s\" color=%d slot=%d\n",
			type, mstruct->state, mstruct->objhandle, mstruct->message, mstruct->color, mstruct->slot));
		break;
	case MSAFE_MISC_GAME_MESSAGE:
		mprintf((0, "MSAFE_MISC_GAME_MESSAGE type=%u state=%u objhandle=%u message=\"%s\" message2=\"%s\" color=%d slot=%d\n",
			type, mstruct->state, mstruct->objhandle, mstruct->message, mstruct->message2, mstruct->color, mstruct->slot));
		break;
	case MSAFE_MISC_END_LEVEL:
		mprintf((0, "MSAFE_MISC_END_LEVEL type=%u state=%u\n", type, mstruct->state));
		break;
	case MSAFE_MISC_POPUP_CAMERA:
		mprintf((0, "MSAFE_MISC_POPUP_CAMERA type=%u objhandle=%u interval=%.3f scalar=%.3f gunpoint=%d\n",
			type, mstruct->objhandle, mstruct->interval, mstruct->scalar, mstruct->gunpoint));
		break;
	case MSAFE_MISC_CLOSE_POPUP:
		mprintf((0, "MSAFE_MISC_CLOSE_POPUP type=%u\n", type));
		break;
	case MSAFE_MISC_GUIDEBOT_NAME:
		mprintf((0, "MSAFE_MISC_GUIDEBOT_NAME type=%u objhandle=%u name=\"%s\"\n", type, mstruct->objhandle, mstruct->name));
		break;
	case MSAFE_MISC_START_TIMER:
		mprintf((0, "MSAFE_MISC_START_TIMER type=%u index=%d color=%d\n", type, mstruct->index, mstruct->color));
		break;
	case MSAFE_MISC_UPDATE_HUD_ITEM:
		mprintf((0, "MSAFE_MISC_UPDATE_HUD_ITEM type=%u message=\"%s\" color=%d\n", type, mstruct->message, mstruct->color));
		break;
	case MSAFE_DOOR_LOCK_STATE:
		mprintf((0, "MSAFE_DOOR_LOCK_STATE type=%u objhandle=%u state=%u\n", type, mstruct->objhandle, mstruct->state));
		break;
	case MSAFE_DOOR_ACTIVATE:
		mprintf((0, "MSAFE_DOOR_ACTIVATE type=%u objhandle=%u\n", type, mstruct->objhandle));
		break;
	case MSAFE_DOOR_STOP:
		mprintf((0, "MSAFE_DOOR_STOP type=%u objhandle=%u\n", type, mstruct->objhandle));
		break;
	case MSAFE_DOOR_POSITION:
		mprintf((0, "MSAFE_DOOR_POSITION type=%u objhandle=%u scalar=%.3f\n", type, mstruct->objhandle, mstruct->scalar));
		break;
	case MSAFE_TRIGGER_SET:
		mprintf((0, "MSAFE_TRIGGER_SET type=%u trigger_num=%u state=%u\n", type, mstruct->trigger_num, mstruct->state));
		break;
	case MSAFE_INVEN_ADD_OBJECT:
		mprintf((0, "MSAFE_INVEN_ADD_OBJECT type=%u objhandle=%u ithandle=%u flags=%d message=\"%s\"\n",
			type, mstruct->objhandle, mstruct->ithandle, mstruct->flags, mstruct->message));
		break;
	case MSAFE_INVEN_REMOVE_OBJECT:
		mprintf((0, "MSAFE_INVEN_REMOVE_OBJECT type=%u objhandle=%u ithandle=%u\n", type, mstruct->objhandle, mstruct->ithandle));
		break;
	case MSAFE_INVEN_ADD_TYPE_ID:
		mprintf((0, "MSAFE_INVEN_ADD_TYPE_ID type=%u objhandle=%u item_type=%d id=%d flags=%d\n",
			type, mstruct->objhandle, mstruct->type, mstruct->id, mstruct->flags));
		break;
	case MSAFE_INVEN_REMOVE:
		mprintf((0, "MSAFE_INVEN_REMOVE type=%u objhandle=%u item_type=%d id=%d\n", type, mstruct->objhandle, mstruct->type, mstruct->id));
		break;
	case MSAFE_COUNTERMEASURE_REMOVE:
		mprintf((0, "MSAFE_COUNTERMEASURE_REMOVE type=%u objhandle=%u item_type=%d id=%d\n", type, mstruct->objhandle, mstruct->type, mstruct->id));
		break;
	case MSAFE_COUNTERMEASURE_ADD:
		mprintf((0, "MSAFE_COUNTERMEASURE_ADD type=%u objhandle=%u count=%d name=\"%s\" aux_type=%d aux_id=%d\n",
			type, mstruct->objhandle, mstruct->count, mstruct->name, mstruct->aux_type, mstruct->aux_id));
		break;
	case MSAFE_OBJECT_ROTDRAG:
		mprintf((0, "MSAFE_OBJECT_ROTDRAG type=%u objhandle=%u rot_drag=%.3f\n", type, mstruct->objhandle, mstruct->rot_drag));
		break;
	case MSAFE_OBJECT_TYPE:
		mprintf((0, "MSAFE_OBJECT_TYPE type=%u objhandle=%u object_type=%d\n", type, mstruct->objhandle, mstruct->type));
		break;
	case MSAFE_OBJECT_ID:
		mprintf((0, "MSAFE_OBJECT_ID type=%u objhandle=%u id=%d\n", type, mstruct->objhandle, mstruct->id));
		break;
	case MSAFE_OBJECT_CONTROL_TYPE:
		mprintf((0, "MSAFE_OBJECT_CONTROL_TYPE type=%u objhandle=%u control_type=%d\n", type, mstruct->objhandle, mstruct->control_type));
		break;
	case MSAFE_OBJECT_FLAGS:
		mprintf((0, "MSAFE_OBJECT_FLAGS type=%u objhandle=%u flags=%d\n", type, mstruct->objhandle, mstruct->flags));
		break;
	case MSAFE_OBJECT_MOVEMENT_TYPE:
		mprintf((0, "MSAFE_OBJECT_MOVEMENT_TYPE type=%u objhandle=%u movement_type=%d\n", type, mstruct->objhandle, mstruct->movement_type));
		break;
	case MSAFE_OBJECT_CREATION_TIME:
		mprintf((0, "MSAFE_OBJECT_CREATION_TIME type=%u objhandle=%u creation_time=%.3f\n", type, mstruct->objhandle, mstruct->creation_time));
		break;
	case MSAFE_OBJECT_PHYSICS_FLAGS:
		mprintf((0, "MSAFE_OBJECT_PHYSICS_FLAGS type=%u objhandle=%u physics_flags=%d\n", type, mstruct->objhandle, mstruct->physics_flags));
		break;
	case MSAFE_OBJECT_PARENT:
		mprintf((0, "MSAFE_OBJECT_PARENT type=%u objhandle=%u ithandle=%u\n", type, mstruct->objhandle, mstruct->ithandle));
		break;
	case MSAFE_WEAPON_CHECK:
		mprintf((0, "MSAFE_WEAPON_CHECK type=%u\n", type));
		break;
	case MSAFE_WEAPON_ADD:
		mprintf((0, "MSAFE_WEAPON_ADD type=%u objhandle=%u index=%d state=%u count=%d\n",
			type, mstruct->objhandle, mstruct->index, mstruct->state, mstruct->count));
		break;
	default:
		mprintf((0, "UNKNOWN_MSAFE_CALL type=%u\n", type));
		break;
	}
}
