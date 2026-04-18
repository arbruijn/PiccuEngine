#include "msafe_callfunction_callers.h"

#include <stddef.h>

#define msafe_CallFunction_Test msafe_CallFunction
#define strcpy CopyCString

namespace
{
	static void ZeroMSafeStruct(msafe_struct* mstruct)
	{
		unsigned char* bytes = (unsigned char*)mstruct;
		for (size_t i = 0; i < sizeof(*mstruct); ++i)
		{
			bytes[i] = 0;
		}
	}

	static void CopyCString(char* dst, const char* src)
	{
		while ((*dst++ = *src++) != '\0')
		{
		}
	}
}

void TestCaller_MSAFE_WEATHER_RAIN(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.scalar = 1.25f;
	msafe_CallFunction_Test(MSAFE_WEATHER_RAIN, &mstruct);
}

void TestCaller_MSAFE_WEATHER_SNOW(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.scalar = 0.65f;
	msafe_CallFunction_Test(MSAFE_WEATHER_SNOW, &mstruct);
}

void TestCaller_MSAFE_WEATHER_LIGHTNING(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.scalar = 0.4f;
	mstruct.randval = 13;
	msafe_CallFunction_Test(MSAFE_WEATHER_LIGHTNING, &mstruct);
}

void TestCaller_MSAFE_WEATHER_LIGHTNING_BOLT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.texnum = 42;
	mstruct.flags = 0x40;
	mstruct.objhandle = 1001;
	mstruct.ithandle = 1002;
	mstruct.roomnum = 7;
	mstruct.pos.x = 10.0f;
	mstruct.pos.y = 20.0f;
	mstruct.pos.z = 30.0f;
	mstruct.pos2.x = 40.0f;
	mstruct.pos2.y = 50.0f;
	mstruct.pos2.z = 60.0f;
	mstruct.lifetime = 5.5f;
	mstruct.color = 0x112233;
	mstruct.size = 6.5f;
	mstruct.state = 1;
	mstruct.count = 8;
	mstruct.interval = 2.5f;
	mstruct.index = 14;
	mstruct.g1 = 12.0f;
	mstruct.g2 = 24.0f;
	msafe_CallFunction_Test(MSAFE_WEATHER_LIGHTNING_BOLT, &mstruct);
}

void TestCaller_MSAFE_ROOM_TEXTURE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.facenum = 3;
	mstruct.index = 14;
	msafe_CallFunction_Test(MSAFE_ROOM_TEXTURE, &mstruct);
}

void TestCaller_MSAFE_ROOM_WIND(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.wind.x = 4.0f;
	mstruct.wind.y = 5.0f;
	mstruct.wind.z = 6.0f;
	msafe_CallFunction_Test(MSAFE_ROOM_WIND, &mstruct);
}

void TestCaller_MSAFE_ROOM_FOG(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.fog_r = 0.1f;
	mstruct.fog_g = 0.2f;
	mstruct.fog_b = 0.3f;
	mstruct.fog_depth = 123.0f;
	msafe_CallFunction_Test(MSAFE_ROOM_FOG, &mstruct);
}

void TestCaller_MSAFE_ROOM_FOG_STATE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_ROOM_FOG_STATE, &mstruct);
}

void TestCaller_MSAFE_ROOM_LIGHT_PULSE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.pulse_time = 9;
	mstruct.pulse_offset = 11;
	msafe_CallFunction_Test(MSAFE_ROOM_LIGHT_PULSE, &mstruct);
}

void TestCaller_MSAFE_ROOM_LIGHT_FLICKER(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_ROOM_LIGHT_FLICKER, &mstruct);
}

void TestCaller_MSAFE_ROOM_LIGHT_STROBE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_ROOM_LIGHT_STROBE, &mstruct);
}

void TestCaller_MSAFE_ROOM_REFUEL(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_ROOM_REFUEL, &mstruct);
}

void TestCaller_MSAFE_ROOM_CHANGING_FOG(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.fog_r = 0.1f;
	mstruct.fog_g = 0.2f;
	mstruct.fog_b = 0.3f;
	mstruct.fog_depth = 123.0f;
	mstruct.interval = 2.5f;
	msafe_CallFunction_Test(MSAFE_ROOM_CHANGING_FOG, &mstruct);
}

void TestCaller_MSAFE_ROOM_CHANGING_WIND(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.wind.x = 4.0f;
	mstruct.wind.y = 5.0f;
	mstruct.wind.z = 6.0f;
	mstruct.interval = 2.5f;
	msafe_CallFunction_Test(MSAFE_ROOM_CHANGING_WIND, &mstruct);
}

void TestCaller_MSAFE_ROOM_PORTAL_RENDER(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.portalnum = 2;
	mstruct.state = 1;
	mstruct.flags = 0x40;
	msafe_CallFunction_Test(MSAFE_ROOM_PORTAL_RENDER, &mstruct);
}

void TestCaller_MSAFE_ROOM_PORTAL_BLOCK(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.portalnum = 2;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_ROOM_PORTAL_BLOCK, &mstruct);
}

void TestCaller_MSAFE_ROOM_BREAK_GLASS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.portalnum = 2;
	msafe_CallFunction_Test(MSAFE_ROOM_BREAK_GLASS, &mstruct);
}

void TestCaller_MSAFE_ROOM_DAMAGE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	mstruct.amount = 12.5f;
	mstruct.index = 14;
	msafe_CallFunction_Test(MSAFE_ROOM_DAMAGE, &mstruct);
}

void TestCaller_MSAFE_ROOM_REMOVE_ALL_POWERUPS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.roomnum = 7;
	msafe_CallFunction_Test(MSAFE_ROOM_REMOVE_ALL_POWERUPS, &mstruct);
}

void TestCaller_MSAFE_OBJECT_FIRE_WEAPON(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.index = 14;
	mstruct.gunpoint = 2;
	msafe_CallFunction_Test(MSAFE_OBJECT_FIRE_WEAPON, &mstruct);
}

void TestCaller_MSAFE_OBJECT_SETONFIRE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.longevity = 4.5f;
	mstruct.interval = 2.5f;
	mstruct.ithandle = 1002;
	msafe_CallFunction_Test(MSAFE_OBJECT_SETONFIRE, &mstruct);
}

void TestCaller_MSAFE_OBJECT_SHIELDS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.shields = 87.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_SHIELDS, &mstruct);
}

void TestCaller_MSAFE_OBJECT_ENERGY(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.energy = 61.25f;
	msafe_CallFunction_Test(MSAFE_OBJECT_ENERGY, &mstruct);
}

void TestCaller_MSAFE_SHOW_ENABLED_CONTROLS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_SHOW_ENABLED_CONTROLS, &mstruct);
}

void TestCaller_MSAFE_OBJECT_PLAYER_CMASK(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.control_mask = 0x200;
	mstruct.control_val = 1;
	msafe_CallFunction_Test(MSAFE_OBJECT_PLAYER_CMASK, &mstruct);
}

void TestCaller_MSAFE_OBJECT_PLAYER_KEY(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.ithandle = 1002;
	mstruct.index = 14;
	mstruct.objhandle = 1001;
	strcpy(mstruct.message, "primary message");
	msafe_CallFunction_Test(MSAFE_OBJECT_PLAYER_KEY, &mstruct);
}

void TestCaller_MSAFE_OBJECT_MOVEMENT_SCALAR(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.scalar = 1.25f;
	msafe_CallFunction_Test(MSAFE_OBJECT_MOVEMENT_SCALAR, &mstruct);
}

void TestCaller_MSAFE_OBJECT_RECHARGE_SCALAR(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.scalar = 1.25f;
	msafe_CallFunction_Test(MSAFE_OBJECT_RECHARGE_SCALAR, &mstruct);
}

void TestCaller_MSAFE_OBJECT_WSPEED_SCALAR(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.scalar = 1.25f;
	msafe_CallFunction_Test(MSAFE_OBJECT_WSPEED_SCALAR, &mstruct);
}

void TestCaller_MSAFE_OBJECT_ARMOR_SCALAR(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.scalar = 1.25f;
	msafe_CallFunction_Test(MSAFE_OBJECT_ARMOR_SCALAR, &mstruct);
}

void TestCaller_MSAFE_OBJECT_DAMAGE_SCALAR(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.scalar = 1.25f;
	msafe_CallFunction_Test(MSAFE_OBJECT_DAMAGE_SCALAR, &mstruct);
}

void TestCaller_MSAFE_OBJECT_ADD_WEAPON(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.index = 14;
	mstruct.ammo = 44.0f;
	msafe_CallFunction_Test(MSAFE_OBJECT_ADD_WEAPON, &mstruct);
}

void TestCaller_MSAFE_OBJECT_DAMAGE_OBJECT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.killer_handle = 1003;
	mstruct.damage_type = 4;
	mstruct.amount = 12.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_DAMAGE_OBJECT, &mstruct);
}

void TestCaller_MSAFE_OBJECT_START_SPEW(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.gunpoint = 2;
	mstruct.random = 1;
	mstruct.is_real = 1;
	mstruct.effect_type = 7;
	mstruct.phys_info = 8;
	mstruct.drag = 0.4f;
	mstruct.mass = 3.5f;
	mstruct.interval = 2.5f;
	mstruct.longevity = 4.5f;
	mstruct.lifetime = 5.5f;
	mstruct.size = 6.5f;
	mstruct.speed = 7.5f;
	mstruct.id = 17;
	msafe_CallFunction_Test(MSAFE_OBJECT_START_SPEW, &mstruct);
}

void TestCaller_MSAFE_OBJECT_STOP_SPEW(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.id = 17;
	msafe_CallFunction_Test(MSAFE_OBJECT_STOP_SPEW, &mstruct);
}

void TestCaller_MSAFE_OBJECT_NO_RENDER(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	msafe_CallFunction_Test(MSAFE_OBJECT_NO_RENDER, &mstruct);
}

void TestCaller_MSAFE_OBJECT_GHOST(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	msafe_CallFunction_Test(MSAFE_OBJECT_GHOST, &mstruct);
}

void TestCaller_MSAFE_OBJECT_UNGHOST(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	msafe_CallFunction_Test(MSAFE_OBJECT_UNGHOST, &mstruct);
}

void TestCaller_MSAFE_OBJECT_REMOVE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.playsound = 1;
	msafe_CallFunction_Test(MSAFE_OBJECT_REMOVE, &mstruct);
}

void TestCaller_MSAFE_OBJECT_INVULNERABLE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.state = 1;
	mstruct.lifetime = 5.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_INVULNERABLE, &mstruct);
}

void TestCaller_MSAFE_OBJECT_CLOAKALLPLAYERS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.lifetime = 5.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_CLOAKALLPLAYERS, &mstruct);
}

void TestCaller_MSAFE_OBJECT_CLOAK(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.state = 1;
	mstruct.lifetime = 5.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_CLOAK, &mstruct);
}

void TestCaller_MSAFE_OBJECT_LIGHT_DIST(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.light_distance = 120.0f;
	msafe_CallFunction_Test(MSAFE_OBJECT_LIGHT_DIST, &mstruct);
}

void TestCaller_MSAFE_OBJECT_LIGHT_COLOR(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.r1 = 0.7f;
	mstruct.g1 = 0.5f;
	mstruct.b1 = 0.3f;
	msafe_CallFunction_Test(MSAFE_OBJECT_LIGHT_COLOR, &mstruct);
}

void TestCaller_MSAFE_OBJECT_DEFORM(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.amount = 12.5f;
	mstruct.lifetime = 5.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_DEFORM, &mstruct);
}

void TestCaller_MSAFE_OBJECT_SPARKS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.amount = 12.5f;
	mstruct.lifetime = 5.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_SPARKS, &mstruct);
}

void TestCaller_MSAFE_OBJECT_VIEWER_SHAKE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.amount = 12.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_VIEWER_SHAKE, &mstruct);
}

void TestCaller_MSAFE_OBJECT_SHAKE_AREA(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.scalar = 1.25f;
	mstruct.amount = 12.5f;
	msafe_CallFunction_Test(MSAFE_OBJECT_SHAKE_AREA, &mstruct);
}

void TestCaller_MSAFE_OBJECT_WORLD_POSITION(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.roomnum = 7;
	mstruct.pos.x = 10.0f;
	mstruct.pos.y = 20.0f;
	mstruct.pos.z = 30.0f;
	mstruct.orient.rvec.x = 1.0f;
	mstruct.orient.rvec.y = 0.0f;
	mstruct.orient.rvec.z = 0.0f;
	mstruct.orient.uvec.x = 0.0f;
	mstruct.orient.uvec.y = 1.0f;
	mstruct.orient.uvec.z = 0.0f;
	mstruct.orient.fvec.x = 0.0f;
	mstruct.orient.fvec.y = 0.0f;
	mstruct.orient.fvec.z = 1.0f;
	msafe_CallFunction_Test(MSAFE_OBJECT_WORLD_POSITION, &mstruct);
}

void TestCaller_MSAFE_OBJECT_PLAYER_CONTROLAI(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.slot = 3;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_OBJECT_PLAYER_CONTROLAI, &mstruct);
}

void TestCaller_MSAFE_OBJECT_DESTROY_ROBOTS_EXCEPT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.list = NULL;
	mstruct.count = 0;
	msafe_CallFunction_Test(MSAFE_OBJECT_DESTROY_ROBOTS_EXCEPT, &mstruct);
}

void TestCaller_MSAFE_SOUND_STREAMING(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.objhandle = 1001;
	strcpy(mstruct.name, "TestName");
	mstruct.volume = 0.8f;
	mstruct.flags = 0x40;
	mstruct.sound_handle = 91;
	mstruct.slot = 3;
	msafe_CallFunction_Test(MSAFE_SOUND_STREAMING, &mstruct);
}

void TestCaller_MSAFE_SOUND_2D(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.objhandle = 1001;
	mstruct.index = 14;
	mstruct.volume = 0.8f;
	mstruct.sound_handle = 91;
	mstruct.slot = 3;
	msafe_CallFunction_Test(MSAFE_SOUND_2D, &mstruct);
}

void TestCaller_MSAFE_SOUND_OBJECT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.index = 14;
	mstruct.sound_handle = 91;
	msafe_CallFunction_Test(MSAFE_SOUND_OBJECT, &mstruct);
}

void TestCaller_MSAFE_SOUND_STOP(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	msafe_CallFunction_Test(MSAFE_SOUND_STOP, &mstruct);
}

void TestCaller_MSAFE_SOUND_STOP_OBJ(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	msafe_CallFunction_Test(MSAFE_SOUND_STOP_OBJ, &mstruct);
}

void TestCaller_MSAFE_SOUND_VOLUME_OBJ(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.volume = 0.8f;
	msafe_CallFunction_Test(MSAFE_SOUND_VOLUME_OBJ, &mstruct);
}

void TestCaller_MSAFE_MUSIC_REGION(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.objhandle = 1001;
	mstruct.index = 14;
	mstruct.slot = 3;
	msafe_CallFunction_Test(MSAFE_MUSIC_REGION, &mstruct);
}

void TestCaller_MSAFE_MISC_LEVELGOAL(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.index = 14;
	strcpy(mstruct.message, "primary message");
	mstruct.type = 2;
	mstruct.count = 8;
	msafe_CallFunction_Test(MSAFE_MISC_LEVELGOAL, &mstruct);
}

void TestCaller_MSAFE_MISC_WAYPOINT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.index = 14;
	msafe_CallFunction_Test(MSAFE_MISC_WAYPOINT, &mstruct);
}

void TestCaller_MSAFE_MISC_ENABLE_SHIP(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	strcpy(mstruct.name, "TestName");
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_MISC_ENABLE_SHIP, &mstruct);
}

void TestCaller_MSAFE_MISC_FILTERED_HUD_MESSAGE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.objhandle = 1001;
	strcpy(mstruct.message, "primary message");
	mstruct.color = 0x112233;
	mstruct.slot = 3;
	msafe_CallFunction_Test(MSAFE_MISC_FILTERED_HUD_MESSAGE, &mstruct);
}

void TestCaller_MSAFE_MISC_HUD_MESSAGE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.objhandle = 1001;
	strcpy(mstruct.message, "primary message");
	mstruct.color = 0x112233;
	mstruct.slot = 3;
	msafe_CallFunction_Test(MSAFE_MISC_HUD_MESSAGE, &mstruct);
}

void TestCaller_MSAFE_MISC_GAME_MESSAGE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	mstruct.objhandle = 1001;
	strcpy(mstruct.message, "primary message");
	strcpy(mstruct.message2, "secondary message");
	mstruct.color = 0x112233;
	mstruct.slot = 3;
	msafe_CallFunction_Test(MSAFE_MISC_GAME_MESSAGE, &mstruct);
}

void TestCaller_MSAFE_MISC_END_LEVEL(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_MISC_END_LEVEL, &mstruct);
}

void TestCaller_MSAFE_MISC_POPUP_CAMERA(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.interval = 2.5f;
	mstruct.scalar = 1.25f;
	mstruct.gunpoint = 2;
	msafe_CallFunction_Test(MSAFE_MISC_POPUP_CAMERA, &mstruct);
}

void TestCaller_MSAFE_MISC_CLOSE_POPUP(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	msafe_CallFunction_Test(MSAFE_MISC_CLOSE_POPUP, &mstruct);
}

void TestCaller_MSAFE_MISC_GUIDEBOT_NAME(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	strcpy(mstruct.name, "TestName");
	msafe_CallFunction_Test(MSAFE_MISC_GUIDEBOT_NAME, &mstruct);
}

void TestCaller_MSAFE_MISC_START_TIMER(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.index = 14;
	mstruct.color = 0x112233;
	msafe_CallFunction_Test(MSAFE_MISC_START_TIMER, &mstruct);
}

void TestCaller_MSAFE_MISC_UPDATE_HUD_ITEM(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	strcpy(mstruct.message, "primary message");
	mstruct.color = 0x112233;
	msafe_CallFunction_Test(MSAFE_MISC_UPDATE_HUD_ITEM, &mstruct);
}

void TestCaller_MSAFE_DOOR_LOCK_STATE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_DOOR_LOCK_STATE, &mstruct);
}

void TestCaller_MSAFE_DOOR_ACTIVATE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	msafe_CallFunction_Test(MSAFE_DOOR_ACTIVATE, &mstruct);
}

void TestCaller_MSAFE_DOOR_STOP(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	msafe_CallFunction_Test(MSAFE_DOOR_STOP, &mstruct);
}

void TestCaller_MSAFE_DOOR_POSITION(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.scalar = 1.25f;
	msafe_CallFunction_Test(MSAFE_DOOR_POSITION, &mstruct);
}

void TestCaller_MSAFE_TRIGGER_SET(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.trigger_num = 5;
	mstruct.state = 1;
	msafe_CallFunction_Test(MSAFE_TRIGGER_SET, &mstruct);
}

void TestCaller_MSAFE_INVEN_ADD_OBJECT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.ithandle = 1002;
	mstruct.flags = 0x40;
	strcpy(mstruct.message, "primary message");
	msafe_CallFunction_Test(MSAFE_INVEN_ADD_OBJECT, &mstruct);
}

void TestCaller_MSAFE_INVEN_REMOVE_OBJECT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.ithandle = 1002;
	msafe_CallFunction_Test(MSAFE_INVEN_REMOVE_OBJECT, &mstruct);
}

void TestCaller_MSAFE_INVEN_ADD_TYPE_ID(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.type = 2;
	mstruct.id = 17;
	mstruct.flags = 0x40;
	msafe_CallFunction_Test(MSAFE_INVEN_ADD_TYPE_ID, &mstruct);
}

void TestCaller_MSAFE_INVEN_REMOVE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.type = 2;
	mstruct.id = 17;
	msafe_CallFunction_Test(MSAFE_INVEN_REMOVE, &mstruct);
}

void TestCaller_MSAFE_COUNTERMEASURE_REMOVE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.type = 2;
	mstruct.id = 17;
	msafe_CallFunction_Test(MSAFE_COUNTERMEASURE_REMOVE, &mstruct);
}

void TestCaller_MSAFE_COUNTERMEASURE_ADD(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.count = 8;
	strcpy(mstruct.name, "TestName");
	mstruct.aux_type = 3;
	mstruct.aux_id = 18;
	msafe_CallFunction_Test(MSAFE_COUNTERMEASURE_ADD, &mstruct);
}

void TestCaller_MSAFE_OBJECT_ROTDRAG(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.rot_drag = 0.75f;
	msafe_CallFunction_Test(MSAFE_OBJECT_ROTDRAG, &mstruct);
}

void TestCaller_MSAFE_OBJECT_TYPE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.type = 2;
	msafe_CallFunction_Test(MSAFE_OBJECT_TYPE, &mstruct);
}

void TestCaller_MSAFE_OBJECT_ID(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.id = 17;
	msafe_CallFunction_Test(MSAFE_OBJECT_ID, &mstruct);
}

void TestCaller_MSAFE_OBJECT_CONTROL_TYPE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.control_type = 5;
	msafe_CallFunction_Test(MSAFE_OBJECT_CONTROL_TYPE, &mstruct);
}

void TestCaller_MSAFE_OBJECT_FLAGS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.flags = 0x40;
	msafe_CallFunction_Test(MSAFE_OBJECT_FLAGS, &mstruct);
}

void TestCaller_MSAFE_OBJECT_MOVEMENT_TYPE(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.movement_type = 6;
	msafe_CallFunction_Test(MSAFE_OBJECT_MOVEMENT_TYPE, &mstruct);
}

void TestCaller_MSAFE_OBJECT_CREATION_TIME(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.creation_time = 77.25f;
	msafe_CallFunction_Test(MSAFE_OBJECT_CREATION_TIME, &mstruct);
}

void TestCaller_MSAFE_OBJECT_PHYSICS_FLAGS(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.physics_flags = 0x1234;
	msafe_CallFunction_Test(MSAFE_OBJECT_PHYSICS_FLAGS, &mstruct);
}

void TestCaller_MSAFE_OBJECT_PARENT(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.ithandle = 1002;
	msafe_CallFunction_Test(MSAFE_OBJECT_PARENT, &mstruct);
}

void TestCaller_MSAFE_WEAPON_CHECK(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	msafe_CallFunction_Test(MSAFE_WEAPON_CHECK, &mstruct);
}

void TestCaller_MSAFE_WEAPON_ADD(void)
{
	msafe_struct mstruct;
	ZeroMSafeStruct(&mstruct);
	mstruct.objhandle = 1001;
	mstruct.index = 14;
	mstruct.state = 1;
	mstruct.count = 8;
	msafe_CallFunction_Test(MSAFE_WEAPON_ADD, &mstruct);
}

void RunAllMSafeCallFunctionTestCallers(void)
{
#define RUN_MSAFE_CALLER(name) TestCaller_##name();
	MSAFE_CALLFUNCTION_TEST_CALLERS(RUN_MSAFE_CALLER)
#undef RUN_MSAFE_CALLER
}
