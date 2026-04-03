#include "value_function_test.h"

#include "pserror.h"

namespace
{
	const char* ValueOpName(char op)
	{
		switch (op)
		{
		case VF_SET:
			return "VF_SET";
		case VF_GET:
			return "VF_GET";
		case VF_SET_FLAGS:
			return "VF_SET_FLAGS";
		case VF_CLEAR_FLAGS:
			return "VF_CLEAR_FLAGS";
		default:
			return "VF_UNKNOWN";
		}
	}
}

void AI_Value_Test(int objhandle, char op, char vtype, void* ptr)
{
	switch (vtype)
	{
	case AIV_F_FOV:
		mprintf((0, "AI_Value objhandle=%d op=%d(%s) vtype=%d(AIV_F_FOV) value=%.3f\n",
			objhandle, op, ValueOpName(op), vtype, *static_cast<float*>(ptr)));
		break;
	case AIV_I_TARGET_HANDLE:
		mprintf((0, "AI_Value objhandle=%d op=%d(%s) vtype=%d(AIV_I_TARGET_HANDLE) value=%d\n",
			objhandle, op, ValueOpName(op), vtype, *static_cast<int*>(ptr)));
		break;
	case AIV_I_FLAGS:
		mprintf((0, "AI_Value objhandle=%d op=%d(%s) vtype=%d(AIV_I_FLAGS) value=%d\n",
			objhandle, op, ValueOpName(op), vtype, *static_cast<int*>(ptr)));
		break;
	case AIV_F_MAX_SPEED:
		mprintf((0, "AI_Value objhandle=%d op=%d(%s) vtype=%d(AIV_F_MAX_SPEED) value=%.3f\n",
			objhandle, op, ValueOpName(op), vtype, *static_cast<float*>(ptr)));
		break;
	case AIV_F_AWARENESS:
		mprintf((0, "AI_Value objhandle=%d op=%d(%s) vtype=%d(AIV_F_AWARENESS) value=%.3f\n",
			objhandle, op, ValueOpName(op), vtype, *static_cast<float*>(ptr)));
		break;
	case AIV_I_STATUS_REG:
		mprintf((0, "AI_Value objhandle=%d op=%d(%s) vtype=%d(AIV_I_STATUS_REG) value=%d\n",
			objhandle, op, ValueOpName(op), vtype, *static_cast<int*>(ptr)));
		break;
	default:
		mprintf((0, "AI_Value objhandle=%d op=%d(%s) vtype=%d ptr=%p\n",
			objhandle, op, ValueOpName(op), vtype, ptr));
		break;
	}
}

void LGoal_Value_Test(char op, char vtype, void* ptr, int g_index, int i_index)
{
	switch (vtype)
	{
	case LGSV_I_STATUS:
		mprintf((0, "LGoal_Value op=%d(%s) vtype=%d(LGSV_I_STATUS) g_index=%d i_index=%d value=%d\n",
			op, ValueOpName(op), vtype, g_index, i_index, *static_cast<int*>(ptr)));
		break;
	case LGSSV_B_ITEM_DONE:
		mprintf((0, "LGoal_Value op=%d(%s) vtype=%d(LGSSV_B_ITEM_DONE) g_index=%d i_index=%d value=%d\n",
			op, ValueOpName(op), vtype, g_index, i_index, *static_cast<bool*>(ptr) ? 1 : 0));
		break;
	case LGSV_I_PRIORITY:
		mprintf((0, "LGoal_Value op=%d(%s) vtype=%d(LGSV_I_PRIORITY) g_index=%d i_index=%d value=%d\n",
			op, ValueOpName(op), vtype, g_index, i_index, *static_cast<int*>(ptr)));
		break;
	case LGSV_PC_COMPLETION_MESSAGE:
		mprintf((0, "LGoal_Value op=%d(%s) vtype=%d(LGSV_PC_COMPLETION_MESSAGE) g_index=%d i_index=%d value=\"%s\"\n",
			op, ValueOpName(op), vtype, g_index, i_index, static_cast<char*>(ptr)));
		break;
	case LGV_I_STATUS:
		mprintf((0, "LGoal_Value op=%d(%s) vtype=%d(LGV_I_STATUS) g_index=%d i_index=%d value=%d\n",
			op, ValueOpName(op), vtype, g_index, i_index, *static_cast<int*>(ptr)));
		break;
	default:
		mprintf((0, "LGoal_Value op=%d(%s) vtype=%d g_index=%d i_index=%d ptr=%p\n",
			op, ValueOpName(op), vtype, g_index, i_index, ptr));
		break;
	}
}

void Matcen_Value_Test(int matcen_handle, char op, char vtype, void* ptr, int prod_index)
{
	switch (vtype)
	{
	case MTNV_I_STATUS:
		mprintf((0, "Matcen_Value matcen_handle=%d op=%d(%s) vtype=%d(MTNV_I_STATUS) prod_index=%d value=%d\n",
			matcen_handle, op, ValueOpName(op), vtype, prod_index, *static_cast<int*>(ptr)));
		break;
	case MTNV_I_MAX_PROD:
		mprintf((0, "Matcen_Value matcen_handle=%d op=%d(%s) vtype=%d(MTNV_I_MAX_PROD) prod_index=%d value=%d\n",
			matcen_handle, op, ValueOpName(op), vtype, prod_index, *static_cast<int*>(ptr)));
		break;
	case MTNV_F_PROD_MULTIPLIER:
		mprintf((0, "Matcen_Value matcen_handle=%d op=%d(%s) vtype=%d(MTNV_F_PROD_MULTIPLIER) prod_index=%d value=%.3f\n",
			matcen_handle, op, ValueOpName(op), vtype, prod_index, *static_cast<float*>(ptr)));
		break;
	case MTNV_I_MAX_ALIVE_CHILDREN:
		mprintf((0, "Matcen_Value matcen_handle=%d op=%d(%s) vtype=%d(MTNV_I_MAX_ALIVE_CHILDREN) prod_index=%d value=%d\n",
			matcen_handle, op, ValueOpName(op), vtype, prod_index, *static_cast<int*>(ptr)));
		break;
	default:
		mprintf((0, "Matcen_Value matcen_handle=%d op=%d(%s) vtype=%d prod_index=%d ptr=%p\n",
			matcen_handle, op, ValueOpName(op), vtype, prod_index, ptr));
		break;
	}
}

void Obj_Value_Test(int objhandle, char op, char vtype, void* ptr, int index)
{
	switch (vtype)
	{
	case OBJV_I_TYPE:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_I_TYPE) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, *static_cast<int*>(ptr)));
		break;
	case OBJV_I_ROOMNUM:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_I_ROOMNUM) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, *static_cast<int*>(ptr)));
		break;
	case OBJV_V_POS:
		{
			vector* value = static_cast<vector*>(ptr);
			mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_V_POS) index=%d value=(%.3f,%.3f,%.3f)\n",
				objhandle, op, ValueOpName(op), vtype, index, value->x, value->y, value->z));
		}
		break;
	case OBJV_I_PHYSICS_FLAGS:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_I_PHYSICS_FLAGS) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, *static_cast<int*>(ptr)));
		break;
	case OBJV_C_MOVEMENT_TYPE:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_C_MOVEMENT_TYPE) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, static_cast<int>(*static_cast<char*>(ptr))));
		break;
	case OBJV_V_VELOCITY:
		{
			vector* value = static_cast<vector*>(ptr);
			mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_V_VELOCITY) index=%d value=(%.3f,%.3f,%.3f)\n",
				objhandle, op, ValueOpName(op), vtype, index, value->x, value->y, value->z));
		}
		break;
	case OBJV_I_FLAGS:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_I_FLAGS) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, *static_cast<int*>(ptr)));
		break;
	case OBJV_C_VIRUS_INFECTED:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_C_VIRUS_INFECTED) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, static_cast<int>(*static_cast<char*>(ptr))));
		break;
	case OBJV_C_NEGATIVE_LIGHT:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_C_NEGATIVE_LIGHT) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, static_cast<int>(*static_cast<char*>(ptr))));
		break;
	case OBJV_C_CONTROL_TYPE:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_C_CONTROL_TYPE) index=%d value=%d\n",
			objhandle, op, ValueOpName(op), vtype, index, static_cast<int>(*static_cast<char*>(ptr))));
		break;
	case OBJV_F_ANIM_FRAME:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d(OBJV_F_ANIM_FRAME) index=%d value=%.3f\n",
			objhandle, op, ValueOpName(op), vtype, index, *static_cast<float*>(ptr)));
		break;
	default:
		mprintf((0, "Obj_Value objhandle=%d op=%d(%s) vtype=%d index=%d ptr=%p\n",
			objhandle, op, ValueOpName(op), vtype, index, ptr));
		break;
	}
}

void Player_Value_Test(int obj_handle, char op, char vhandle, void* ptr, int index)
{
	switch (vhandle)
	{
	case PLYV_I_STRIP_WEAPONS:
		mprintf((0, "Player_Value obj_handle=%d op=%d(%s) vhandle=%d(PLYV_I_STRIP_WEAPONS) index=%d value=%d\n",
			obj_handle, op, ValueOpName(op), vhandle, index, *static_cast<int*>(ptr)));
		break;
	default:
		mprintf((0, "Player_Value obj_handle=%d op=%d(%s) vhandle=%d index=%d ptr=%p\n",
			obj_handle, op, ValueOpName(op), vhandle, index, ptr));
		break;
	}
}

void Room_Value_Test(int roomnum, char op, char vhandle, void* ptr, int index)
{
	switch (vhandle)
	{
	case RMSV_V_PATH_PNT:
		{
			vector* value = static_cast<vector*>(ptr);
			mprintf((0, "Room_Value roomnum=%d op=%d(%s) vhandle=%d(RMSV_V_PATH_PNT) index=%d value=(%.3f,%.3f,%.3f)\n",
				roomnum, op, ValueOpName(op), vhandle, index, value->x, value->y, value->z));
		}
		break;
	default:
		mprintf((0, "Room_Value roomnum=%d op=%d(%s) vhandle=%d index=%d ptr=%p\n",
			roomnum, op, ValueOpName(op), vhandle, index, ptr));
		break;
	}
}
