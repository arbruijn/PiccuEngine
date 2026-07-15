#include "msafe.h"
#include "msafe_struct32.h"

#include <cstring>

namespace emuabi
{
namespace
{
	static void decode_vector(vector& dst, const vector32& src)
	{
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}

	static void decode_matrix(matrix& dst, const matrix32& src)
	{
		decode_vector(dst.rvec, src.rvec);
		decode_vector(dst.uvec, src.uvec);
		decode_vector(dst.fvec, src.fvec);
	}

	static void encode_vector(vector32& dst, const vector& src)
	{
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}

	static void encode_matrix(matrix32& dst, const matrix& src)
	{
		encode_vector(dst.rvec, src.rvec);
		encode_vector(dst.uvec, src.uvec);
		encode_vector(dst.fvec, src.fvec);
	}

#define MSAFE_STRUCT_FIELDS(APPLY) \
	APPLY(I32, roomnum) \
	APPLY(I16, facenum) \
	APPLY(I16, texnum) \
	APPLY(I16, portalnum) \
	APPLY(F32, fog_r) \
	APPLY(F32, fog_g) \
	APPLY(F32, fog_b) \
	APPLY(F32, fog_depth) \
	APPLY(VECTOR, wind) \
	APPLY(U8, pulse_time) \
	APPLY(U8, pulse_offset) \
	APPLY(U32, objhandle) \
	APPLY(U32, ithandle) \
	APPLY(F32, shields) \
	APPLY(F32, energy) \
	APPLY(I16, start_tick) \
	APPLY(I16, end_tick) \
	APPLY(F32, cycle_time) \
	APPLY(I32, type) \
	APPLY(I32, id) \
	APPLY(I32, aux_type) \
	APPLY(I32, aux_id) \
	APPLY(U32, checksum) \
	APPLY(I32, path_id) \
	APPLY(F32, amount) \
	APPLY(U8, damage_type) \
	APPLY(U32, killer_handle) \
	APPLY(F32, ammo) \
	APPLY(U8, playsound) \
	APPLY(U8, remove) \
	APPLY(U8, do_powerup) \
	APPLY(VECTOR, velocity) \
	APPLY(VECTOR, rot_velocity) \
	APPLY(F32, rot_drag) \
	APPLY(VECTOR, thrust) \
	APPLY(VECTOR, rot_thrust) \
	APPLY(I8, control_type) \
	APPLY(I8, movement_type) \
	APPLY(F32, creation_time) \
	APPLY(I32, physics_flags) \
	APPLY(VECTOR, pos) \
	APPLY(MATRIX, orient) \
	APPLY(F32, anim_frame) \
	APPLY(U8, is_real) \
	APPLY(U8, random) \
	APPLY(U8, unused2) \
	APPLY(I8, gunpoint) \
	APPLY(U8, effect_type) \
	APPLY(U8, phys_info) \
	APPLY(F32, drag) \
	APPLY(F32, mass) \
	APPLY(U16, unused) \
	APPLY(I32, randval) \
	APPLY(U8, trigger_num) \
	APPLY(I32, sound_handle) \
	APPLY(F32, volume) \
	APPLY(I32, index) \
	APPLY(F32, scalar) \
	APPLY(F32, interval) \
	APPLY(U8, state) \
	APPLY(I8, slot) \
	APPLY(CHAR_ARRAY, message, MSAFE_MESSAGE_LENGTH) \
	APPLY(CHAR_ARRAY, name, MSAFE_NAME_LENGTH) \
	APPLY(I32, color) \
	APPLY(F32, longevity) \
	APPLY(F32, lifetime) \
	APPLY(F32, size) \
	APPLY(F32, speed) \
	APPLY(I32, count) \
	APPLY(I32, flags) \
	APPLY(PTR32, list) \
	APPLY(U32, control_mask) \
	APPLY(U8, control_val) \
	APPLY(F32, light_distance) \
	APPLY(F32, r1) \
	APPLY(F32, g1) \
	APPLY(F32, b1) \
	APPLY(F32, r2) \
	APPLY(F32, g2) \
	APPLY(F32, b2) \
	APPLY(F32, time_interval) \
	APPLY(F32, flicker_distance) \
	APPLY(F32, directional_dot) \
	APPLY(I32, timebits) \
	APPLY(VECTOR, pos2) \
	APPLY(CHAR_ARRAY, message2, MSAFE_MESSAGE_LENGTH)

	static void decode_getvalue_fields(int type, const msafe_struct32& src, msafe_struct& dst)
	{
		switch (type)
		{
		case MSAFE_SHOW_ENABLED_CONTROLS:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_PLAYER_CMASK:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_POS:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ORIENT:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ROOMNUM:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_WORLD_POSITION:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_VELOCITY:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ROTVELOCITY:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_THRUST:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ROTTHRUST:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_FLAGS:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_SIZE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_CONTROL_TYPE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_MOVEMENT_TYPE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_CREATION_TIME:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_PHYSICS_FLAGS:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ROTDRAG:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_SHIELDS:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_SHIELDS_ORIGINAL:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ENERGY:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_MOVEMENT_SCALAR:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_RECHARGE_SCALAR:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_WSPEED_SCALAR:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ARMOR_SCALAR:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_DAMAGE_SCALAR:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_TYPE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ID:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_PARENT:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ENERGY_WEAPON:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_DAMAGE_AMOUNT:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_COUNT_TYPE:
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_OBJECT_INVULNERABLE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_CLOAK:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_LIGHT_DIST:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_PLAYER_HANDLE:
			dst.slot = src.slot;
			break;
		case MSAFE_OBJECT_PLAYER_CONTROLAI:
			dst.slot = src.slot;
			break;
		case MSAFE_DOOR_LOCK_STATE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_DOOR_OPENABLE:
			dst.objhandle = src.objhandle;
			dst.ithandle = src.ithandle;
			break;
		case MSAFE_DOOR_POSITION:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_TRIGGER_SET:
			dst.trigger_num = src.trigger_num;
			break;
		case MSAFE_ROOM_HAS_PLAYER:
			dst.roomnum = src.roomnum;
			break;
		case MSAFE_ROOM_PORTAL_RENDER:
			dst.roomnum = src.roomnum;
			dst.portalnum = src.portalnum;
			break;
		case MSAFE_ROOM_PORTAL_BLOCK:
			dst.roomnum = src.roomnum;
			dst.portalnum = src.portalnum;
			break;
		case MSAFE_ROOM_DAMAGE:
			dst.roomnum = src.roomnum;
			break;
		case MSAFE_MISC_ENABLE_SHIP:
			memcpy(dst.name, src.name, MSAFE_NAME_LENGTH);
			break;
		case MSAFE_MISC_WAYPOINT:
			break;
		case MSAFE_MISC_GUIDEBOT_NAME:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_INVEN_CHECK:
		case MSAFE_COUNTERMEASURE_CHECK:
			dst.objhandle = src.objhandle;
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_INVEN_COUNT:
			dst.objhandle = src.objhandle;
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_INVEN_SIZE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_INVEN_GET_TYPE_ID:
			dst.objhandle = src.objhandle;
			dst.index = src.index;
			break;
		case MSAFE_INVEN_CHECK_OBJECT:
			dst.objhandle = src.objhandle;
			dst.ithandle = src.ithandle;
			break;
		case MSAFE_COUNTERMEASURE_COUNT:
			dst.objhandle = src.objhandle;
			memcpy(dst.name, src.name, MSAFE_NAME_LENGTH);
			break;
		case MSAFE_COUNTERMEASURE_SIZE:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_COUNTERMEASURE_GET:
			dst.objhandle = src.objhandle;
			dst.index = src.index;
			break;
		case MSAFE_ROOM_FOG_STATE:
			dst.roomnum = src.roomnum;
			break;
		case MSAFE_WEAPON_CHECK:
			dst.objhandle = src.objhandle;
			dst.index = src.index;
			break;
		case MSAFE_WEAPON_ADD:
			break;
		default:
			break;
		}
	}

	static void encode_getvalue_fields(int type, const msafe_struct& src, msafe_struct32& dst)
	{
		switch (type)
		{
		case MSAFE_SHOW_ENABLED_CONTROLS:
			dst.objhandle = src.objhandle;
			dst.state = src.state;
			break;
		case MSAFE_OBJECT_PLAYER_CMASK:
			dst.objhandle = src.objhandle;
			dst.control_mask = src.control_mask;
			break;
		case MSAFE_OBJECT_POS:
			dst.objhandle = src.objhandle;
			encode_vector(dst.pos, src.pos);
			break;
		case MSAFE_OBJECT_ORIENT:
			dst.objhandle = src.objhandle;
			encode_matrix(dst.orient, src.orient);
			break;
		case MSAFE_OBJECT_ROOMNUM:
			dst.objhandle = src.objhandle;
			dst.roomnum = src.roomnum;
			break;
		case MSAFE_OBJECT_WORLD_POSITION:
			dst.objhandle = src.objhandle;
			encode_vector(dst.pos, src.pos);
			encode_matrix(dst.orient, src.orient);
			dst.roomnum = src.roomnum;
			break;
		case MSAFE_OBJECT_VELOCITY:
			dst.objhandle = src.objhandle;
			encode_vector(dst.velocity, src.velocity);
			break;
		case MSAFE_OBJECT_ROTVELOCITY:
			dst.objhandle = src.objhandle;
			encode_vector(dst.rot_velocity, src.rot_velocity);
			break;
		case MSAFE_OBJECT_THRUST:
			dst.objhandle = src.objhandle;
			encode_vector(dst.thrust, src.thrust);
			break;
		case MSAFE_OBJECT_ROTTHRUST:
			dst.objhandle = src.objhandle;
			encode_vector(dst.rot_thrust, src.rot_thrust);
			break;
		case MSAFE_OBJECT_FLAGS:
			dst.objhandle = src.objhandle;
			dst.flags = src.flags;
			break;
		case MSAFE_OBJECT_SIZE:
			dst.objhandle = src.objhandle;
			dst.size = src.size;
			break;
		case MSAFE_OBJECT_CONTROL_TYPE:
			dst.objhandle = src.objhandle;
			dst.control_type = src.control_type;
			break;
		case MSAFE_OBJECT_MOVEMENT_TYPE:
			dst.objhandle = src.objhandle;
			dst.movement_type = src.movement_type;
			break;
		case MSAFE_OBJECT_CREATION_TIME:
			dst.objhandle = src.objhandle;
			dst.creation_time = src.creation_time;
			break;
		case MSAFE_OBJECT_PHYSICS_FLAGS:
			dst.objhandle = src.objhandle;
			dst.physics_flags = src.physics_flags;
			break;
		case MSAFE_OBJECT_ROTDRAG:
			dst.objhandle = src.objhandle;
			dst.rot_drag = src.rot_drag;
			break;
		case MSAFE_OBJECT_SHIELDS:
			dst.objhandle = src.objhandle;
			dst.shields = src.shields;
			break;
		case MSAFE_OBJECT_SHIELDS_ORIGINAL:
			dst.objhandle = src.objhandle;
			dst.shields = src.shields;
			break;
		case MSAFE_OBJECT_ENERGY:
			dst.objhandle = src.objhandle;
			dst.energy = src.energy;
			break;
		case MSAFE_OBJECT_MOVEMENT_SCALAR:
			dst.objhandle = src.objhandle;
			dst.scalar = src.scalar;
			break;
		case MSAFE_OBJECT_RECHARGE_SCALAR:
			dst.objhandle = src.objhandle;
			dst.scalar = src.scalar;
			break;
		case MSAFE_OBJECT_WSPEED_SCALAR:
			dst.objhandle = src.objhandle;
			dst.scalar = src.scalar;
			break;
		case MSAFE_OBJECT_ARMOR_SCALAR:
			dst.objhandle = src.objhandle;
			dst.scalar = src.scalar;
			break;
		case MSAFE_OBJECT_DAMAGE_SCALAR:
			dst.objhandle = src.objhandle;
			dst.scalar = src.scalar;
			break;
		case MSAFE_OBJECT_TYPE:
			dst.objhandle = src.objhandle;
			dst.type = src.type;
			break;
		case MSAFE_OBJECT_ID:
			dst.objhandle = src.objhandle;
			dst.id = src.id;
			break;
		case MSAFE_OBJECT_PARENT:
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_ENERGY_WEAPON:
			dst.objhandle = src.objhandle;
			dst.state = src.state;
			dst.amount = src.amount;
			break;
		case MSAFE_OBJECT_DAMAGE_AMOUNT:
			dst.objhandle = src.objhandle;
			dst.amount = src.amount;
			break;
		case MSAFE_OBJECT_COUNT_TYPE:
			dst.type = src.type;
			dst.id = src.id;
			dst.count = src.count;
			break;
		case MSAFE_OBJECT_INVULNERABLE:
			dst.objhandle = src.objhandle;
			dst.state = src.state;
			dst.lifetime = src.lifetime;
			break;
		case MSAFE_OBJECT_CLOAK:
			dst.objhandle = src.objhandle;
			dst.state = src.state;
			dst.lifetime = src.lifetime;
			break;
		case MSAFE_OBJECT_LIGHT_DIST:
			dst.objhandle = src.objhandle;
			dst.light_distance = src.light_distance;
			break;
		case MSAFE_OBJECT_PLAYER_HANDLE:
			dst.slot = src.slot;
			dst.objhandle = src.objhandle;
			break;
		case MSAFE_OBJECT_PLAYER_CONTROLAI:
			dst.slot = src.slot;
			dst.state = src.state;
			break;
		case MSAFE_DOOR_LOCK_STATE:
			dst.objhandle = src.objhandle;
			dst.state = src.state;
			break;
		case MSAFE_DOOR_OPENABLE:
			dst.objhandle = src.objhandle;
			dst.ithandle = src.ithandle;
			dst.state = src.state;
			break;
		case MSAFE_DOOR_POSITION:
			dst.objhandle = src.objhandle;
			dst.scalar = src.scalar;
			break;
		case MSAFE_TRIGGER_SET:
			dst.trigger_num = src.trigger_num;
			dst.state = src.state;
			break;
		case MSAFE_ROOM_HAS_PLAYER:
			dst.roomnum = src.roomnum;
			dst.state = src.state;
			break;
		case MSAFE_ROOM_PORTAL_RENDER:
			dst.roomnum = src.roomnum;
			dst.portalnum = src.portalnum;
			dst.state = src.state;
			break;
		case MSAFE_ROOM_PORTAL_BLOCK:
			dst.roomnum = src.roomnum;
			dst.portalnum = src.portalnum;
			dst.state = src.state;
			break;
		case MSAFE_ROOM_DAMAGE:
			dst.roomnum = src.roomnum;
			dst.amount = src.amount;
			dst.index = src.index;
			break;
		case MSAFE_MISC_ENABLE_SHIP:
			memcpy(dst.name, src.name, MSAFE_NAME_LENGTH);
			dst.state = src.state;
			break;
		case MSAFE_MISC_WAYPOINT:
			dst.index = src.index;
			break;
		case MSAFE_MISC_GUIDEBOT_NAME:
			dst.objhandle = src.objhandle;
			memcpy(dst.name, src.name, MSAFE_NAME_LENGTH);
			break;
		case MSAFE_INVEN_CHECK:
			dst.objhandle = src.objhandle;
			dst.state = src.state;
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_COUNTERMEASURE_CHECK:
			dst.objhandle = src.objhandle;
			dst.state = src.state;
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_INVEN_COUNT:
			dst.objhandle = src.objhandle;
			dst.count = src.count;
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_INVEN_SIZE:
			dst.objhandle = src.objhandle;
			dst.size = src.size;
			break;
		case MSAFE_INVEN_GET_TYPE_ID:
			dst.objhandle = src.objhandle;
			dst.index = src.index;
			dst.count = src.count;
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_INVEN_CHECK_OBJECT:
			dst.objhandle = src.objhandle;
			dst.ithandle = src.ithandle;
			dst.state = src.state;
			break;
		case MSAFE_COUNTERMEASURE_COUNT:
			dst.objhandle = src.objhandle;
			dst.count = src.count;
			memcpy(dst.name, src.name, MSAFE_NAME_LENGTH);
			break;
		case MSAFE_COUNTERMEASURE_SIZE:
			dst.objhandle = src.objhandle;
			dst.size = src.size;
			break;
		case MSAFE_COUNTERMEASURE_GET:
			dst.objhandle = src.objhandle;
			dst.index = src.index;
			dst.count = src.count;
			dst.type = src.type;
			dst.id = src.id;
			break;
		case MSAFE_ROOM_FOG_STATE:
			dst.roomnum = src.roomnum;
			dst.state = src.state;
			break;
		case MSAFE_WEAPON_CHECK:
			dst.objhandle = src.objhandle;
			dst.index = src.index;
			dst.state = src.state;
			dst.count = src.count;
			break;
		case MSAFE_WEAPON_ADD:
			break;
		default:
			break;
		}
	}
}

static_assert(sizeof(msafe_struct32) == msafe_struct_size_32, "msafe_struct ABI size mismatch");

void decode_msafe_getvalue_struct(int type, const void* srcbuf, msafe_struct& dst, const VmPtrDecoder& vm)
{
	memset(&dst, 0, sizeof(dst));
	if (!srcbuf)
		return;

	const msafe_struct32& src = *static_cast<const msafe_struct32*>(srcbuf);
	decode_getvalue_fields(type, src, dst);
	(void)vm;
}

void encode_msafe_getvalue_struct(int type, const msafe_struct& srcbuf, void* dstbuf, const VmPtrEncoder& vm)
{
	if (!dstbuf)
		return;

	msafe_struct32& dst = *static_cast<msafe_struct32*>(dstbuf);
	memset(&dst, 0, sizeof(dst));
	encode_getvalue_fields(type, srcbuf, dst);
	(void)vm;
}
}
