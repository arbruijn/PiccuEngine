#include "eventinfo.h"

#include <stddef.h>
#include <string.h>

namespace emuabi
{
namespace
{
#define ABI_COPY_I32(name) dst.name = src.name;
#define ABI_COPY_U8(name) dst.name = src.name;
#define ABI_COPY_F32(name) dst.name = src.name;
#define ABI_COPY_PTR32(name) dst.name = vm.encode_ptr32(src.name);

#define ABI_DECL_I32(name) int32_t name;
#define ABI_DECL_U8(name) uint8_t name;
#define ABI_DECL_F32(name) float name;
#define ABI_DECL_PTR32(name) uint32_t name;

#define OSIRIS_EVTINTERVAL_FIELDS(APPLY) \
	APPLY(F32, frame_time) \
	APPLY(F32, game_time)

#define OSIRIS_EVTDAMAGED_FIELDS(APPLY) \
	APPLY(F32, damage) \
	APPLY(I32, it_handle) \
	APPLY(I32, weapon_handle) \
	APPLY(I32, damage_type)

#define OSIRIS_EVTCOLLIDE_FIELDS(APPLY) \
	APPLY(I32, it_handle)

#define OSIRIS_EVTDESTROY_FIELDS(APPLY) \
	APPLY(U8, is_dying)

#define OSIRIS_EVTTIMER_FIELDS(APPLY) \
	APPLY(I32, id) \
	APPLY(F32, game_time)

#define OSIRIS_EVTUSE_FIELDS(APPLY) \
	APPLY(I32, it_handle)

#define OSIRIS_EVTSAVESTATE_FIELDS(APPLY) \
	APPLY(PTR32, fileptr)

#define OSIRIS_EVTRESTORESTATE_FIELDS(APPLY) \
	APPLY(PTR32, fileptr)

#define OSIRIS_EVTAINOTIFY_FIELDS(APPLY) \
	APPLY(I32, notify_type) \
	APPLY(I32, it_handle) \
	APPLY(I32, goal_num) \
	APPLY(I32, goal_uid) \
	APPLY(I32, enabler_num)

#define OSIRIS_EVTCHANGESEG_FIELDS(APPLY) \
	APPLY(I32, room_num)

#define OSIRIS_EVTMEMRESTORE_FIELDS(APPLY) \
	APPLY(I32, id) \
	APPLY(PTR32, memory_ptr)

#define OSIRIS_EVTTIMERCANCEL_FIELDS(APPLY) \
	APPLY(I32, handle) \
	APPLY(U8, detonated)

#define OSIRIS_EVTCHILDDIED_FIELDS(APPLY) \
	APPLY(I32, it_handle)

#define OSIRIS_EVTMATCENCREATE_FIELDS(APPLY) \
	APPLY(I32, it_handle) \
	APPLY(I32, id)

#define OSIRIS_EVTLEVELGOALCOMPLETE_FIELDS(APPLY) \
	APPLY(I32, level_goal_index)

#define OSIRIS_EVTLEVELGOALITEMCOMPLETE_FIELDS(APPLY) \
	APPLY(I32, level_goal_index)

#define OSIRIS_EVTPLAYERRESPAWN_FIELDS(APPLY) \
	APPLY(I32, it_handle)

#define OSIRIS_EVTPLAYERDIES_FIELDS(APPLY) \
	APPLY(I32, it_handle)

#define DECLARE_32_STRUCT(native_type, generated_type, fields_macro) \
	struct generated_type \
	{ \
		fields_macro(DECLARE_32_STRUCT_FIELD) \
	};

#define DECLARE_32_STRUCT_FIELD(kind, name) ABI_DECL_##kind(name)

	DECLARE_32_STRUCT(tOSIRISEVTINTERVAL, EventInterval32, OSIRIS_EVTINTERVAL_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTDAMAGED, EventDamaged32, OSIRIS_EVTDAMAGED_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTCOLLIDE, EventCollide32, OSIRIS_EVTCOLLIDE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTDESTROY, EventDestroy32, OSIRIS_EVTDESTROY_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTTIMER, EventTimer32, OSIRIS_EVTTIMER_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTUSE, EventUse32, OSIRIS_EVTUSE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTSAVESTATE, EventSaveState32, OSIRIS_EVTSAVESTATE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTRESTORESTATE, EventRestoreState32, OSIRIS_EVTRESTORESTATE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTAINOTIFY, EventAINotify32, OSIRIS_EVTAINOTIFY_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTCHANGESEG, EventChangeSeg32, OSIRIS_EVTCHANGESEG_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTMEMRESTORE, EventMemRestore32, OSIRIS_EVTMEMRESTORE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTTIMERCANCEL, EventTimerCancel32, OSIRIS_EVTTIMERCANCEL_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTCHILDDIED, EventChildDied32, OSIRIS_EVTCHILDDIED_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTMATCENCREATE, EventMatcenCreate32, OSIRIS_EVTMATCENCREATE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTLEVELGOALCOMPLETE, EventLevelGoalComplete32, OSIRIS_EVTLEVELGOALCOMPLETE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTLEVELGOALITEMCOMPLETE, EventLevelGoalItemComplete32, OSIRIS_EVTLEVELGOALITEMCOMPLETE_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTPLAYERRESPAWN, EventPlayerRespawn32, OSIRIS_EVTPLAYERRESPAWN_FIELDS)
	DECLARE_32_STRUCT(tOSIRISEVTPLAYERDIES, EventPlayerDies32, OSIRIS_EVTPLAYERDIES_FIELDS)

#undef DECLARE_32_STRUCT_FIELD
#undef DECLARE_32_STRUCT

#if 0
	static_assert(sizeof(EventInterval32) == sizeof(tOSIRISEVTINTERVAL32), "Event interval ABI mismatch");
	static_assert(sizeof(EventDamaged32) == sizeof(tOSIRISEVTDAMAGED32), "Event damaged ABI mismatch");
	static_assert(sizeof(EventCollide32) == sizeof(tOSIRISEVTCOLLIDE32), "Event collide ABI mismatch");
	static_assert(sizeof(EventDestroy32) == sizeof(tOSIRISEVTDESTROY32), "Event destroy ABI mismatch");
	static_assert(sizeof(EventTimer32) == sizeof(tOSIRISEVTTIMER32), "Event timer ABI mismatch");
	static_assert(sizeof(EventUse32) == sizeof(tOSIRISEVTUSE32), "Event use ABI mismatch");
	static_assert(sizeof(EventSaveState32) == sizeof(tOSIRISEVTSAVESTATE32), "Event save-state ABI mismatch");
	static_assert(sizeof(EventRestoreState32) == sizeof(tOSIRISEVTRESTORESTATE32), "Event restore-state ABI mismatch");
	static_assert(sizeof(EventAINotify32) == sizeof(tOSIRISEVTAINOTIFY32), "Event AI notify ABI mismatch");
	static_assert(sizeof(EventChangeSeg32) == sizeof(tOSIRISEVTCHANGESEG32), "Event change-seg ABI mismatch");
	static_assert(sizeof(EventMemRestore32) == sizeof(tOSIRISEVTMEMRESTORE32), "Event mem-restore ABI mismatch");
	static_assert(sizeof(EventTimerCancel32) == sizeof(tOSIRISEVTTIMERCANCEL32), "Event timercancel ABI mismatch");
	static_assert(sizeof(EventChildDied32) == sizeof(tOSIRISEVTCHILDDIED32), "Event child-died ABI mismatch");
	static_assert(sizeof(EventMatcenCreate32) == sizeof(tOSIRISEVTMATCENCREATE32), "Event matcen-create ABI mismatch");
	static_assert(sizeof(EventLevelGoalComplete32) == sizeof(tOSIRISEVTLEVELGOALCOMPLETE32), "Event level-goal-complete ABI mismatch");
	static_assert(sizeof(EventLevelGoalItemComplete32) == sizeof(tOSIRISEVTLEVELGOALITEMCOMPLETE32), "Event level-goal-item-complete ABI mismatch");
	static_assert(sizeof(EventPlayerRespawn32) == sizeof(tOSIRISEVTPLAYERRESPAWN32), "Event player-respawn ABI mismatch");
	static_assert(sizeof(EventPlayerDies32) == sizeof(tOSIRISEVTPLAYERDIES32), "Event player-dies ABI mismatch");
#endif

#define DEFINE_ENCODER(native_type, generated_type, fields_macro) \
	void encode_##generated_type(const native_type& src, generated_type& dst, const VmPtrEncoder& vm) \
	{ \
		memset(&dst, 0, sizeof(dst)); \
		fields_macro(ENCODE_STRUCT_FIELD) \
	}

#define ENCODE_STRUCT_FIELD(kind, name) ABI_COPY_##kind(name)

	DEFINE_ENCODER(tOSIRISEVTINTERVAL, EventInterval32, OSIRIS_EVTINTERVAL_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTDAMAGED, EventDamaged32, OSIRIS_EVTDAMAGED_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTCOLLIDE, EventCollide32, OSIRIS_EVTCOLLIDE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTDESTROY, EventDestroy32, OSIRIS_EVTDESTROY_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTTIMER, EventTimer32, OSIRIS_EVTTIMER_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTUSE, EventUse32, OSIRIS_EVTUSE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTSAVESTATE, EventSaveState32, OSIRIS_EVTSAVESTATE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTRESTORESTATE, EventRestoreState32, OSIRIS_EVTRESTORESTATE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTAINOTIFY, EventAINotify32, OSIRIS_EVTAINOTIFY_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTCHANGESEG, EventChangeSeg32, OSIRIS_EVTCHANGESEG_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTMEMRESTORE, EventMemRestore32, OSIRIS_EVTMEMRESTORE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTTIMERCANCEL, EventTimerCancel32, OSIRIS_EVTTIMERCANCEL_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTCHILDDIED, EventChildDied32, OSIRIS_EVTCHILDDIED_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTMATCENCREATE, EventMatcenCreate32, OSIRIS_EVTMATCENCREATE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTLEVELGOALCOMPLETE, EventLevelGoalComplete32, OSIRIS_EVTLEVELGOALCOMPLETE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTLEVELGOALITEMCOMPLETE, EventLevelGoalItemComplete32, OSIRIS_EVTLEVELGOALITEMCOMPLETE_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTPLAYERRESPAWN, EventPlayerRespawn32, OSIRIS_EVTPLAYERRESPAWN_FIELDS)
	DEFINE_ENCODER(tOSIRISEVTPLAYERDIES, EventPlayerDies32, OSIRIS_EVTPLAYERDIES_FIELDS)

#undef ENCODE_STRUCT_FIELD
#undef DEFINE_ENCODER

	template <typename DstEvent32, typename SrcEvent64, typename GeneratedEvent32>
	void encode_union_event(DstEvent32& dst, const SrcEvent64& src, const VmPtrEncoder& vm, void (*encode_fn)(const SrcEvent64&, GeneratedEvent32&, const VmPtrEncoder&))
	{
		GeneratedEvent32 encoded;
		encode_fn(src, encoded, vm);
		memcpy(&dst, &encoded, sizeof(encoded));
	}

#undef ABI_COPY_I32
#undef ABI_COPY_U8
#undef ABI_COPY_F32
#undef ABI_COPY_PTR32
#undef ABI_DECL_I32
#undef ABI_DECL_U8
#undef ABI_DECL_F32
#undef ABI_DECL_PTR32
#undef OSIRIS_EVTINTERVAL_FIELDS
#undef OSIRIS_EVTDAMAGED_FIELDS
#undef OSIRIS_EVTCOLLIDE_FIELDS
#undef OSIRIS_EVTDESTROY_FIELDS
#undef OSIRIS_EVTTIMER_FIELDS
#undef OSIRIS_EVTUSE_FIELDS
#undef OSIRIS_EVTSAVESTATE_FIELDS
#undef OSIRIS_EVTRESTORESTATE_FIELDS
#undef OSIRIS_EVTAINOTIFY_FIELDS
#undef OSIRIS_EVTCHANGESEG_FIELDS
#undef OSIRIS_EVTMEMRESTORE_FIELDS
#undef OSIRIS_EVTTIMERCANCEL_FIELDS
#undef OSIRIS_EVTCHILDDIED_FIELDS
#undef OSIRIS_EVTMATCENCREATE_FIELDS
#undef OSIRIS_EVTLEVELGOALCOMPLETE_FIELDS
#undef OSIRIS_EVTLEVELGOALITEMCOMPLETE_FIELDS
#undef OSIRIS_EVTPLAYERRESPAWN_FIELDS
#undef OSIRIS_EVTPLAYERDIES_FIELDS
}

void encode_event_info(int event, const tOSIRISEventInfo& src, tOSIRISEventInfo32& dst, const VmPtrEncoder& vm)
{
	memset(&dst, 0, sizeof(dst));
	dst.me_handle = src.me_handle;
	dst.extra_info = vm.encode_ptr32(src.extra_info);

	switch (event)
	{
	case EVT_INTERVAL:
		encode_union_event<tOSIRISEVTINTERVAL32, tOSIRISEVTINTERVAL, EventInterval32>(dst.evt_interval, src.evt_interval, vm, encode_EventInterval32);
		break;
	case EVT_AI_FRAME:
	case EVT_CREATED:
	case EVT_AI_INIT:
	case EVT_DOOR_ACTIVATE:
	case EVT_DOOR_CLOSE:
	case EVT_ALL_LEVEL_GOALS_COMPLETE:
	case EVT_PLAYER_MOVIE_START:
	case EVT_PLAYER_MOVIE_END:
		break;
	case EVT_DAMAGED:
		encode_union_event<tOSIRISEVTDAMAGED32, tOSIRISEVTDAMAGED, EventDamaged32>(dst.evt_damaged, src.evt_damaged, vm, encode_EventDamaged32);
		break;
	case EVT_COLLIDE:
		encode_union_event<tOSIRISEVTCOLLIDE32, tOSIRISEVTCOLLIDE, EventCollide32>(dst.evt_collide, src.evt_collide, vm, encode_EventCollide32);
		break;
	case EVT_DESTROY:
		encode_union_event<tOSIRISEVTDESTROY32, tOSIRISEVTDESTROY, EventDestroy32>(dst.evt_destroy, src.evt_destroy, vm, encode_EventDestroy32);
		break;
	case EVT_TIMER:
		encode_union_event<tOSIRISEVTTIMER32, tOSIRISEVTTIMER, EventTimer32>(dst.evt_timer, src.evt_timer, vm, encode_EventTimer32);
		break;
	case EVT_USE:
		encode_union_event<tOSIRISEVTUSE32, tOSIRISEVTUSE, EventUse32>(dst.evt_use, src.evt_use, vm, encode_EventUse32);
		break;
	case EVT_AI_NOTIFY:
	case EVT_AIN_OBJKILLED:
	case EVT_AIN_SEEPLAYER:
	case EVT_AIN_WHITOBJECT:
	case EVT_AIN_GOALCOMPLETE:
	case EVT_AIN_GOALFAIL:
	case EVT_AIN_MELEE_HIT:
	case EVT_AIN_MELEE_ATTACK_FRAME:
	case EVT_AIN_MOVIE_START:
	case EVT_AIN_MOVIE_END:
		encode_union_event<tOSIRISEVTAINOTIFY32, tOSIRISEVTAINOTIFY, EventAINotify32>(dst.evt_ai_notify, src.evt_ai_notify, vm, encode_EventAINotify32);
		break;
	case EVT_CHANGESEG:
		encode_union_event<tOSIRISEVTCHANGESEG32, tOSIRISEVTCHANGESEG, EventChangeSeg32>(dst.evt_changeseg, src.evt_changeseg, vm, encode_EventChangeSeg32);
		break;
	case EVT_SAVESTATE:
		encode_union_event<tOSIRISEVTSAVESTATE32, tOSIRISEVTSAVESTATE, EventSaveState32>(dst.evt_savestate, src.evt_savestate, vm, encode_EventSaveState32);
		break;
	case EVT_RESTORESTATE:
		encode_union_event<tOSIRISEVTRESTORESTATE32, tOSIRISEVTRESTORESTATE, EventRestoreState32>(dst.evt_restorestate, src.evt_restorestate, vm, encode_EventRestoreState32);
		break;
	case EVT_MEMRESTORE:
		encode_union_event<tOSIRISEVTMEMRESTORE32, tOSIRISEVTMEMRESTORE, EventMemRestore32>(dst.evt_memrestore, src.evt_memrestore, vm, encode_EventMemRestore32);
		break;
	case EVT_TIMERCANCEL:
		encode_union_event<tOSIRISEVTTIMERCANCEL32, tOSIRISEVTTIMERCANCEL, EventTimerCancel32>(dst.evt_timercancel, src.evt_timercancel, vm, encode_EventTimerCancel32);
		break;
	case EVT_CHILD_DIED:
		encode_union_event<tOSIRISEVTCHILDDIED32, tOSIRISEVTCHILDDIED, EventChildDied32>(dst.evt_child_died, src.evt_child_died, vm, encode_EventChildDied32);
		break;
	case EVT_MATCEN_CREATE:
		encode_union_event<tOSIRISEVTMATCENCREATE32, tOSIRISEVTMATCENCREATE, EventMatcenCreate32>(dst.evt_matcen_create, src.evt_matcen_create, vm, encode_EventMatcenCreate32);
		break;
	case EVT_LEVEL_GOAL_COMPLETE:
		encode_union_event<tOSIRISEVTLEVELGOALCOMPLETE32, tOSIRISEVTLEVELGOALCOMPLETE, EventLevelGoalComplete32>(dst.evt_level_goal_complete, src.evt_level_goal_complete, vm, encode_EventLevelGoalComplete32);
		break;
	case EVT_LEVEL_GOAL_ITEM_COMPLETE:
		encode_union_event<tOSIRISEVTLEVELGOALITEMCOMPLETE32, tOSIRISEVTLEVELGOALITEMCOMPLETE, EventLevelGoalItemComplete32>(dst.evt_level_goal_item_complete, src.evt_level_goal_item_complete, vm, encode_EventLevelGoalItemComplete32);
		break;
	case EVT_PLAYER_RESPAWN:
		encode_union_event<tOSIRISEVTPLAYERRESPAWN32, tOSIRISEVTPLAYERRESPAWN, EventPlayerRespawn32>(dst.evt_player_respawn, src.evt_player_respawn, vm, encode_EventPlayerRespawn32);
		break;
	case EVT_PLAYER_DIES:
		encode_union_event<tOSIRISEVTPLAYERDIES32, tOSIRISEVTPLAYERDIES, EventPlayerDies32>(dst.evt_player_dies, src.evt_player_dies, vm, encode_EventPlayerDies32);
		break;
	default:
		break;
	}
}

}
