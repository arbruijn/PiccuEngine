#include "cannedcinematicinfo.h"

#include <string.h>

namespace emuabi
{
namespace
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

	struct canned_cinematic_info32
	{
		int32_t type;
		int32_t camera_pathid;
		int32_t target_pathid;
		uint32_t text_to_display;
		int32_t target_objhandle;
		int32_t room;
		float time;
		int32_t object_to_use_for_point;
		vector32 pos;
		matrix32 orient;
	};

	static void decode_vector(vector& dst, const vector32& src)
	{
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}

	static void decode_matrix(matrix& dst, const matrix32& src)
	{
		decode_vector(dst.rvec, src.rvec);
		decode_vector(dst.uvec, src.uvec);
		decode_vector(dst.fvec, src.fvec);
	}

	static void decode_canned_cinematic_info_struct(const canned_cinematic_info32& src, tCannedCinematicInfo& dst, const VmPtrDecoder& vm)
	{
		memset(&dst, 0, sizeof(dst));
		dst.type = src.type;
		dst.camera_pathid = src.camera_pathid;
		dst.target_pathid = src.target_pathid;
		dst.text_to_display = static_cast<char *>(vm.decode_ptr32(src.text_to_display));
		dst.target_objhandle = src.target_objhandle;
		dst.room = src.room;
		dst.time = src.time;
		dst.object_to_use_for_point = src.object_to_use_for_point;
		decode_vector(dst.pos, src.pos);
		decode_matrix(dst.orient, src.orient);
	}
}

void decode_canned_cinematic_info_struct(const void* srcbuf, tCannedCinematicInfo& dst, const VmPtrDecoder& vm)
{
	if (!srcbuf)
	{
		memset(&dst, 0, sizeof(dst));
		return;
	}

	const canned_cinematic_info32& src = *static_cast<const canned_cinematic_info32*>(srcbuf);
	decode_canned_cinematic_info_struct(src, dst, vm);
}
}
