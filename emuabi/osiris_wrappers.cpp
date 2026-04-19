#include "emu86.h"

int osipf_FindSoundName(char *name);
int osipf_FindRoomName(char *name);
int osipf_FindTriggerName(char *name);
int osipf_FindObjectName(char *name);
int osipf_FindDoorName(char *name);
int osipf_FindTextureName(char *name);
int osipf_FindMatcenName(char *name);
int osipf_FindPathName(char *name);
int osipf_FindLevelGoalName(char *name);

void emucall_osipf_FindSoundName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindSoundName(name));
}

void emucall_osipf_FindRoomName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindRoomName(name));
}

void emucall_osipf_FindTriggerName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindTriggerName(name));
}

void emucall_osipf_FindObjectName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindObjectName(name));
}

void emucall_osipf_FindDoorName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindDoorName(name));
}

void emucall_osipf_FindTextureName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindTextureName(name));
}

void emucall_osipf_FindMatcenName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindMatcenName(name));
}

void emucall_osipf_FindPathName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindPathName(name));
}

void emucall_osipf_FindLevelGoalName(Emu86FunCtx& ctx, void *)
{
	char *name = ctx.arg<char *>(0);
	ctx.set_return(osipf_FindLevelGoalName(name));
}

const emu86_fun_t kOsirisWrapperFuns[] = {
	{"osipf_FindSoundName", emucall_osipf_FindSoundName, 1, 0},
	{"osipf_FindRoomName", emucall_osipf_FindRoomName, 1, 0},
	{"osipf_FindTriggerName", emucall_osipf_FindTriggerName, 1, 0},
	{"osipf_FindObjectName", emucall_osipf_FindObjectName, 1, 0},
	{"osipf_FindDoorName", emucall_osipf_FindDoorName, 1, 0},
	{"osipf_FindTextureName", emucall_osipf_FindTextureName, 1, 0},
	{"osipf_FindMatcenName", emucall_osipf_FindMatcenName, 1, 0},
	{"osipf_FindPathName", emucall_osipf_FindPathName, 1, 0},
	{"osipf_FindLevelGoalName", emucall_osipf_FindLevelGoalName, 1, 0},
};
