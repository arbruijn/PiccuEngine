#ifndef EMUABI_MSAFE_STRUCT32_H
#define EMUABI_MSAFE_STRUCT32_H

#include <cstdint>

namespace emuabi
{
struct vector32
{
	float x;
	float y;
	float z;
};

struct matrix32
{
	vector32 rvec;
	vector32 uvec;
	vector32 fvec;
};

struct msafe_struct32
{
	int32_t roomnum;
	int16_t facenum;
	int16_t texnum;
	int16_t portalnum;
	float fog_r;
	float fog_g;
	float fog_b;
	float fog_depth;
	vector32 wind;
	uint8_t pulse_time;
	uint8_t pulse_offset;
	uint32_t objhandle;
	uint32_t ithandle;
	float shields;
	float energy;
	int16_t start_tick;
	int16_t end_tick;
	float cycle_time;
	int32_t type;
	int32_t id;
	int32_t aux_type;
	int32_t aux_id;
	uint32_t checksum;
	int32_t path_id;
	float amount;
	uint8_t damage_type;
	uint32_t killer_handle;
	float ammo;
	uint8_t playsound;
	uint8_t remove;
	uint8_t do_powerup;
	vector32 velocity;
	vector32 rot_velocity;
	float rot_drag;
	vector32 thrust;
	vector32 rot_thrust;
	int8_t control_type;
	int8_t movement_type;
	float creation_time;
	int32_t physics_flags;
	vector32 pos;
	matrix32 orient;
	float anim_frame;
	uint8_t is_real;
	uint8_t random;
	uint8_t unused2;
	int8_t gunpoint;
	uint8_t effect_type;
	uint8_t phys_info;
	float drag;
	float mass;
	uint16_t unused;
	int32_t randval;
	uint8_t trigger_num;
	int32_t sound_handle;
	float volume;
	int32_t index;
	float scalar;
	float interval;
	uint8_t state;
	int8_t slot;
	char message[MSAFE_MESSAGE_LENGTH];
	char name[MSAFE_NAME_LENGTH];
	int32_t color;
	float longevity;
	float lifetime;
	float size;
	float speed;
	int32_t count;
	int32_t flags;
	uint32_t list;
	uint32_t control_mask;
	uint8_t control_val;
	float light_distance;
	float r1;
	float g1;
	float b1;
	float r2;
	float g2;
	float b2;
	float time_interval;
	float flicker_distance;
	float directional_dot;
	int32_t timebits;
	vector32 pos2;
	char message2[MSAFE_MESSAGE_LENGTH];
};
}

#endif
