#include "osiris_event_callers.h"

#include <string.h>

#define OsirisEventCall_Test(event, data) CallInstanceEvent(0, 0, event, data)

namespace
{
	void ZeroEventInfo(tOSIRISEventInfo* data)
	{
		memset(data, 0, sizeof(*data));
	}

	void SeedCommon(tOSIRISEventInfo* data)
	{
		data->me_handle = 1001;
	}
}

void TestCaller_EVT_INTERVAL(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_interval.frame_time = 0.125f;
	data.evt_interval.game_time = 42.5f;
	OsirisEventCall_Test(EVT_INTERVAL, &data);
}

void TestCaller_EVT_AI_FRAME(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	OsirisEventCall_Test(EVT_AI_FRAME, &data);
}

void TestCaller_EVT_DAMAGED(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_damaged.damage = 12.5f;
	data.evt_damaged.it_handle = 2002;
	data.evt_damaged.weapon_handle = 3003;
	data.evt_damaged.damage_type = 4;
	OsirisEventCall_Test(EVT_DAMAGED, &data);
}

void TestCaller_EVT_COLLIDE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_collide.it_handle = 2002;
	OsirisEventCall_Test(EVT_COLLIDE, &data);
}

void TestCaller_EVT_CREATED(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	OsirisEventCall_Test(EVT_CREATED, &data);
}

void TestCaller_EVT_DESTROY(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_destroy.is_dying = 1;
	OsirisEventCall_Test(EVT_DESTROY, &data);
}

void TestCaller_EVT_TIMER(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_timer.id = 17;
	data.evt_timer.game_time = 84.25f;
	OsirisEventCall_Test(EVT_TIMER, &data);
}

void TestCaller_EVT_USE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_use.it_handle = 2002;
	OsirisEventCall_Test(EVT_USE, &data);
}

void TestCaller_EVT_AI_NOTIFY(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_SCRIPTED_ENABLER;
	data.evt_ai_notify.it_handle = 2002;
	data.evt_ai_notify.goal_num = 7;
	data.evt_ai_notify.goal_uid = 77;
	data.evt_ai_notify.enabler_num = 9;
	OsirisEventCall_Test(EVT_AI_NOTIFY, &data);
}

void TestCaller_EVT_AI_INIT(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	OsirisEventCall_Test(EVT_AI_INIT, &data);
}

void TestCaller_EVT_CHANGESEG(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_changeseg.room_num = 23;
	OsirisEventCall_Test(EVT_CHANGESEG, &data);
}

void TestCaller_EVT_SAVESTATE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_savestate.fileptr = reinterpret_cast<void*>(0x1234);
	OsirisEventCall_Test(EVT_SAVESTATE, &data);
}

void TestCaller_EVT_RESTORESTATE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_restorestate.fileptr = reinterpret_cast<void*>(0x2345);
	OsirisEventCall_Test(EVT_RESTORESTATE, &data);
}

void TestCaller_EVT_MEMRESTORE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_memrestore.id = 31;
	data.evt_memrestore.memory_ptr = reinterpret_cast<void*>(0x3456);
	OsirisEventCall_Test(EVT_MEMRESTORE, &data);
}

void TestCaller_EVT_TIMERCANCEL(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_timercancel.handle = 41;
	data.evt_timercancel.detonated = 1;
	OsirisEventCall_Test(EVT_TIMERCANCEL, &data);
}

void TestCaller_EVT_AIN_OBJKILLED(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_OBJ_KILLED;
	data.evt_ai_notify.it_handle = 2002;
	OsirisEventCall_Test(EVT_AIN_OBJKILLED, &data);
}

void TestCaller_EVT_AIN_SEEPLAYER(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_SEE_TARGET;
	data.evt_ai_notify.it_handle = 2002;
	OsirisEventCall_Test(EVT_AIN_SEEPLAYER, &data);
}

void TestCaller_EVT_AIN_WHITOBJECT(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_WHIT_OBJECT;
	data.evt_ai_notify.it_handle = 2002;
	OsirisEventCall_Test(EVT_AIN_WHITOBJECT, &data);
}

void TestCaller_EVT_AIN_GOALCOMPLETE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_GOAL_COMPLETE;
	data.evt_ai_notify.it_handle = 2002;
	data.evt_ai_notify.goal_num = 7;
	data.evt_ai_notify.goal_uid = 77;
	data.evt_ai_notify.enabler_num = 9;
	OsirisEventCall_Test(EVT_AIN_GOALCOMPLETE, &data);
}

void TestCaller_EVT_AIN_GOALFAIL(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_GOAL_FAIL;
	data.evt_ai_notify.it_handle = 2002;
	data.evt_ai_notify.goal_num = 8;
	data.evt_ai_notify.goal_uid = 88;
	data.evt_ai_notify.enabler_num = 10;
	OsirisEventCall_Test(EVT_AIN_GOALFAIL, &data);
}

void TestCaller_EVT_AIN_MELEE_HIT(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_MELEE_HIT;
	data.evt_ai_notify.it_handle = 2002;
	data.evt_ai_notify.attack_num = 5;
	OsirisEventCall_Test(EVT_AIN_MELEE_HIT, &data);
}

void TestCaller_EVT_AIN_MELEE_ATTACK_FRAME(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_MELEE_ATTACK_FRAME;
	data.evt_ai_notify.it_handle = 2002;
	data.evt_ai_notify.attack_num = 6;
	OsirisEventCall_Test(EVT_AIN_MELEE_ATTACK_FRAME, &data);
}

void TestCaller_EVT_AIN_MOVIE_START(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_MOVIE_START;
	data.evt_ai_notify.it_handle = 2002;
	OsirisEventCall_Test(EVT_AIN_MOVIE_START, &data);
}

void TestCaller_EVT_AIN_MOVIE_END(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_ai_notify.notify_type = AIN_MOVIE_END;
	data.evt_ai_notify.it_handle = 2002;
	OsirisEventCall_Test(EVT_AIN_MOVIE_END, &data);
}

void TestCaller_EVT_MATCEN_CREATE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_matcen_create.it_handle = 2002;
	data.evt_matcen_create.id = 12;
	OsirisEventCall_Test(EVT_MATCEN_CREATE, &data);
}

void TestCaller_EVT_DOOR_ACTIVATE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	OsirisEventCall_Test(EVT_DOOR_ACTIVATE, &data);
}

void TestCaller_EVT_DOOR_CLOSE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	OsirisEventCall_Test(EVT_DOOR_CLOSE, &data);
}

void TestCaller_EVT_CHILD_DIED(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	SeedCommon(&data);
	data.evt_child_died.it_handle = 2002;
	OsirisEventCall_Test(EVT_CHILD_DIED, &data);
}

void TestCaller_EVT_LEVEL_GOAL_COMPLETE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_level_goal_complete.level_goal_index = 3;
	OsirisEventCall_Test(EVT_LEVEL_GOAL_COMPLETE, &data);
}

void TestCaller_EVT_ALL_LEVEL_GOALS_COMPLETE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	OsirisEventCall_Test(EVT_ALL_LEVEL_GOALS_COMPLETE, &data);
}

void TestCaller_EVT_LEVEL_GOAL_ITEM_COMPLETE(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_level_goal_item_complete.level_goal_index = 4;
	OsirisEventCall_Test(EVT_LEVEL_GOAL_ITEM_COMPLETE, &data);
}

void TestCaller_EVT_PLAYER_MOVIE_START(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	OsirisEventCall_Test(EVT_PLAYER_MOVIE_START, &data);
}

void TestCaller_EVT_PLAYER_MOVIE_END(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	OsirisEventCall_Test(EVT_PLAYER_MOVIE_END, &data);
}

void TestCaller_EVT_PLAYER_RESPAWN(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_player_respawn.it_handle = 2002;
	OsirisEventCall_Test(EVT_PLAYER_RESPAWN, &data);
}

void TestCaller_EVT_PLAYER_DIES(void)
{
	tOSIRISEventInfo data;
	ZeroEventInfo(&data);
	data.me_handle = -1;
	data.evt_player_dies.it_handle = 2002;
	OsirisEventCall_Test(EVT_PLAYER_DIES, &data);
}

void RunAllOsirisEventTestCallers(void)
{
#define RUN_OSIRIS_EVENT_CALLER(name) TestCaller_##name();
	OSIRIS_EVENT_TEST_CALLERS(RUN_OSIRIS_EVENT_CALLER)
#undef RUN_OSIRIS_EVENT_CALLER
}
