#include "module.h"
#include "osimod.h"

#ifdef HANDLE
#undef HANDLE
#endif
#include "emu86.h"
#include "heap.h"
#include "emuabi/cfile_wrappers.h"
#include "emuabi/eventinfo.h"
#include "emuabi/msafe.h"
#include "emuabi/osiris_import_bridge.h"
#include "emuabi/osiris_import_registry.h"
#include "emuabi/osiris_module_init.h"
#include "multisafe.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#if defined(__LINUX__)
typedef char DLLFUNCCALL(*InitializeDLL_fp)(tOSIRISModuleInit* function_list);
typedef void DLLFUNCCALL(*ShutdownDLL_fp)(void);
typedef int  DLLFUNCCALL(*GetGOScriptID_fp)(char* name, ubyte isdoor);
typedef void DLLFUNCCALL* (*CreateInstance_fp)(int id);
typedef void DLLFUNCCALL(*DestroyInstance_fp)(int id, void* ptr);
typedef short DLLFUNCCALL(*CallInstanceEvent_fp)(int id, void* ptr, int event, tOSIRISEventInfo* data);
typedef int  DLLFUNCCALL(*GetTriggerScriptID_fp)(int trigger_room, int trigger_face);
typedef int  DLLFUNCCALL(*GetCOScriptList_fp)(int** list, int** id_list);
typedef int  DLLFUNCCALL(*SaveRestoreState_fp)(void* file_ptr, ubyte saving_state);
#else
typedef char(DLLFUNCCALL* InitializeDLL_fp)(tOSIRISModuleInit* function_list);
typedef void(DLLFUNCCALL* ShutdownDLL_fp)(void);
typedef int (DLLFUNCCALL* GetGOScriptID_fp)(char* name, ubyte isdoor);
typedef void* (DLLFUNCCALL* CreateInstance_fp)(int id);
typedef void(DLLFUNCCALL* DestroyInstance_fp)(int id, void* ptr);
typedef short(DLLFUNCCALL* CallInstanceEvent_fp)(int id, void* ptr, int event, tOSIRISEventInfo* data);
typedef int (DLLFUNCCALL* GetTriggerScriptID_fp)(int trigger_room, int trigger_face);
typedef int (DLLFUNCCALL* GetCOScriptList_fp)(int** list, int** id_list);
typedef int (DLLFUNCCALL* SaveRestoreState_fp)(void* file_ptr, ubyte saving_state);
#endif

enum osimod_backend
{
	OSIMOD_BACKEND_NATIVE,
	OSIMOD_BACKEND_VM
};

struct osimod_t
{
	osimod_backend backend;
	module mod;

	InitializeDLL_fp InitializeDLL;
	ShutdownDLL_fp ShutdownDLL;
	GetGOScriptID_fp GetGOScriptID;
	CreateInstance_fp CreateInstance;
	DestroyInstance_fp DestroyInstance;
	CallInstanceEvent_fp CallInstanceEvent;
	GetTriggerScriptID_fp GetTriggerScriptID;
	GetCOScriptList_fp GetCOScriptList;
	SaveRestoreState_fp SaveRestoreState;

	Emu* vm;
	emu_ptr_t vm_handle;
	emu_ptr_t vm_initialize_dll;
	emu_ptr_t vm_shutdown_dll;
	emu_ptr_t vm_get_goscript_id;
	emu_ptr_t vm_create_instance;
	emu_ptr_t vm_destroy_instance;
	emu_ptr_t vm_call_instance_event;
	emu_ptr_t vm_get_trigger_script_id;
	emu_ptr_t vm_get_co_script_list;
	emu_ptr_t vm_save_restore_state;
	std::vector<emu_ptr_t> import_thunks;
};

namespace
{
static bool ends_with_dll(const char* path)
{
	if (!path)
		return false;

	std::string lower(path);
	for (size_t i = 0; i < lower.size(); ++i)
		lower[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(lower[i])));

	return lower.size() >= 4 && lower.compare(lower.size() - 4, 4, ".dll") == 0;
}

static emu_ptr_t vm_temp_alloc(Emu* vm, size_t size)
{
	if (!vm || !size)
		return 0;

	return heap_alloc(vm, vm->process_heap, 0, static_cast<emu_ptr_t>(size));
}

static void vm_temp_free(Emu* vm, emu_ptr_t ptr)
{
	if (!vm || !ptr)
		return;

	heap_free(vm, vm->process_heap, 0, ptr);
}

static void* vm_to_host_ptr(Emu* vm, emu_ptr_t ptr)
{
	if (!vm || !ptr || ptr >= vm->as.size)
		return 0;
	return vm->as.base + ptr;
}

template <typename Fn>
static void bridge_msafe_struct(Emu86FunCtx& ctx, int type, emu_ptr_t guest_struct_ptr, Fn&& fn)
{
	msafe_struct mstruct;
	void* host_struct_ptr = vm_to_host_ptr(ctx.emu86, guest_struct_ptr);
	emuabi::VmPtrDecoder vm = { ctx.emu86 ? ctx.emu86->as.base : nullptr };
	emuabi::decode_msafe_struct(type, host_struct_ptr, mstruct, vm);
	fn(mstruct);

	if (host_struct_ptr && ctx.emu86 && ctx.emu86->as.base)
	{
		emuabi::VmPtrEncoder encoder = { ctx.emu86->as.base };
		emuabi::encode_msafe_struct(type, mstruct, host_struct_ptr, encoder);
	}
}

static void emucall_msafe_CallFunction(Emu86FunCtx& ctx, void *)
{
	const int type = ctx.arg<int>(0);
	const emu_ptr_t guest_struct_ptr = ctx.arg<emu_ptr_t>(1);

	msafe_struct mstruct;
	void* host_struct_ptr = vm_to_host_ptr(ctx.emu86, guest_struct_ptr);
	emuabi::VmPtrDecoder vm = { ctx.emu86 ? ctx.emu86->as.base : nullptr };
	emuabi::decode_msafe_struct(type, host_struct_ptr, mstruct, vm);

	msafe_CallFunction(static_cast<ubyte>(type), &mstruct);
}

static void emucall_msafe_GetValue(Emu86FunCtx& ctx, void *)
{
	const int type = ctx.arg<int>(0);
	const emu_ptr_t guest_struct_ptr = ctx.arg<emu_ptr_t>(1);

	msafe_struct mstruct;
	void* host_struct_ptr = vm_to_host_ptr(ctx.emu86, guest_struct_ptr);
	emuabi::VmPtrDecoder vm = { ctx.emu86 ? ctx.emu86->as.base : nullptr };
	emuabi::decode_msafe_struct(type, host_struct_ptr, mstruct, vm);

	msafe_GetValue(type, &mstruct);

	emuabi::VmPtrEncoder encoder = { ctx.emu86->as.base };
	emuabi::encode_msafe_struct(type, mstruct, host_struct_ptr, encoder);
}

static void emucall_msafe_DoPowerup(Emu86FunCtx& ctx, void *)
{
	const emu_ptr_t guest_struct_ptr = ctx.arg<emu_ptr_t>(0);
	msafe_struct mstruct;
	void* host_struct_ptr = vm_to_host_ptr(ctx.emu86, guest_struct_ptr);
	emuabi::VmPtrDecoder vm = { ctx.emu86 ? ctx.emu86->as.base : nullptr };
	emuabi::decode_msafe_do_powerup_struct(host_struct_ptr, mstruct, vm);
	msafe_DoPowerup(&mstruct);
	/*
	if (host_struct_ptr && ctx.emu86 && ctx.emu86->as.base)
	{
		emuabi::VmPtrEncoder encoder = { ctx.emu86->as.base };
		emuabi::encode_msafe_do_powerup_struct(mstruct, host_struct_ptr, encoder);
	}
	*/
}

static void emucall_Osiris_CreateTimer(Emu86FunCtx& ctx, void *)
{
	const emu_ptr_t guest_timer_ptr = ctx.arg<emu_ptr_t>(0);
	tOSIRISTIMER timer;
	void* host_timer_ptr = vm_to_host_ptr(ctx.emu86, guest_timer_ptr);
	emuabi::decode_osiris_timer_struct(host_timer_ptr, timer);
	ctx.set_return(Osiris_CreateTimer(&timer));
}

static emu_ptr_t find_export_variant(Emu* vm, emu_ptr_t module_handle, const char* name, unsigned arg_bytes)
{
	if (!vm || !module_handle || !name)
		return 0;

	char decorated1[256];
	char decorated2[256];
	std::snprintf(decorated1, sizeof(decorated1), "_%s@%u", name, arg_bytes);
	std::snprintf(decorated2, sizeof(decorated2), "%s@%u", name, arg_bytes);

	const char* candidates[] = {
		name,
		decorated1,
		decorated2,
		0
	};

	for (size_t i = 0; candidates[i]; ++i)
	{
		emu_ptr_t proc = emu86_get_proc_address(vm, module_handle, candidates[i]);
		if (proc)
			return proc;
	}

	return 0;
}

static bool resolve_vm_exports(osimod_t* om, osimod_type type)
{
	const struct
	{
		const char* name;
		emu_ptr_t* slot;
		unsigned arg_bytes;
		bool required;
	} kExports[] = {
		{"InitializeDLL", &om->vm_initialize_dll, 4, type != OMT_MISSION},
		{"ShutdownDLL", &om->vm_shutdown_dll, 0, type != OMT_MISSION},
		{"GetGOScriptID", &om->vm_get_goscript_id, 8, true},
		{"CreateInstance", &om->vm_create_instance, 4, true},
		{"DestroyInstance", &om->vm_destroy_instance, 8, true},
		{"CallInstanceEvent", &om->vm_call_instance_event, 16, true},
		{"GetTriggerScriptID", &om->vm_get_trigger_script_id, 8, type == OMT_LEVEL},
		{"GetCOScriptList", &om->vm_get_co_script_list, 8, type == OMT_LEVEL},
		{"SaveRestoreState", &om->vm_save_restore_state, 8, true},
	};

	bool ok = true;
	for (size_t i = 0; i < sizeof(kExports) / sizeof(kExports[0]); ++i)
	{
		*kExports[i].slot = find_export_variant(om->vm, om->vm_handle, kExports[i].name, kExports[i].arg_bytes);
		if (!*kExports[i].slot && kExports[i].required)
		{
			std::fprintf(stderr, "OSIMOD: missing required export %s\n", kExports[i].name);
			ok = false;
		}
	}

	return ok;
}

static bool load_vm_backend(osimod_t* om, const char* fullpath, osimod_type type)
{
	emu86_init();
	om->vm = emu86_create();
	if (!om->vm)
	{
		std::fprintf(stderr, "OSIMOD: failed to create emu86 VM for %s\n", fullpath ? fullpath : "(null)");
		return false;
	}

	om->vm_handle = emu86_load_library(om->vm, fullpath);
	if (!om->vm_handle)
	{
		std::fprintf(stderr, "OSIMOD: failed to load guest DLL %s\n", fullpath ? fullpath : "(null)");
		emu86_free(om->vm);
		om->vm = 0;
		return false;
	}

	if (!resolve_vm_exports(om, type))
	{
		emu86_free(om->vm);
		om->vm = 0;
		om->vm_handle = 0;
		return false;
	}

	om->import_thunks.assign(emuabi::osiris_import_spec_count(), 0);
	std::vector<std::string> missing;
	if (!emuabi::Osiris_BuildImportThunkTable(om->vm, om->import_thunks.data(), om->import_thunks.size(), &missing))
	{
		emu86_free(om->vm);
		om->vm = 0;
		om->vm_handle = 0;
		return false;
	}

	if (!missing.empty())
	{
		std::fprintf(stderr, "OSIMOD: %zu guest import(s) are unsupported by the current bridge\n", missing.size());
		for (size_t i = 0; i < missing.size(); ++i)
			std::fprintf(stderr, "OSIMOD: unsupported import %s\n", missing[i].c_str());
	}

	om->backend = OSIMOD_BACKEND_VM;
	return true;
}

static bool load_native_backend(osimod_t* om, module* mod, osimod_type type)
{
	om->backend = OSIMOD_BACKEND_NATIVE;
	om->mod = *mod;

	om->InitializeDLL = type != OMT_MISSION ? (InitializeDLL_fp)mod_GetSymbol(mod, (char*)"InitializeDLL", 4) : NULL;
	om->ShutdownDLL = type != OMT_MISSION ? (ShutdownDLL_fp)mod_GetSymbol(mod, (char*)"ShutdownDLL", 0) : NULL;
	om->GetGOScriptID = (GetGOScriptID_fp)mod_GetSymbol(mod, (char*)"GetGOScriptID", 8);
	om->GetTriggerScriptID = type == OMT_LEVEL ? (GetTriggerScriptID_fp)mod_GetSymbol(mod, (char*)"GetTriggerScriptID", 8) : NULL;
	om->GetCOScriptList = type == OMT_LEVEL ? (GetCOScriptList_fp)mod_GetSymbol(mod, (char*)"GetCOScriptList", 8) : NULL;
	om->CreateInstance = (CreateInstance_fp)mod_GetSymbol(mod, (char*)"CreateInstance", 4);
	om->DestroyInstance = (DestroyInstance_fp)mod_GetSymbol(mod, (char*)"DestroyInstance", 8);
	om->CallInstanceEvent = (CallInstanceEvent_fp)mod_GetSymbol(mod, (char*)"CallInstanceEvent", 16);
	om->SaveRestoreState = (SaveRestoreState_fp)mod_GetSymbol(mod, (char*)"SaveRestoreState", 8);

	if ((type != OMT_MISSION && !om->InitializeDLL) ||
		(type != OMT_MISSION && !om->ShutdownDLL) ||
		!om->GetGOScriptID ||
		(type == OMT_LEVEL && !om->GetTriggerScriptID) ||
		(type == OMT_LEVEL && !om->GetCOScriptList) ||
		!om->CreateInstance ||
		!om->DestroyInstance ||
		!om->SaveRestoreState ||
		!om->CallInstanceEvent)
	{
		return false;
	}

	return true;
}

static void clear_vm_backend(osimod_t* om)
{
	if (!om)
		return;

	if (om->vm)
	{
		emu86_free(om->vm);
		om->vm = 0;
	}

	om->vm_handle = 0;
	om->vm_initialize_dll = 0;
	om->vm_shutdown_dll = 0;
	om->vm_get_goscript_id = 0;
	om->vm_create_instance = 0;
	om->vm_destroy_instance = 0;
	om->vm_call_instance_event = 0;
	om->vm_get_trigger_script_id = 0;
	om->vm_get_co_script_list = 0;
	om->vm_save_restore_state = 0;
	om->import_thunks.clear();
}
}

namespace emuabi
{
const emu86_ctx_fun_t kOsirisBridgeFuns[] = {
	{"msafe_CallFunction", emucall_msafe_CallFunction, 2, nullptr},
	{"msafe_GetValue", emucall_msafe_GetValue, 2, nullptr},
	{"msafe_DoPowerup", emucall_msafe_DoPowerup, 1, nullptr},
	{"Osiris_CreateTimer", emucall_Osiris_CreateTimer, 1, nullptr},
};

const size_t kOsirisBridgeFunsCount = sizeof(kOsirisBridgeFuns) / sizeof(kOsirisBridgeFuns[0]);
}

void osimod_init(osimod_t *om)
{
	if (!om)
		return;

	om->backend = OSIMOD_BACKEND_NATIVE;
	om->mod.handle = 0;
	om->InitializeDLL = 0;
	om->ShutdownDLL = 0;
	om->GetGOScriptID = 0;
	om->CreateInstance = 0;
	om->DestroyInstance = 0;
	om->CallInstanceEvent = 0;
	om->GetTriggerScriptID = 0;
	om->GetCOScriptList = 0;
	om->SaveRestoreState = 0;
	om->vm = 0;
	om->vm_handle = 0;
	om->vm_initialize_dll = 0;
	om->vm_shutdown_dll = 0;
	om->vm_get_goscript_id = 0;
	om->vm_create_instance = 0;
	om->vm_destroy_instance = 0;
	om->vm_call_instance_event = 0;
	om->vm_get_trigger_script_id = 0;
	om->vm_get_co_script_list = 0;
	om->vm_save_restore_state = 0;
	om->import_thunks.clear();
}

char osimod_InitializeDLL(osimod_t *om, tOSIRISModuleInit* function_list)
{
	if (!om || !function_list)
		return 0;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
		return om->InitializeDLL(function_list);

	emuabi::OsirisModuleInit32 encoded;
	if (!emuabi::encode_osiris_module_init(om->vm, *function_list, om->import_thunks.data(), om->import_thunks.size(), encoded))
		return 0;

	emu_ptr_t guest = vm_temp_alloc(om->vm, sizeof(encoded));
	if (!guest)
		return 0;

	encoded.game_checksum = 2273873307UL; // 32-bit checksum

	std::memcpy(om->vm->as.base + guest, &encoded, sizeof(encoded));
	const unsigned ret = emu86fun_call(om->vm, om->vm_initialize_dll, 1, guest);
	vm_temp_free(om->vm, guest);
	return static_cast<char>(ret);
}

void osimod_ShutdownDLL(osimod_t *om)
{
	if (!om)
		return;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
	{
		if (om->ShutdownDLL)
			om->ShutdownDLL();
		return;
	}

	if (om->vm && om->vm_shutdown_dll)
		emu86fun_call(om->vm, om->vm_shutdown_dll, 0);
}

int osimod_GetGOScriptID(osimod_t *om, char* name, ubyte isdoor)
{
	if (!om || !name)
		return -1;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
		return om->GetGOScriptID(name, isdoor);

	emu_ptr_t guest_name = emuabi::copy_string_to_vm(om->vm, name);
	if (!guest_name)
		return -1;

	const unsigned ret = emu86fun_call(om->vm, om->vm_get_goscript_id, 2, guest_name, static_cast<unsigned>(isdoor));
	vm_temp_free(om->vm, guest_name);
	return static_cast<int>(ret);
}

void *osimod_CreateInstance(osimod_t *om, int id)
{
	if (!om)
		return 0;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
		return om->CreateInstance(id);

	const unsigned ret = emu86fun_call(om->vm, om->vm_create_instance, 1, static_cast<unsigned>(id));
	return reinterpret_cast<void*>(static_cast<uintptr_t>(ret));
}

void osimod_DestroyInstance(osimod_t *om, int id, void* ptr)
{
	if (!om)
		return;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
	{
		om->DestroyInstance(id, ptr);
		return;
	}

	emu86fun_call(om->vm, om->vm_destroy_instance, 2, static_cast<unsigned>(id), static_cast<unsigned>(reinterpret_cast<uintptr_t>(ptr)));
}

short osimod_CallInstanceEvent(osimod_t *om, int id, void* ptr, int event, tOSIRISEventInfo* data)
{
	if (!om)
		return 0;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
		return om->CallInstanceEvent(id, ptr, event, data);

	emu_ptr_t guest_event = vm_temp_alloc(om->vm, emuabi::event_info_size_32);
	if (!guest_event)
		return 0;

	emuabi::VmPtrEncoder vm = { om->vm->as.base };
	tOSIRISEventInfo encoded;
	std::memset(&encoded, 0, sizeof(encoded));
	if (data)
		encoded = *data;
	if (event == EVT_SAVESTATE)
		encoded.evt_savestate.fileptr = 0;
	else if (event == EVT_RESTORESTATE)
		encoded.evt_restorestate.fileptr = 0;

	emuabi::encode_event_info_with_file_handle(event, encoded, om->vm->as.base + guest_event, vm,
		(data && (event == EVT_SAVESTATE || event == EVT_RESTORESTATE)) ? emuabi::register_file_handle(data->evt_savestate.fileptr) : 0);

	const unsigned ret = emu86fun_call(om->vm, om->vm_call_instance_event, 4,
		static_cast<unsigned>(id),
		static_cast<unsigned>(reinterpret_cast<uintptr_t>(ptr)),
		static_cast<unsigned>(event),
		guest_event);
	vm_temp_free(om->vm, guest_event);
	return static_cast<short>(ret);
}

int osimod_GetTriggerScriptID(osimod_t *om, int trigger_room, int trigger_face)
{
	if (!om)
		return -1;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
	{
		if (!om->GetTriggerScriptID)
			return -1;
		return om->GetTriggerScriptID(trigger_room, trigger_face);
	}

	if (!om->vm_get_trigger_script_id)
		return -1;

	const unsigned ret = emu86fun_call(om->vm, om->vm_get_trigger_script_id, 2,
		static_cast<unsigned>(trigger_room),
		static_cast<unsigned>(trigger_face));
	return static_cast<int>(ret);
}

int osimod_GetCOScriptList(osimod_t *om, int** list, int** id_list)
{
	if (!om || !list || !id_list)
		return 0;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
		return om->GetCOScriptList(list, id_list);

	if (!om->vm_get_co_script_list)
		return 0;

	emu_ptr_t list_slot = vm_temp_alloc(om->vm, sizeof(uint32_t));
	emu_ptr_t id_slot = vm_temp_alloc(om->vm, sizeof(uint32_t));
	if (!list_slot || !id_slot)
	{
		vm_temp_free(om->vm, list_slot);
		vm_temp_free(om->vm, id_slot);
		return 0;
	}

	std::memset(om->vm->as.base + list_slot, 0, sizeof(uint32_t));
	std::memset(om->vm->as.base + id_slot, 0, sizeof(uint32_t));
	const unsigned ret = emu86fun_call(om->vm, om->vm_get_co_script_list, 2, list_slot, id_slot);
	const emu_ptr_t guest_list = *reinterpret_cast<uint32_t*>(om->vm->as.base + list_slot);
	const emu_ptr_t guest_id_list = *reinterpret_cast<uint32_t*>(om->vm->as.base + id_slot);

	*list = static_cast<int*>(vm_to_host_ptr(om->vm, guest_list));
	*id_list = static_cast<int*>(vm_to_host_ptr(om->vm, guest_id_list));

	vm_temp_free(om->vm, list_slot);
	vm_temp_free(om->vm, id_slot);
	return static_cast<int>(ret);
}

int osimod_SaveRestoreState(osimod_t *om, void* file_ptr, ubyte saving_state)
{
	if (!om)
		return 0;

	if (om->backend == OSIMOD_BACKEND_NATIVE)
		return om->SaveRestoreState(file_ptr, saving_state);

	if (!om->vm_save_restore_state)
		return 0;

	const emu_ptr_t guest_file = emuabi::register_file_handle(file_ptr);
	const unsigned ret = emu86fun_call(om->vm, om->vm_save_restore_state, 2, guest_file, static_cast<unsigned>(saving_state));
	return static_cast<int>(ret);
}

int osimod_load_mod(osimod_t **ret, module *mod, osimod_type type) {
	if (!ret || !mod)
		return 0;

	osimod_t *om = new osimod_t();
	osimod_init(om);
	if (!load_native_backend(om, mod, type))
	{
		delete om;
		return 0;
	}

	*ret = om;
	return 1;
}

int osimod_load(osimod_t **ret, char *fullpath, osimod_type type) {
	if (!ret || !fullpath)
		return 0;

	if (0 && !ends_with_dll(fullpath))
	{
		module mod;
		if (!mod_LoadModule(&mod, fullpath))
			return 0;

		int rc = osimod_load_mod(ret, &mod, type);
		if (!rc)
			mod_FreeModule(&mod);
		return rc;
	}

	osimod_t *om = new osimod_t();
	osimod_init(om);
	if (!load_vm_backend(om, fullpath, type))
	{
		delete om;
		return 0;
	}

	*ret = om;
	return 1;
}

void osimod_free(osimod_t **om_ptr) {
	if (!om_ptr || !*om_ptr)
		return;

	osimod_t *om = *om_ptr;
	if (om->backend == OSIMOD_BACKEND_NATIVE)
	{
		mod_FreeModule(&om->mod);
	}
	else
	{
		clear_vm_backend(om);
	}

	delete om;
	*om_ptr = NULL;
}
