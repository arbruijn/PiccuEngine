#ifndef OSIMOD_H
#define OSIMOD_h
#include "osiris_dll.h"
#include "pstypes.h"

struct osimod_t;

enum osimod_type {
    OMT_LEVEL,
    OMT_MISSION,
    OMT_GAME
};

char osimod_InitializeDLL(osimod_t *om, tOSIRISModuleInit* function_list);
void osimod_ShutdownDLL(osimod_t *om);
int osimod_GetGOScriptID(osimod_t *om, char* name, ubyte isdoor);
void *osimod_CreateInstance(osimod_t *om, int id);
void osimod_DestroyInstance(osimod_t *om, int id, void* ptr);
short osimod_CallInstanceEvent(osimod_t *om, int id, void* ptr, int event, tOSIRISEventInfo* data);
int osimod_GetTriggerScriptID(osimod_t *om, int trigger_room, int trigger_face);
int osimod_GetCOScriptList(osimod_t *om, int** list, int** id_list);
int osimod_SaveRestoreState(osimod_t *om, void* file_ptr, ubyte saving_state);

int osimod_load(osimod_t **ret, char *fullpath, osimod_type type);
int osimod_load_mod(osimod_t **ret, module *mod, osimod_type type);
void osimod_free(osimod_t **om_ptr);

#endif
