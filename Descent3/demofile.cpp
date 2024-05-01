/* 
* Descent 3 
* Copyright (C) 2024 Parallax Software
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include "CFILE.H"
#include "objinfo.h"
#include "ship.h"
#include "ui.h"
#include "newui.h"
#include "hud.h"
#include "stringtable.h"
#include "program.h"
#include "Mission.h"
#include "game.h"
#include "gamesequence.h"
#include "weapon.h"
#include "damage.h"
#include "mem.h"
#include "ObjScript.h"
#include "hlsoundlib.h"
#include "viseffect.h"
#include "collide.h"
#include "sounds.h"
#include "fireball.h"
#include "attach.h"
#include "gameloop.h"
#include "multi.h"
#include "multisafe.h"
#include "osiris_dll.h"
#include "args.h"
#include "gamecinematics.h"
#include "psrand.h"
#include "cockpit.h"
//We borrow a lot of code from the savegame system
#include "gamesave.h"
#include "demofile.h"


extern bool is_multi_demo;
CFILE * Demo_cfp = NULL;
char Demo_fname[_MAX_PATH*2];
char Old_demo_fname[_MAX_PATH*2];
float Demo_next_frame = 0;
float Demo_frame_time = 0;
float Demo_last_pinfo;
float Demo_frame_ofs;
int Demo_auto_idx=0;
unsigned int Demo_flags = 0;
unsigned short Demo_obj_map[MAX_OBJECTS];
bool Demo_turretchanged[MAX_OBJECTS];
bool Demo_looping = false;
bool Demo_paused = false;
bool Demo_do_one_frame = false;
bool Demo_restart = false;
bool Demo_auto_play=false;
bool Demo_first_frame=true;
bool Demo_make_movie = false;

#define DEMO_PINFO_UPDATE	.1
#define MAX_COOP_TURRETS 400
extern float turret_holder[MAX_COOP_TURRETS];

extern gs_tables *gs_Xlates;

extern float Min_frametime;
extern float Max_frametime;
extern float Avg_frametime;
extern unsigned int Frames_counted;
extern bool Game_paused;

extern bool Game_gauge_do_time_test;

bool Demo_play_fast = false;

void PageInAllData (void);

//Prompts user for filename and starts recording if successfull
void DemoToggleRecording()
{
	char szfile[_MAX_PATH*2];
	szfile[0] = NULL;
	if(Demo_flags==DF_RECORDING)
	{
		//Stop recording and close the file
		cfclose(Demo_cfp);
		Demo_flags = DF_NONE;
		AddBlinkingHUDMessage(TXT_DEMOSAVED);

		Demo_fname[0] = NULL;
		return;
	}
	else if(Demo_flags==DF_PLAYBACK)
	{
		// We can't record a demo while we are playing back a demo
		return;
	}
	
//	hand coded 128 because long filenames were failing in cfopen(which called fopen, which failed on very
// long filenames.  instead of printing a message out to the user, just don't allow filenames that long)
	if(DoEditDialog(TXT_DEMOFILENAME,szfile,128))
	{
		if(strcmpi(szfile+(strlen(szfile)-4),".dem")!=0)
		{
			strcat(szfile,".dem");
		}
		ddio_MakePath(Demo_fname,User_directory,"demo",szfile,NULL);
		mprintf((0,"Saving demo to file: %s\n",Demo_fname));
		//Try to create the file
		Demo_cfp = cfopen(Demo_fname,"wb");
		if(Demo_cfp)
		{
			//Setup the demo variables
			if(!(Game_mode & GM_MULTI))
			{
				MultiBuildMatchTables();
			}
			//Male sure we write the player info the first frame
			Demo_last_pinfo = timer_GetTime()-(DEMO_PINFO_UPDATE*2);
			Demo_flags = DF_RECORDING;
			//Write the header
			DemoWriteHeader();
			DemoStartNewFrame();
		}
		else
		{
			//cfopen failed
			AddBlinkingHUDMessage(TXT_DEMOCANTCREATE);
			Demo_fname[0] = NULL;
			return;
		}
	}


}

void DemoWriteHeader()
{
	char szsig[10];
	strcpy(szsig,D3_DEMO_SIG_NEW);
	ASSERT(Demo_flags==DF_RECORDING);
	
	//Start off with the signature
	cf_WriteString(Demo_cfp,(const char *)szsig);
	//Next is the version
	cf_WriteShort(Demo_cfp,GAMESAVE_VERSION);
	//Write the mission filename
	if(Current_mission.filename && (strcmpi("d3_2.mn3",Current_mission.filename)==0) )
	{
		cf_WriteString(Demo_cfp, "d3.mn3");
	}
	else
	{
		cf_WriteString(Demo_cfp, Current_mission.filename ? Current_mission.filename : "");
	}
	
	//Level number
	cf_WriteInt(Demo_cfp,Current_mission.cur_level);	
	//Gametime
	cf_WriteFloat(Demo_cfp,Gametime);
	
	//Frame count
	cf_WriteInt(Demo_cfp, FrameCount);

	//Now store the world state (borrowing save game code)
	
	// Load translation tables
	SGSXlateTables(Demo_cfp);
	
	// save out room information.
	SGSRooms(Demo_cfp);

	// save out triggers
	SGSTriggers(Demo_cfp);

	// save out object information.
	SGSObjects(Demo_cfp);

	// players
	SGSPlayers(Demo_cfp);

	// save out viseffects
	SGSVisEffects(Demo_cfp);

	// save out spew
	SGSSpew(Demo_cfp);

	Osiris_SaveSystemState(Demo_cfp);

	cf_WriteShort(Demo_cfp,Player_num);

}

void DemoStartNewFrame()
{
	if(Demo_flags!=DF_RECORDING)
	{
		return;
	}
	

	//Start with the gametime of this frame
	cf_WriteByte(Demo_cfp, DT_NEW_FRAME);	
	cf_WriteFloat(Demo_cfp,Gametime);	
	cf_WriteFloat(Demo_cfp,Frametime);	
	
	if((timer_GetTime()-Demo_last_pinfo)>=DEMO_PINFO_UPDATE)
	{
		DemoWritePlayerInfo();	
		Demo_last_pinfo = timer_GetTime();
	}
	
}

//Store object num, position, orientation
//WB info, Anim states
void DemoWriteChangedObj(object *op)
{
	ASSERT(op);
	if(Demo_flags==DF_RECORDING)
	{
		gs_WriteByte(Demo_cfp, DT_OBJ);
		cf_WriteShort(Demo_cfp,OBJNUM(op));
		//	positional information
		gs_WriteInt(Demo_cfp, op->roomnum);
		gs_WriteVector(Demo_cfp, op->pos);
		gs_WriteMatrix(Demo_cfp, op->orient);
		if(op->type==OBJ_PLAYER || op->type==OBJ_OBSERVER)
		{
			gs_WriteInt(Demo_cfp, Players[op->id].flags);
		}
	}	
}

void DemoWriteWeaponFire(unsigned short objectnum,vector *pos,vector *dir,unsigned short weaponnum,unsigned short weapobjnum,short gunnum)
{
	uint uniqueid = MultiGetMatchChecksum (OBJ_WEAPON,weaponnum);
	if(weapobjnum==-1)
		return;
	if(!Weapons[weaponnum].used)	
	{
		Int3();
	}
	if(Demo_flags==DF_RECORDING)
	{
		gs_WriteByte(Demo_cfp, DT_WEAPON_FIRE);
		cf_WriteFloat(Demo_cfp,Gametime);	
		cf_WriteShort(Demo_cfp,objectnum);
		cf_WriteInt(Demo_cfp,uniqueid);
		gs_WriteVector(Demo_cfp, *pos);
		gs_WriteVector(Demo_cfp, *dir);
		gs_WriteShort(Demo_cfp,weapobjnum);
		cf_WriteShort(Demo_cfp,gunnum);
	}
}

void DemoWriteHudMessage(unsigned int color,bool blink,char *msg)
{
	if(Demo_flags==DF_RECORDING)
	{
		cf_WriteByte(Demo_cfp, DT_HUD_MESSAGE);
		cf_WriteInt(Demo_cfp, color);
		cf_WriteByte(Demo_cfp, blink?1:0);
		cf_WriteString(Demo_cfp,msg);
	}
}

void DemoWriteChangedObjects()
{
	int i;
	//int num_changed = 0;
	if(Demo_flags==DF_RECORDING)
	{
		for (i = 0; i <= Highest_object_index; i++)
		{
			
			if((Objects[i].type==OBJ_PLAYER)||(Objects[i].type==OBJ_OBSERVER)||(Objects[i].type==OBJ_ROBOT)||(Objects[i].type==OBJ_POWERUP)||(Objects[i].type==OBJ_CLUTTER)||(Objects[i].type==OBJ_BUILDING)||(Objects[i].type==OBJ_CAMERA))
			{
				if((Objects[i].flags & OF_MOVED_THIS_FRAME))//||(Objects[i].mtype.phys_info.velocity!=Zero_vector))
				{
					//mprintf((0,"Object moved this frame! type = %d\n",Objects[i].type));
					DemoWriteChangedObj(&Objects[i]);
					//num_changed++;
				}
			}
		}
	//	if(num_changed)
	//		mprintf((0,"%d Objects moved this frame!\n",num_changed));
	}
}

void DemoWriteObjCreate(ubyte type, ushort id, int roomnum, vector *pos, const matrix *orient, int parent_handle,object * obj)
{
	
	if(Demo_flags==DF_RECORDING)
	{
		
		if((type==OBJ_ROBOT)||(type==OBJ_POWERUP)||(type==OBJ_CLUTTER)||(type==OBJ_BUILDING)||(type==OBJ_CAMERA))
		{
			cf_WriteByte(Demo_cfp, DT_OBJ_CREATE);
			//cf_WriteFloat(Demo_cfp,Gametime);	
			cf_WriteByte(Demo_cfp, type);
			cf_WriteByte(Demo_cfp,orient?1:0);
			cf_WriteShort(Demo_cfp,id);
			cf_WriteInt(Demo_cfp, roomnum);
			gs_WriteVector(Demo_cfp, *pos);
			cf_WriteInt(Demo_cfp, parent_handle);
			if(orient) 
				gs_WriteMatrix(Demo_cfp, *orient);			

			// Add the script name if there is one
			// Start with the name len (minus the null)

			cf_WriteShort(Demo_cfp,OBJNUM(obj));
		}
	}
}

#define MAX_COOP_TURRETS 400
extern float turret_holder[MAX_COOP_TURRETS];

void DemoWriteTurretChanged(unsigned short objnum)
{
	Demo_turretchanged[objnum] = true;
}

void DemoReadTurretChanged(void)
{
	multi_turret multi_turret_info;
	int objnum;
	ushort num_turrets;
	float turr_time;
	int count = 0;
	float do_time;
	
	do_time = cf_ReadFloat(Demo_cfp);
	short old_objnum = cf_ReadShort(Demo_cfp);
	objnum = Demo_obj_map[old_objnum];
	
	turr_time = cf_ReadFloat(Demo_cfp);
	num_turrets = cf_ReadShort(Demo_cfp);
	multi_turret_info.keyframes = (float *)&turret_holder;
	multi_turret_info.num_turrets = num_turrets;
	for(int i = 0; i < num_turrets; i++)
	{
		if(MAX_COOP_TURRETS>i)
			multi_turret_info.keyframes[i] = cf_ReadFloat(Demo_cfp);
	}
	
	if(Objects[objnum].type!=OBJ_NONE)
	{
		//mprintf((0,"Updating turret %d!\n",objnum));
		ObjSetTurretUpdate(objnum, &multi_turret_info);
	}
}


void DemoWriteObjAnimChanged(unsigned short objnum)
{

	custom_anim multi_anim_info;
	if(ObjGetAnimUpdate(objnum, &multi_anim_info)) // Checks if obj is still alive and all
	{
		cf_WriteByte(Demo_cfp, DT_OBJ_ANIM);
		cf_WriteFloat(Demo_cfp,Gametime);
		cf_WriteShort(Demo_cfp,objnum);

		cf_WriteFloat(Demo_cfp,multi_anim_info.server_time);
		cf_WriteShort(Demo_cfp,multi_anim_info.server_anim_frame);
		cf_WriteByte(Demo_cfp, multi_anim_info.anim_start_frame);
		cf_WriteByte(Demo_cfp, multi_anim_info.anim_end_frame);
		
		cf_WriteFloat(Demo_cfp,multi_anim_info.anim_time);
		cf_WriteFloat(Demo_cfp,multi_anim_info.max_speed);
		
		cf_WriteByte(Demo_cfp, multi_anim_info.flags);
		cf_WriteShort(Demo_cfp,multi_anim_info.anim_sound_index);
	}
}

void DemoReadObjAnimChanged(void)
{
	custom_anim multi_anim_info;
	int objnum;
	int count = 0;
	float changetime;
	changetime = cf_ReadFloat(Demo_cfp);
	objnum = cf_ReadShort(Demo_cfp);
	
	multi_anim_info.server_time = cf_ReadFloat(Demo_cfp);
	multi_anim_info.server_anim_frame = cf_ReadShort(Demo_cfp);
	multi_anim_info.anim_start_frame = cf_ReadByte(Demo_cfp);
	multi_anim_info.anim_end_frame = cf_ReadByte(Demo_cfp);
	multi_anim_info.anim_time = cf_ReadFloat(Demo_cfp);
	multi_anim_info.max_speed = cf_ReadFloat(Demo_cfp);
	multi_anim_info.flags = cf_ReadByte(Demo_cfp);
	multi_anim_info.anim_sound_index = cf_ReadShort(Demo_cfp);
	
	if(Objects[objnum].type!=OBJ_NONE)
	{
		ObjSetAnimUpdate(objnum, &multi_anim_info);
	}

}

void DemoWriteKillObject(object *hit_obj,object *killer,float damage,int death_flags,float delay,int seed)
{
	cf_WriteByte(Demo_cfp,DT_OBJ_EXPLODE);
	cf_WriteShort(Demo_cfp,OBJNUM(hit_obj));
	cf_WriteShort(Demo_cfp,killer?OBJNUM(killer):65535);
	cf_WriteFloat(Demo_cfp,damage);
	cf_WriteInt(Demo_cfp,death_flags);
	cf_WriteFloat(Demo_cfp,delay);
	cf_WriteShort(Demo_cfp,seed);
}

void DemoReadKillObject(void)
{
	short hit_objnum = cf_ReadShort(Demo_cfp);
	short killer = cf_ReadShort(Demo_cfp);
	float damage = cf_ReadFloat(Demo_cfp);
	int death_flags = cf_ReadInt(Demo_cfp);
	float delay = cf_ReadFloat(Demo_cfp);
	short seed = cf_ReadShort(Demo_cfp);

	if (! (IS_GENERIC(Objects[hit_objnum].type) || (Objects[hit_objnum].type == OBJ_DOOR)))
		return;	//bail if invalid object type

	ps_srand(seed);
	KillObject(&Objects[hit_objnum],(killer!=65535)?&Objects[killer]:NULL,damage,death_flags,delay);
}

void DemoWritePlayerDeath(object *player,bool melee,int fate)
{
	cf_WriteByte(Demo_cfp,DT_PLAYER_DEATH);
	cf_WriteShort(Demo_cfp,OBJNUM(player));
	cf_WriteByte(Demo_cfp,melee?1:0);
	cf_WriteInt(Demo_cfp, fate);
}

void DemoReadPlayerDeath(void)
{
	short playernum = cf_ReadShort(Demo_cfp);
	ubyte melee = cf_ReadByte(Demo_cfp);
	int fate = cf_ReadInt(Demo_cfp);
	InitiatePlayerDeath(&Objects[playernum],melee?true:false,fate);
}

void DemoWrite2DSound(short soundidx,float volume)
{
	cf_WriteByte(Demo_cfp,DT_2D_SOUND);
	cf_WriteShort(Demo_cfp,soundidx);
	cf_WriteFloat(Demo_cfp,volume);
}

void DemoRead2DSound(void)
{
	int soundidx = cf_ReadShort(Demo_cfp);
	float volume = cf_ReadFloat(Demo_cfp);

	Sound_system.Play2dSound(soundidx,volume);
}

void DemoWrite3DSound(short soundidx,ushort objnum,int priority,float volume)
{
	cf_WriteByte(Demo_cfp,DT_3D_SOUND);
	cf_WriteShort(Demo_cfp,objnum);
	cf_WriteShort(Demo_cfp,soundidx);
	cf_WriteFloat(Demo_cfp,volume);

}

void DemoRead3DSound(void)
{
	int objnum;
	short soundidx;
	float volume;

	objnum = cf_ReadShort(Demo_cfp);

	soundidx = cf_ReadShort(Demo_cfp);
	volume = cf_ReadFloat(Demo_cfp);

	Sound_system.Play3dSound(soundidx,&Objects[objnum],volume);
}



void DemoWriteScriptEvent()
{

}

int FrameDemoDelta=0;
int DemoFrameCount = 0;

int DemoPlaybackFile(char *filename)
{
	is_multi_demo = false;
	mprintf((0,"Playing back demo file!\n"));
	MultiBuildMatchTables();
	if(FindArg("-makemovie"))
	{
		Demo_make_movie = true;
	}

	if(FindArg("-fastdemo"))
	{
		Demo_play_fast = true;
	}
	Demo_first_frame=true;
	for(int i=0;i<MAX_OBJECTS;i++)
	{
		Demo_obj_map[i] = i;
	}

	if(Demo_flags==DF_RECORDING)
	{
		DemoToggleRecording();	
	}
	Demo_cfp = cfopen(filename,"rb");
	
	if(Demo_cfp==NULL)
	{
		DoMessageBox(TXT_ERROR,TXT_CANTLOAD,MSGBOX_OK,UICOL_WINDOW_TITLE,UICOL_TEXT_NORMAL);		
		return 0;
	}
	
	Demo_flags = DF_PLAYBACK;
	FrameDemoDelta = FrameCount;
	if(!DemoReadHeader())
	{
		Demo_flags = DF_NONE;
		DoMessageBox(TXT_ERROR,TXT_BADDEMOFILE,MSGBOX_OK,UICOL_WINDOW_TITLE,UICOL_TEXT_NORMAL);		
		return 0;
	}
	

	return 1;
}

extern bool IsRestoredGame;
int DemoReadHeader()
{
	char szsig[10];
	short ver;
	char demo_mission[_MAX_PATH];
	int level_num;
	int frame_count;
	float demo_gametime;
	mprintf((0,"Reading demo header...\n"));
	ASSERT(Demo_flags==DF_PLAYBACK);
	cf_ReadString((char *)szsig,10,Demo_cfp);
	ver = cf_ReadShort(Demo_cfp);
	
	if(strcmp(szsig,D3_DEMO_SIG)!=0)
	{
		if(strcmp(szsig,D3_DEMO_SIG_NEW)!=0)
		{
			mprintf((0,"Bad demo header signature!\n"));
			return 0;
		}		
	}

	if(ver>GAMESAVE_VERSION)
	{
		//This file version is too new!
		//return 0;
	}
	
	cf_ReadString(demo_mission,_MAX_PATH,Demo_cfp);
	level_num = cf_ReadInt(Demo_cfp);
	demo_gametime = cf_ReadFloat(Demo_cfp);
	frame_count = cf_ReadInt(Demo_cfp);
	
	//Now load the mission
	Osiris_DisableCreateEvents();
	IsRestoredGame = true;
	if (LoadMission((const char *)demo_mission)) 
	{
		mng_LoadAddonPages ();

		SetCurrentLevel(level_num);
		if (!LoadAndStartCurrentLevel())
		{
			mprintf((0,"Couldn't start new level to play back the demo!\n"));
			Osiris_EnableCreateEvents();
			return 0;
		}		
	}
	else
	{
		mprintf((0,"Couldn't load the level to play back the demo!\n"));
		Osiris_EnableCreateEvents();
		return 0;
	}
	Osiris_EnableCreateEvents();
	
	FrameCount = frame_count;
	Demo_next_frame = demo_gametime;

	if (gs_Xlates)
		delete (gs_Xlates);
	gs_Xlates = new gs_tables;

	try
	{
		LGSXlateTables(Demo_cfp);
		
		LGSRooms(Demo_cfp);

		LGSTriggers(Demo_cfp);

		for(int j=0;j<=Highest_object_index;j++)
		{
			if( (Objects[j].type==OBJ_PLAYER) && (Objects[j].id!=Player_num) )
			{
				object *objp = &Objects[j];
				objp->type=OBJ_GHOST;
				objp->movement_type=MT_NONE;
				objp->render_type=RT_NONE;
				SetObjectControlType(objp, CT_NONE);
			}
		}

		LGSObjects(Demo_cfp, ver);
		//Fix up the object list for use in MSAFE
		for(int a=0;a<MAX_OBJECTS;a++)
		{
			Server_object_list[a] = a;
			Objects[a].flags |= OF_SERVER_OBJECT;
		}
		
		LGSPlayers(Demo_cfp);

		LGSVisEffects(Demo_cfp);

		LGSSpew(Demo_cfp);

		if(!Osiris_RestoreSystemState(Demo_cfp))
		{
			mprintf((0,"Error restoring Osiris\n"));
			return 0;
		}

		Player_num = cf_ReadShort(Demo_cfp);

		Viewer_object = &Objects[Players[Player_num].objnum];
		Player_object = Viewer_object;

		InitShipHUD(Players[Player_num].ship_index);
		InitCockpit(Players[Player_num].ship_index);

		if (GetHUDMode() == HUD_COCKPIT) 
			SetHUDMode(HUD_COCKPIT);
		else if (GetHUDMode() == HUD_FULLSCREEN) 
			SetHUDMode(HUD_FULLSCREEN);


		//Reset rearview cameras since Player_object may have changed
		extern void RestoreCameraRearviews();
		extern int Camera_view_mode[];
		Camera_view_mode[0] = Camera_view_mode[2] = 0;		//(0==CV_NONE)  Force reinitialization
		RestoreCameraRearviews();
	}
	catch(...)
	{
		mprintf((0,"Someone threw an exception while reading the demo header!\n"));
		return 0;
	}

	PageInAllData ();

	//Don't free this variable here, because we need to xlate ids when creating objects during playback
	//mem_free(gs_Xlates);
	//gs_Xlates = NULL;

	//Turn sounds back on
	Sound_system.ResumeSounds();
	Min_frametime = 500;
	Max_frametime = 0;
	Avg_frametime = 0;
	Frames_counted=0;

	int a;
	for(a=0;a<MAX_PLAYERS;a++)
	{
		Players[a].weapon_speed_scalar = 1;
		Players[a].movement_scalar = 1;
		Players[a].damage_scalar = 1;
		Players[a].armor_scalar = 1;
		Players[a].turn_scalar = 1;
		Players[a].weapon_recharge_scalar = 1;

		//Set the ship number for this player, since this value is not saved with demo header
		for (int objnum=0;objnum<=Highest_object_index;objnum++)
		{
			//Look for a player object that is this player
			if ((Objects[objnum].type == OBJ_PLAYER) && (Objects[objnum].id == a))
			{
				//We found an object for this player.  Now find a ship that matches this object's model
				for (int s=0;s<MAX_SHIPS;s++)
				{
					if (Ships[s].used && (Ships[s].model_handle == Objects[objnum].rtype.pobj_info.model_num))
					{
						//Found a matching ship, so set the player to use that ship
						Players[a].ship_index = s;
						break;		//found a ship for this player, so stop looking
					}
				}

				break;	//found an object for this player, so stop looking
			}
		}
	}

	return 1;
}




void DemoReadObj()
{
	short objnum;
	object *obj;
	vector pos;
	matrix orient;
	ASSERT(Demo_flags==DF_PLAYBACK);
	objnum = cf_ReadShort(Demo_cfp);	
	
	obj = &Objects[Demo_obj_map[objnum]];
	//ASSERT(obj->type!=OBJ_NONE);

	int roomnum = cf_ReadInt(Demo_cfp);
	gs_ReadVector(Demo_cfp,pos);
	gs_ReadMatrix(Demo_cfp,orient);
	if(obj->type==OBJ_PLAYER||obj->type==OBJ_OBSERVER)
	{
		if(obj->id!=Player_num)
			is_multi_demo = true;
		Players[obj->id].flags = cf_ReadInt(Demo_cfp);

		if (! (Players[obj->id].flags & PLAYER_FLAGS_DEAD))
			ResetPersistentHUDMessage();
	}
	//if((!((obj->flags&OF_DYING)||(obj->flags&OF_EXPLODING)||(obj->flags&OF_DEAD)))&&obj->type!=255)
	
	if(obj->type!=OBJ_NONE)
	{
		obj->mtype.phys_info.velocity = Zero_vector;
		obj->mtype.phys_info.rotvel = Zero_vector;
		ObjSetPos (obj,&pos,roomnum,&orient,true);
		poly_model *pm=&Poly_models[obj->rtype.pobj_info.model_num];
		if( (!(obj->flags&OF_ATTACHED)) && (pm->n_attach) )
			AttachUpdateSubObjects(obj);
	}
}

void DemoReadHudMessage()
{
	char msg[HUD_MESSAGE_LENGTH];
	int color = cf_ReadInt(Demo_cfp);
	ubyte blink = cf_ReadByte(Demo_cfp);
	cf_ReadString(msg,HUD_MESSAGE_LENGTH,Demo_cfp);

	if(color)
	{
		AddColoredHUDMessage (color,msg);
	}
	else if(blink)
	{
		AddBlinkingHUDMessage(msg);
	}
	else
	{
		AddHUDMessage(msg);
	}
}



void DemoReadWeaponFire()
{
	vector pos,dir;
	float gametime;
	uint uniqueid;
	object *obj;
	short weaponnum,objnum,weapobjnum;
	vector laser_pos,laser_dir;
	
	// Mass driver is hack
	static int mass_driver_id=-2;
	static int mass_driver_ring=-2;
	static int omega_id = -2;
	static int vauss_id = -2;
	static int napalm_id = -2;
	
	if (mass_driver_id==-2)
	{
		mass_driver_id=FindWeaponName ("Mass Driver");
		ASSERT(mass_driver_id != -1);	//DAJ -1FIX
		mass_driver_ring=FindTextureName ("CloakRing.tga1");
		ASSERT(mass_driver_ring != -1);	//DAJ -1FIX
		omega_id = FindWeaponName ("Omega");
		ASSERT(omega_id != -1);	//DAJ -1FIX
		vauss_id = FindWeaponName ("Vauss");
		ASSERT(vauss_id != -1);	//DAJ -1FIX
		napalm_id = FindWeaponName ("Napalm");
		ASSERT(napalm_id != -1);	//DAJ -1FIX
	}
	
	gametime = cf_ReadFloat(Demo_cfp);
	short old_objnum = cf_ReadShort(Demo_cfp);
	objnum = Demo_obj_map[old_objnum];
	obj = &Objects[objnum];
	ASSERT(Objects[objnum].type!=OBJ_NONE);

	uniqueid = cf_ReadInt(Demo_cfp);
	gs_ReadVector(Demo_cfp,pos);
	gs_ReadVector(Demo_cfp,dir);
	laser_pos = pos;
	laser_dir = dir;
	weaponnum = MultiMatchWeapon(uniqueid);
	weapobjnum = cf_ReadShort(Demo_cfp);
	short gunnum = cf_ReadShort(Demo_cfp);
	ASSERT(uniqueid != 0xffffffff);
	ASSERT(dir != Zero_vector);


	//This is a hack for the napalm, omega & vauss to prevent making files incompatible
	if( (obj->type==OBJ_PLAYER) && (obj->id!=Player_num) )
	{
		if(omega_id==weaponnum)
		{
			Players[obj->id].weapon[PW_PRIMARY].index = OMEGA_INDEX;
		}
		else if(vauss_id==weaponnum)
		{
			Players[obj->id].weapon[PW_PRIMARY].index = VAUSS_INDEX;
		}
		else if(napalm_id==weaponnum)
		{
			Players[obj->id].weapon[PW_PRIMARY].index = NAPALM_INDEX;
		}

	}

	unsigned short new_weap_objnum = CreateAndFireWeapon( &pos, &dir, &Objects[objnum], weaponnum);
	if(0xffff==new_weap_objnum)
	{
		Int3();
		return;
	}
	Objects[new_weap_objnum].ctype.laser_info.src_gun_num=gunnum;
	
	mprintf((0,"Player %d Firing weapon (%d) -- using objnum %d (old num = %d)\n",obj->id,weaponnum,new_weap_objnum,weapobjnum));
	
	Demo_obj_map[weapobjnum] = new_weap_objnum;
	short weapon_num = weaponnum;

	if (Weapons[weapon_num].sounds[WSI_FLYING]!=SOUND_NONE_INDEX)
		Sound_system.Play3dSound(Weapons[weapon_num].sounds[WSI_FLYING], &Objects[objnum]);

	// Do a muzzle flash from this gun
	if (Weapons[weapon_num].flags & WF_MUZZLE)
	{
		int visnum;
		vector newpos;
		
		if (obj==&Objects[Players[Player_num].objnum])
		{
			newpos=laser_pos+(laser_dir);
			visnum=VisEffectCreate (VIS_FIREBALL,MUZZLE_FLASH_INDEX,obj->roomnum,&newpos);
		}
		else
		{
			newpos=laser_pos+(laser_dir/2);
			visnum=VisEffectCreate (VIS_FIREBALL,MUZZLE_FLASH_INDEX,obj->roomnum,&newpos);
		}
	
		// Make this guy live for a very short time
		if (visnum>=0)
		{
			vis_effect *vis=&VisEffects[visnum];

			vis->lifetime=.08f;
			vis->lifeleft=.08f;
			vis->size=1.0f;
		
			vis->movement_type=MT_PHYSICS;
			vis->velocity=obj->mtype.phys_info.velocity;

			// Make some smoke!
			visnum=VisEffectCreate (VIS_FIREBALL,MED_SMOKE_INDEX,obj->roomnum,&newpos);
			if (visnum>=0)
			{
				vis_effect *vis=&VisEffects[visnum];

				vis->lifetime=.3f;
				vis->lifeleft=.3f;

				vis->movement_type=MT_PHYSICS;
				vis->velocity=obj->mtype.phys_info.velocity;
				vis->velocity.y+=10;
			}
		
		}
	}

	// Check for mass driver
	if (weaponnum==mass_driver_id)
	{
		// Create a fire trail for this weapon
		// find out where this weapon hits
		fvi_query	fq;
		fvi_info		hit_data;
			
		vector dest_vec=laser_pos+(laser_dir*5000);

		fq.p0						= &laser_pos;
		fq.startroom			= obj->roomnum;
		fq.p1						= &dest_vec;
		fq.rad					= 0;
		fq.thisobjnum			= -1;
		fq.ignore_obj_list	= NULL;
		fq.flags					= FQ_CHECK_OBJS|FQ_IGNORE_POWERUPS|FQ_IGNORE_WEAPONS;

		fvi_FindIntersection(&fq, &hit_data);

		float mag=vm_VectorDistanceQuick (&hit_data.hit_pnt,&obj->pos);
				
		int visnum;
		
		if(obj == NULL || obj->type != OBJ_BUILDING || stricmp(Object_info[obj->id].name, "FinalbossLITTLETIT") != 0)
			visnum=VisEffectCreate (VIS_FIREBALL,MASSDRIVER_EFFECT_INDEX,obj->roomnum,&laser_pos);
		else
			visnum=VisEffectCreate (VIS_FIREBALL,MERCBOSS_MASSDRIVER_EFFECT_INDEX,obj->roomnum,&laser_pos);

		if (visnum>=0)
		{
			vis_effect *vis=&VisEffects[visnum];
			vis->custom_handle=mass_driver_ring;
			vis->lifeleft=1.5;
			vis->lifetime=1.5;

			vis->end_pos=hit_data.hit_pnt;
			vis->billboard_info.width=1;
			if(obj == NULL || obj->type != OBJ_BUILDING || stricmp(Object_info[obj->id].name, "FinalbossLITTLETIT") != 0)
				vis->lighting_color=GR_RGB16(100,100,170);
			else
				vis->lighting_color=GR_RGB16(170,100,100);
			vis->flags|=VF_LINK_TO_VIEWER|VF_EXPAND;
			vis->size=mag;
		}
	}

}

void DemoReadObjCreate()
{
	//float gametime;
	ubyte type;
	ubyte use_orient;
	matrix orient;
	vector pos;
	int roomnum;
	short id;
	int parent_handle;
	object *obj;


	type = cf_ReadByte(Demo_cfp);
	use_orient = cf_ReadByte(Demo_cfp);
	id = cf_ReadShort(Demo_cfp);
	roomnum = cf_ReadInt(Demo_cfp);
	gs_ReadVector(Demo_cfp,pos);
	parent_handle = cf_ReadInt(Demo_cfp);
	if(use_orient)
		gs_ReadMatrix(Demo_cfp,orient);
	//mprintf((0,"Creating object ype: %d id: %d room: %d\n",type,id,roomnum));

	// xlate id to new id.
	switch (type) 
	{
		case OBJ_ROBOT:
		case OBJ_POWERUP:
		case OBJ_BUILDING:
		case OBJ_CLUTTER:
			id = gs_Xlates->obji_indices[id]; 
			break;
		case OBJ_CAMERA:		//Don't xlate camera IDs
			break;
		case OBJ_DOOR:
		case OBJ_WEAPON:
		case OBJ_PLAYER:
			Int3();				//Shouldn't be creating these things
			break;
		default:
			Int3();				//What is this?
	}

	short new_objnum = ObjCreate(type,id,roomnum,&pos,use_orient?&orient:NULL,parent_handle);
	if(new_objnum > -1) {	//DAJ -1FIX
		obj = &Objects[new_objnum];

		short oldobjnum = cf_ReadShort(Demo_cfp);
		Demo_obj_map[oldobjnum] = new_objnum;
		//MSAFE needs this list too
		Server_object_list[oldobjnum] = new_objnum;
		InitObjectScripts (obj);
		obj->flags |= OF_SERVER_OBJECT;
	}
}


//FrameDemoDelta = FrameCount
//DemoFrameCount = 0;

ubyte DemoLastOpcode=0;

void DemoFrame()
{
	ubyte opcode;
	if(Demo_flags!=DF_PLAYBACK)
		return;
	if(!Demo_first_frame)
	{
		Demo_frame_ofs=0;

		if(Demo_make_movie)
		{
			DoScreenshot();
		}
		//This code slows down demo playback
		if( (!Game_gauge_do_time_test) && (!Demo_play_fast) )
		{
			
			float tdelta = timer_GetTime();
			//if(Gametime<Demo_next_frame)
			//{
			//	mprintf((0,"!%.3f!",Demo_next_frame-Gametime));
			//}
			
			
			while ((Gametime+Demo_frame_ofs)<Demo_next_frame) 
			{
				Demo_frame_ofs=timer_GetTime()-tdelta;
			}
			
		}
	}
	else
	{
		//mprintf((0,"This is the first frame in the demo playback!(%f)\n",Demo_next_frame));
		Gametime = Demo_next_frame;
		Demo_frame_time = 0;
		Demo_first_frame=false;
	}
	int exit_loop=0;
	do
	{
		//Read one opcode at a time, and dispatch the proper function to read the rest of the data
		//Keep going until we hit a new frame
		try
		{
			opcode = cf_ReadByte(Demo_cfp);
			//mprintf((0,"Demo ocode: %d\n",opcode));
		}
		catch( ... )
		{
			//End of file, so we're done playing the demo
			mprintf((0,"End of demo file!"));
			strcpy(Old_demo_fname,Demo_fname);
			DemoAbort();
			//Do some cool stuff here, like end of demo stats or exit to the main menu
			if(Demo_looping)
			{
				Game_interface_mode = GAME_DEMO_LOOP;
				strcpy(Demo_fname,Old_demo_fname);
			}
			else
			{
				Game_interface_mode = GAME_POST_DEMO;
			}
			return;
		}
		switch(opcode)
		{
		case DT_NEW_FRAME:
			DemoReadNewFrame();
			exit_loop=1;
			break;
		case DT_OBJ:
			DemoReadObj();
			break;
		case DT_WEAPON_FIRE:
			DemoReadWeaponFire();
			break;
		case DT_HUD_MESSAGE:
			DemoReadHudMessage();
			break;
		case DT_3D_SOUND:
			DemoRead3DSound();
			break;
		case DT_2D_SOUND:
			DemoRead2DSound();
			break;
		case DT_OBJ_CREATE:
			DemoReadObjCreate();
			break;
		case DT_OBJ_ANIM:
			DemoReadObjAnimChanged();
			break;
		case DT_OBJ_TURRET:
			DemoReadTurretChanged();
			break;
		case DT_OBJ_EXPLODE:
			DemoReadKillObject();
			break;
		case DT_PLAYER_DEATH:
			DemoReadPlayerDeath();
			break;
		case DT_COLLIDE_PLR:
			DemoReadCollidePlayerWeapon();
			break;
		case DT_COLLIDE_GEN:
			DemoReadCollideGenericWeapon();
			break;
		case DT_ATTACH:
			DemoReadAttachObj();
			break;
		case DT_ATTACH_RAD:
			DemoReadAttachObjRad();
			break;
		case DT_UNATTACH:
			DemoReadUnattachObj();
			break;
		case DT_WEAP_FIRE_FLAG:
			DemoReadObjWeapFireFlagChanged();
			break;
		case DT_PLAYER_INFO:
			DemoReadPlayerInfo();
			break;
		case DT_MSAFE:
			DemoReadMSafe();
			break;
		case DT_POWERUP:
			DemoReadPowerups();
			break;
		case DT_CINEMATICS:
			DemoReadCinematics();
			break;
		case DT_PERSISTANT_HUD:
			DemoReadPersistantHUDMessage();
			break;
		case DT_SETOBJDEAD:
			DemoReadSetObjDead();
			break;
		case DT_PLAYERBALLS:
			DemoReadPlayerBalls();
			break;
		case DT_PLAYERTYPECHNG:
			DemoReadPlayerTypeChange();
			break;
		case DT_SETOBJLIFELEFT:
			DemoReadObjLifeLeft();
			break;
		default:
			mprintf((0,"ERROR! Unknown opcode in demo file!(%d) last code: %d\n",opcode,DemoLastOpcode));
			//Int3();
			DemoAbort();
			if(Demo_looping)
			{
				Game_interface_mode = GAME_DEMO_LOOP;
				strcpy(Demo_fname,Old_demo_fname);
			}
			else
			{
				Game_interface_mode = GAME_POST_DEMO;
				
			}
			return;
		}		
	DemoLastOpcode = opcode;
	}while(!exit_loop);



}

void DemoWriteCinematics(ubyte *data,unsigned short len)
{
	cf_WriteByte(Demo_cfp,DT_CINEMATICS);
	//Write a bunch of data
	mprintf((0,"Writing Cinematic data (%d bytes) to demo file.\n",len));
	cf_WriteShort(Demo_cfp,len);
	cf_WriteBytes(data,len,Demo_cfp);
}

void DemoReadCinematics()
{
	ubyte buffer[1500];
	ushort len = cf_ReadShort(Demo_cfp);
	cf_ReadBytes(buffer,len,Demo_cfp);
	mprintf((0,"Reading Cinematic data from demo file.\n"));
	Cinematic_DoDemoFileData(buffer);
}

void DemoWriteMSafe(ubyte *data,unsigned short len)
{
	cf_WriteByte(Demo_cfp,DT_MSAFE);
	cf_WriteShort(Demo_cfp,len);
	cf_WriteBytes(data,len,Demo_cfp);
//	mprintf((0,"Writing MSAFE data to demo file.\n"));
}

void DemoWritePowerup(ubyte *data,unsigned short len)
{
	cf_WriteByte(Demo_cfp,DT_POWERUP);
	cf_WriteShort(Demo_cfp,len);
	cf_WriteBytes(data,len,Demo_cfp);
}

extern void MultiDoMSafeFunction (ubyte *data);

void DemoReadMSafe()
{
	ubyte buffer[1500];
	ushort len = cf_ReadShort(Demo_cfp);
	cf_ReadBytes(buffer,len,Demo_cfp);
	//mprintf((0,"Reading MSAFE data from demo file.\n"));
	MultiDoMSafeFunction(buffer);
}

extern void MultiDoMSafePowerup (ubyte *data);

void DemoReadPowerups()
{
	ubyte buffer[1500];
	ushort len = cf_ReadShort(Demo_cfp);
	cf_ReadBytes(buffer,len,Demo_cfp);
	MultiDoMSafePowerup(buffer);
	
}
extern void collide_player_and_weapon( object * playerobj, object * weapon, vector *collision_point, vector *collision_normal, bool f_reverse_normal, fvi_info *hit_info ) ;
extern void collide_generic_and_weapon( object * robotobj, object * weapon, vector *collision_point, vector *collision_normal, bool f_reverse_normal, fvi_info *hit_info ) ;


void DemoWriteCollidePlayerWeapon(object * playerobj, object * weapon, vector *collision_point, vector *collision_normal, bool f_reverse_normal, void *hit_info )
{
	cf_WriteByte(Demo_cfp,DT_COLLIDE_PLR);
	cf_WriteShort(Demo_cfp,OBJNUM(playerobj));
	cf_WriteShort(Demo_cfp,OBJNUM(weapon));
	gs_WriteVector(Demo_cfp, *collision_point);
	gs_WriteVector(Demo_cfp, *collision_normal);
	cf_WriteByte(Demo_cfp,f_reverse_normal?1:0);

}

void DemoReadCollidePlayerWeapon(void)
{
	vector collision_p;
	vector collision_n;
	bool f_reverse_normal;
	unsigned short real_weapnum;
	short plr_objnum = cf_ReadShort(Demo_cfp);
	short wep_objnum = cf_ReadShort(Demo_cfp);
	gs_ReadVector(Demo_cfp,collision_p);
	gs_ReadVector(Demo_cfp,collision_n);
	ubyte b = cf_ReadByte(Demo_cfp);
	f_reverse_normal = b?true:false;
		
	real_weapnum = Demo_obj_map[wep_objnum];
	if(real_weapnum!=65535)
	{
		collide_player_and_weapon(&Objects[plr_objnum],&Objects[real_weapnum],&collision_p,&collision_n,f_reverse_normal,NULL);
	}
	else
	{
		mprintf((0,"Unable to map weapon number for collision in demo playback!\n"));
	}
}

void DemoWriteCollideGenericWeapon( object * robotobj, object * weapon, vector *collision_point, vector *collision_normal, bool f_reverse_normal, void *hit_info ) 
{
	cf_WriteByte(Demo_cfp,DT_COLLIDE_GEN);
	cf_WriteShort(Demo_cfp,OBJNUM(robotobj));
	cf_WriteShort(Demo_cfp,OBJNUM(weapon));
	gs_WriteVector(Demo_cfp, *collision_point);
	gs_WriteVector(Demo_cfp, *collision_normal);
	cf_WriteByte(Demo_cfp,f_reverse_normal?1:0);

}


void DemoReadCollideGenericWeapon(void)
{
	vector collision_p;
	vector collision_n;
	bool f_reverse_normal;
	unsigned short real_weapnum;
	short gen_objnum = cf_ReadShort(Demo_cfp);
	short wep_objnum = cf_ReadShort(Demo_cfp);
	gs_ReadVector(Demo_cfp,collision_p);
	gs_ReadVector(Demo_cfp,collision_n);
	ubyte b = cf_ReadByte(Demo_cfp);
	f_reverse_normal = b?true:false;

	real_weapnum = Demo_obj_map[wep_objnum];
	if(real_weapnum!=65535)
	{
		collide_generic_and_weapon(&Objects[gen_objnum],&Objects[real_weapnum],&collision_p,&collision_n,f_reverse_normal,NULL);
	}
	else
	{
		mprintf((0,"Unable to map weapon number for collision in demo playback!\n"));
	}
}



bool LoadDemoDialog()
{
//#ifdef DEMO
//	DoMessageBox(TXT_ERROR, TXT_WRONGVERSION, MSGBOX_OK);
//	return false;
//#else 
	
	char file[_MAX_PATH*2];

	ddio_MakePath(file,User_directory,"demo",NULL);

	if(DoPathFileDialog(false,file,TXT_VIEWDEMO,"*.dem",PFDF_FILEMUSTEXIST))
	{
		strcpy(Demo_fname,file);
		return true;
	}
	return false;
//#endif	
}

//This function aborts the demo recording or playback, regardless of the state
void DemoAbort(bool deletefile)
{
	is_multi_demo = false;
	if(Demo_flags != DF_NONE)
	{
		//We're done with the xlate table, so free it
		delete (gs_Xlates);
		gs_Xlates = NULL;

		cfclose(Demo_cfp);
		Demo_flags = DF_NONE;
		if(deletefile)
			ddio_DeleteFile(Demo_fname);
		Demo_fname[0] = NULL;
		return;
	}
	
}
int Debugme = 1;

void DemoReadNewFrame(void)
{
	//float our_frametime;
	Gametime = Demo_next_frame;
	Frametime = Demo_frame_time;	
	Demo_next_frame = cf_ReadFloat(Demo_cfp);
	Demo_frame_time = cf_ReadFloat(Demo_cfp);
	
	DemoFrameCount++;
		
}

void DemoWriteAttachObjRad(object *parent, char parent_ap, object *child, float rad)
{
	cf_WriteByte(Demo_cfp,DT_ATTACH);	
	cf_WriteShort(Demo_cfp,OBJNUM(parent));
	cf_WriteByte(Demo_cfp,parent_ap);
	cf_WriteShort(Demo_cfp,OBJNUM(child));
	cf_WriteFloat(Demo_cfp,rad);
	
}

void DemoReadAttachObjRad(void)
{
	short old_objnum;
	short parent_num;
	short child_num;
	char parent_ap;
	float rad;
	old_objnum = cf_ReadShort(Demo_cfp);
	parent_num = Demo_obj_map[old_objnum];
	parent_ap = cf_ReadByte(Demo_cfp);
	old_objnum = cf_ReadShort(Demo_cfp);
	child_num = Demo_obj_map[old_objnum];
	rad = cf_ReadFloat(Demo_cfp);
	AttachObject(&Objects[parent_num],parent_ap,&Objects[child_num],rad);
}

void DemoWriteAttachObj(object *parent, char parent_ap, object *child, char child_ap, bool f_aligned)
{
	cf_WriteByte(Demo_cfp,DT_ATTACH);	
	cf_WriteShort(Demo_cfp,OBJNUM(parent));
	cf_WriteByte(Demo_cfp,parent_ap);
	cf_WriteShort(Demo_cfp,OBJNUM(child));
	cf_WriteByte(Demo_cfp,child_ap);
	cf_WriteByte(Demo_cfp,f_aligned?1:0);
}

void DemoReadAttachObj(void)
{
	short old_objnum;
	short parent_num;
	short child_num;
	char parent_ap;
	char child_ap;
	bool f_aligned;
	old_objnum = cf_ReadShort(Demo_cfp);
	parent_num = Demo_obj_map[old_objnum];
	parent_ap = cf_ReadByte(Demo_cfp);
	old_objnum = cf_ReadShort(Demo_cfp);
	child_num = Demo_obj_map[old_objnum];
	child_ap = cf_ReadByte(Demo_cfp);
	f_aligned = cf_ReadByte(Demo_cfp)?true:false;
	AttachObject(&Objects[parent_num],parent_ap,&Objects[child_num],child_ap,f_aligned);
}

void DemoWriteUnattachObj(object *child)
{
	cf_WriteByte(Demo_cfp,DT_UNATTACH);	
	cf_WriteShort(Demo_cfp,OBJNUM(child));
}

void DemoReadUnattachObj(void)
{
	short old_objnum = cf_ReadShort(Demo_cfp);
	short unattach_objnum = Demo_obj_map[old_objnum];
	UnattachFromParent(&Objects[unattach_objnum]);
}


// Display options and statistics after displaying a demofile
void DemoPostPlaybackMenu(void)
{
#define DEMO_MENU_WIDTH		320	
#define DEMO_MENU_HEIGHT	192
#define BTN_WIDTH			96
#define UID_LOOPING			200
	NewUIGameWindow window;
	NewUIButton ok_btn;
	NewUIButton cancel_btn;
	NewUIButton loop_btn;
	UIText wndtitle,fps_min,fps_avg,fps_max;
	bool exit_menu = false, ret=true;
	int cury = 9;
	char sztemp[200] = "";

	
	window.Create(0,0,DEMO_MENU_WIDTH,DEMO_MENU_HEIGHT,UIF_PROCESS_MENU|UIF_CENTER | NUWF_TITLELARGE);
	wndtitle.Create(&window, &UITextItem(TXT_DEMO_PLAY_OPTIONS), 0, cury, UIF_CENTER| UIF_FIT );cury+=30;
	if(Demo_play_fast)
	{
		sprintf(sztemp,TXT_MINFPS,(float)1/Max_frametime);
		fps_min.Create(&window, &UITextItem(sztemp), 0, cury, UIF_CENTER| UIF_FIT);cury+=20;
		sprintf(sztemp,TXT_MAXFPS,(float)1/Min_frametime);
		fps_max.Create(&window, &UITextItem(sztemp), 0, cury, UIF_CENTER| UIF_FIT);cury+=20;
		sprintf(sztemp,TXT_AVGFSP,(float)1/(Avg_frametime/Frames_counted));
		fps_avg.Create(&window, &UITextItem(sztemp), 0, cury, UIF_CENTER| UIF_FIT);cury+=20;
	}
	//cury+=20;
	loop_btn.Create(&window, UID_LOOPING, &UITextItem(TXT_PLAYLOOPING), 0, cury, BTN_WIDTH*1.5, 32,UIF_CENTER| UIF_FIT); cury+=35;
	
	ok_btn.Create(&window, UID_OK, &UITextItem(TXT_DONE), 0, DEMO_MENU_HEIGHT-70, BTN_WIDTH, 32,UIF_CENTER| UIF_FIT); 
	
	window.Open();

	while(!exit_menu)
	{
		int res = DoUI();

		switch(res)
		{
		case UID_LOOPING:
			Game_interface_mode = GAME_DEMO_LOOP;
			strcpy(Demo_fname,Old_demo_fname);
			Demo_looping = true;
			exit_menu = true;
			ret = true;
			break;
		case NEWUIRES_FORCEQUIT:
		case UID_OK:
			exit_menu = true;
			ret = true;
			if ((Players[Player_num].flags & PLAYER_FLAGS_DYING) || (Players[Player_num].flags & PLAYER_FLAGS_DEAD))
			{
				EndPlayerDeath(Player_num); 
				SetFunctionMode(MENU_MODE);
			}
			else
			{
				SetFunctionMode(MENU_MODE);
			}
			break;
		}
	}
	window.Close();
	window.Destroy();
	//Game_paused = false;
	
}

void DemoWriteObjWeapFireFlagChanged(short objnum)
{
	cf_WriteByte(Demo_cfp,DT_WEAP_FIRE_FLAG);	
	cf_WriteShort(Demo_cfp,objnum);
	cf_WriteByte(Demo_cfp,Objects[objnum].weapon_fire_flags);
}

void DemoReadObjWeapFireFlagChanged(void)
{
	short oldobjnum = cf_ReadShort(Demo_cfp);
	short newobjnum = Demo_obj_map[oldobjnum];
	Objects[newobjnum].weapon_fire_flags = cf_ReadByte(Demo_cfp);
}

void DemoWritePlayerInfo(void)
{
	int i;
	for(int a=0;a<MAX_OBJECTS;a++)
	{
		if(Demo_turretchanged[a])
		{
			object *obj = &Objects[a];
			multi_turret multi_turret_info;
			polyobj_info *p_info = &obj->rtype.pobj_info;
			int i;
						
			if((a >= 0) && (obj->type != OBJ_NONE) && (obj->type != OBJ_WEAPON) &&
				(obj->flags & OF_POLYGON_OBJECT) && (p_info->multi_turret_info.num_turrets))
			{
				mprintf((0,"Turret %d updated!\n",a));
				multi_turret_info.keyframes = (float *)&turret_holder;
				ObjGetTurretUpdate(a,&multi_turret_info);

				cf_WriteByte(Demo_cfp, DT_OBJ_TURRET);
				cf_WriteFloat(Demo_cfp,Gametime);	
				cf_WriteShort(Demo_cfp,a);//objnum
				cf_WriteFloat(Demo_cfp,multi_turret_info.time);	
				cf_WriteShort(Demo_cfp,multi_turret_info.num_turrets);
				
				for(i = 0; i < multi_turret_info.num_turrets; i++)
				{
					cf_WriteFloat(Demo_cfp,multi_turret_info.keyframes[i]);				
				}
			}
		}
	}
	for(i=0;i<MAX_OBJECTS;i++)
	{
		Demo_turretchanged[i] = false;
	}
	cf_WriteByte(Demo_cfp,DT_PLAYER_INFO);	
//	cf_WriteFloat(Demo_cfp,Players[Player_num].energy);
	cf_WriteShort(Demo_cfp,Players[Player_num].energy);
//	cf_WriteFloat(Demo_cfp,Objects[Players[Player_num].objnum].shields);
	cf_WriteShort(Demo_cfp,Objects[Players[Player_num].objnum].shields);
	for(i=0;i<MAX_PLAYER_WEAPONS;i++)
	{
		cf_WriteShort(Demo_cfp, Players[Player_num].weapon_ammo[i]);
	}
	//cf_WriteFloat(Demo_cfp,Players[Player_num].weapon[0].firing_time);
	cf_WriteInt(Demo_cfp,Players[Player_num].weapon[0].index);
	//cf_WriteInt(Demo_cfp,Players[Player_num].weapon[0].sound_handle);
	//cf_WriteFloat(Demo_cfp,Players[Player_num].weapon[1].firing_time);
	cf_WriteInt(Demo_cfp,Players[Player_num].weapon[1].index);
	//cf_WriteInt(Demo_cfp,Players[Player_num].weapon[1].sound_handle);
	cf_WriteInt(Demo_cfp,Players[Player_num].weapon_flags);
	cf_WriteFloat(Demo_cfp,Players[Player_num].afterburn_time_left);
	cf_WriteFloat(Demo_cfp,Render_FOV);

}

void DemoReadPlayerInfo(void)
{
	int i;
	Players[Player_num].energy = cf_ReadShort(Demo_cfp);//cf_ReadFloat(Demo_cfp);
	Objects[Players[Player_num].objnum].shields = cf_ReadShort(Demo_cfp);//cf_ReadFloat(Demo_cfp);
	for(i=0;i<MAX_PLAYER_WEAPONS;i++)
	{
		Players[Player_num].weapon_ammo[i] = cf_ReadShort(Demo_cfp);
	}
	//Players[Player_num].weapon[0].firing_time = cf_ReadFloat(Demo_cfp);
	Players[Player_num].weapon[0].index = cf_ReadInt(Demo_cfp);
	//Players[Player_num].weapon[0].sound_handle = cf_ReadInt(Demo_cfp);
	//Players[Player_num].weapon[1].firing_time = cf_ReadFloat(Demo_cfp);
	Players[Player_num].weapon[1].index = cf_ReadInt(Demo_cfp);
	//Players[Player_num].weapon[1].sound_handle = cf_ReadInt(Demo_cfp);
	Players[Player_num].weapon_flags = cf_ReadInt(Demo_cfp);
	Players[Player_num].afterburn_time_left = cf_ReadFloat(Demo_cfp);
		
	if(Viewer_object==Player_object)
	{
		Render_FOV = cf_ReadFloat(Demo_cfp);
	}
	else
	{
		cf_ReadFloat(Demo_cfp);
	}
}


void DemoPlayAutoDemo(void)
{
	char wcard_fname[_MAX_PATH*2];
	char auto_fname[_MAX_PATH*2];
	int idx=0;
	ddio_MakePath(wcard_fname,User_directory,"demo","*.dem",NULL);
	if(ddio_FindFileStart(wcard_fname,auto_fname))
	{
		//Get to the next demo file we are scheduled to do.
		while(idx<Demo_auto_idx)
		{
			if(!ddio_FindNextFile(auto_fname))
			{
				Demo_auto_idx=0;
				ddio_FindFileClose();
				// A little recursion to go back to the start
				DemoPlayAutoDemo();
				return;
			}
			idx++;
		}
		//Now we should have the proper filename		
		if(DemoPlaybackFile(auto_fname))
		{
			Demo_auto_play = true;
		}
		else
		{
			Demo_auto_play = false;
		}
	}
	else
	{
		//No demo files are available, sorry!
		Demo_auto_play = false;
	}
} 

void DemoWritePersistantHUDMessage(ddgr_color color,int x, int y, float time, int flags, int sound_index,char *msg)
{
	cf_WriteByte(Demo_cfp, DT_PERSISTANT_HUD);
	cf_WriteInt(Demo_cfp,(int)color);
	cf_WriteInt(Demo_cfp,(int)x);
	cf_WriteInt(Demo_cfp,(int)y);
	cf_WriteFloat(Demo_cfp,time);
	cf_WriteInt(Demo_cfp,flags);
	cf_WriteInt(Demo_cfp,sound_index);
	cf_WriteShort(Demo_cfp,strlen(msg)+1);
	cf_WriteBytes((const ubyte *)msg,strlen(msg)+1,Demo_cfp);

}

void DemoReadPersistantHUDMessage()
{
	ddgr_color color;
	int x;
	int y;
	float time;
	int flags;
	int sound_index;
	char *fmt;

	color = (unsigned int) cf_ReadInt(Demo_cfp);
	x = cf_ReadInt(Demo_cfp);
	y = cf_ReadInt(Demo_cfp);
	time = cf_ReadFloat(Demo_cfp);
	flags = cf_ReadInt(Demo_cfp);
	sound_index = cf_ReadInt(Demo_cfp);
	int msglen = cf_ReadShort(Demo_cfp);
	fmt = (char *)mem_malloc(msglen);
	cf_ReadBytes((ubyte *)fmt,msglen,Demo_cfp);
	AddPersistentHUDMessage(color,x,y,time,flags,sound_index,fmt);
	mem_free(fmt);
}

void DemoWriteSetObjDead(object *obj)
{
	cf_WriteByte(Demo_cfp, DT_SETOBJDEAD);
	cf_WriteShort(Demo_cfp,OBJNUM(obj));
}

void DemoReadSetObjDead()
{
	short oldobjnum = cf_ReadShort(Demo_cfp);
	short local_objnum = Demo_obj_map[oldobjnum] ;

	Objects[local_objnum].flags|=OF_SERVER_SAYS_DELETE;	
	SetObjectDeadFlag (&Objects[local_objnum]);

	// Clear our lists
	Demo_obj_map[oldobjnum] = 0xffff;
	Server_object_list[oldobjnum]=0xffff;
	Local_object_list[local_objnum]=0xffff;

}

void DemoWritePlayerBalls(int pnum)
{
	cf_WriteByte(Demo_cfp, DT_PLAYERBALLS);

	cf_WriteShort(Demo_cfp,pnum);
	cf_WriteByte(Demo_cfp,Players[pnum].num_balls);

	if(Players[pnum].num_balls>0)
	{
		cf_WriteFloat(Demo_cfp,Players[pnum].ballspeed);
		
		for (int i=0;i<Players[pnum].num_balls;i++)
		{
			cf_WriteFloat(Demo_cfp,Players[pnum].ball_r[i]);
			cf_WriteFloat(Demo_cfp,Players[pnum].ball_g[i]);
			cf_WriteFloat(Demo_cfp,Players[pnum].ball_b[i]);
		}
	}
}

void DemoReadPlayerBalls(void)
{
	int slot = cf_ReadShort(Demo_cfp);
	int num_balls = cf_ReadByte(Demo_cfp);
	float speed;
	float r[4],g[4],b[4];

	ASSERT(num_balls>=0 && num_balls<=4);

	if(num_balls>0)
	{
		speed = cf_ReadFloat(Demo_cfp);
		
		for (int i=0;i<num_balls;i++)
		{
			r[i] = cf_ReadFloat(Demo_cfp);
			g[i] = cf_ReadFloat(Demo_cfp);
			b[i] = cf_ReadFloat(Demo_cfp);
		}
	}else
	{
		speed = 0;
		memset(r,0,sizeof(float)*4);
		memset(g,0,sizeof(float)*4);
		memset(b,0,sizeof(float)*4);
	}

	PlayerSetRotatingBall (slot,num_balls,speed,r,g,b);
}

void DemoWritePlayerTypeChange(int slot,bool stop_observing,int observer_mode,int piggy_objnum)
{
	cf_WriteByte(Demo_cfp, DT_PLAYERTYPECHNG);
	cf_WriteByte(Demo_cfp,slot);
	cf_WriteByte(Demo_cfp,Objects[Players[slot].objnum].type);

	switch(Objects[Players[slot].objnum].type)
	{
	case OBJ_PLAYER:
		cf_WriteByte(Demo_cfp,(stop_observing)?1:0);
		break;
	case OBJ_GHOST:
		break;
	case OBJ_OBSERVER:
		cf_WriteInt(Demo_cfp,observer_mode);

		if(observer_mode==OBSERVER_MODE_PIGGYBACK)
		{
			cf_WriteInt(Demo_cfp,piggy_objnum);	
		}
		break;
	};
}

void DemoReadPlayerTypeChange(void)
{
	int slot;
	int type;

	slot = cf_ReadByte(Demo_cfp);
	type = cf_ReadByte(Demo_cfp);

	switch(type)
	{
	case OBJ_PLAYER:
		if(cf_ReadByte(Demo_cfp))
			PlayerStopObserving (slot);
		else
			MultiMakePlayerReal (slot);
		break;
	case OBJ_GHOST:
		MultiMakePlayerGhost (slot);
		break;
	case OBJ_OBSERVER:
		{
			int obs_mode = cf_ReadInt(Demo_cfp);
			int objnum = -1;

			if(obs_mode==OBSERVER_MODE_PIGGYBACK)
			{
				objnum = Demo_obj_map[cf_ReadInt(Demo_cfp)];
			}

			PlayerSwitchToObserver (slot,obs_mode,objnum);
			
		}break;
	};
}

void DemoWriteObjLifeLeft(object *obj)
{
	if(Demo_flags==DF_RECORDING)
	{
		cf_WriteByte(Demo_cfp, DT_SETOBJLIFELEFT);
		cf_WriteShort(Demo_cfp,OBJNUM(obj));

		if(obj->flags&OF_USES_LIFELEFT)
		{
			cf_WriteByte(Demo_cfp,1);
			cf_WriteFloat(Demo_cfp,obj->lifeleft);
		}else
		{
			cf_WriteByte(Demo_cfp,0);
		}
	}
}

void DemoReadObjLifeLeft(void)
{
	short oldobjnum = cf_ReadShort(Demo_cfp);
	short local_objnum = Demo_obj_map[oldobjnum] ;

	if(cf_ReadByte(Demo_cfp))
	{
		Objects[local_objnum].flags |= OF_USES_LIFELEFT;
		Objects[local_objnum].lifeleft = cf_ReadFloat(Demo_cfp);
	}else
	{
		Objects[local_objnum].flags &= ~OF_USES_LIFELEFT;
		Objects[local_objnum].lifeleft = 0;
	}
}
