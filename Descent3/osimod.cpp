#include "module.h"
#include "osimod.h"

// The exported DLL function call prototypes
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
struct osimod_t {
	InitializeDLL_fp		InitializeDLL;
	ShutdownDLL_fp			ShutdownDLL;
	GetGOScriptID_fp		GetGOScriptID;
	CreateInstance_fp		CreateInstance;
	DestroyInstance_fp		DestroyInstance;
	CallInstanceEvent_fp	CallInstanceEvent;
	GetTriggerScriptID_fp	GetTriggerScriptID;
	GetCOScriptList_fp		GetCOScriptList;
	SaveRestoreState_fp		SaveRestoreState;
	module					mod;
};

void osimod_init(osimod_t *om)
{
        om->CallInstanceEvent = NULL;
        om->CreateInstance = NULL;
        om->DestroyInstance = NULL;
        om->GetCOScriptList = NULL;
        om->GetGOScriptID = NULL;
        om->GetTriggerScriptID = NULL;
        om->InitializeDLL = NULL;
        om->SaveRestoreState = NULL;
}

char osimod_InitializeDLL(osimod_t *om, tOSIRISModuleInit* function_list)
{
	return om->InitializeDLL(function_list);
}

void osimod_ShutdownDLL(osimod_t *om)
{
	om->ShutdownDLL();
}

int osimod_GetGOScriptID(osimod_t *om, char* name, ubyte isdoor)
{
	return om->GetGOScriptID(name, isdoor);
}

void *osimod_CreateInstance(osimod_t *om, int id)
{
	return om->CreateInstance(id);
}

void osimod_DestroyInstance(osimod_t *om, int id, void* ptr)
{
	om->DestroyInstance(id, ptr);
}

short osimod_CallInstanceEvent(osimod_t *om, int id, void* ptr, int event, tOSIRISEventInfo* data)
{
	return om->CallInstanceEvent(id, ptr, event, data);
}

int osimod_GetTriggerScriptID(osimod_t *om, int trigger_room, int trigger_face)
{
	return om->GetTriggerScriptID(trigger_room, trigger_face);
}

int osimod_GetCOScriptList(osimod_t *om, int** list, int** id_list)
{
	return om->GetCOScriptList(list, id_list);
}

int osimod_SaveRestoreState(osimod_t *om, void* file_ptr, ubyte saving_state)
{
	return om->SaveRestoreState(file_ptr, saving_state);
}

int osimod_load_mod(osimod_t **ret, module *mod, osimod_type type) {
	osimod_t *om;
	om = new osimod_t();
	om->mod = *mod;

	om->InitializeDLL = type != OMT_MISSION ? (InitializeDLL_fp)mod_GetSymbol(mod, "InitializeDLL", 4) : NULL;
	om->ShutdownDLL = type != OMT_MISSION ? (ShutdownDLL_fp)mod_GetSymbol(mod, "ShutdownDLL", 0) : NULL;
	om->GetGOScriptID = (GetGOScriptID_fp)mod_GetSymbol(mod, "GetGOScriptID", 8);
	om->GetTriggerScriptID = type == OMT_LEVEL ? (GetTriggerScriptID_fp)mod_GetSymbol(mod, "GetTriggerScriptID", 8) : NULL;
	om->GetCOScriptList = type == OMT_LEVEL ? (GetCOScriptList_fp)mod_GetSymbol(mod, "GetCOScriptList", 8) : NULL;
	om->CreateInstance = (CreateInstance_fp)mod_GetSymbol(mod, "CreateInstance", 4);
	om->DestroyInstance = (DestroyInstance_fp)mod_GetSymbol(mod, "DestroyInstance", 8);
	om->CallInstanceEvent = (CallInstanceEvent_fp)mod_GetSymbol(mod, "CallInstanceEvent", 16);
	om->SaveRestoreState = (SaveRestoreState_fp)mod_GetSymbol(mod, "SaveRestoreState", 8);

	if ((type != OMT_MISSION && !om->InitializeDLL) ||
		(type != OMT_MISSION && !om->ShutdownDLL) ||
		!om->GetGOScriptID ||
		(type == OMT_LEVEL && !om->GetTriggerScriptID) ||
		(type == OMT_LEVEL && !om->GetCOScriptList) ||
		!om->CreateInstance ||
		!om->DestroyInstance ||
		!om->SaveRestoreState ||
		!om->CallInstanceEvent) {
		delete om;
		return 0;
	}

	*ret = om;
	return 1;
}

int osimod_load(osimod_t **ret, char *fullpath, osimod_type type) {
	module mod;
	if (!mod_LoadModule(&mod, fullpath))
		return 0;
	int rc = osimod_load_mod(ret, &mod, type);
	if (!rc)
		mod_FreeModule(&mod);
	return rc;
}

void osimod_free(osimod_t **om_ptr) {
	osimod_t *om = *om_ptr;
	mod_FreeModule(&om->mod);
	delete om;
	*om_ptr = NULL;
}
