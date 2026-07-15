#include "emuabi/msafe.h"
#include "emuabi/msafe_struct32.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

namespace
{
	static std::string g_log;

	static bool FileExists(const char* path)
	{
		FILE* file = fopen(path, "rb");
		if (!file)
			return false;
		fclose(file);
		return true;
	}

	static bool WriteFile(const char* path, const std::string& contents)
	{
		FILE* file = fopen(path, "wb");
		if (!file)
			return false;
		const size_t written = fwrite(contents.data(), 1, contents.size(), file);
		fclose(file);
		return written == contents.size();
	}

	static void LogVector(const char* label, const vector& value)
	{
		std::ostringstream stream;
		stream << label << "=(" << value.x << "," << value.y << "," << value.z << ")\n";
		g_log += stream.str();
	}

	static void LogMatrix(const char* label, const matrix& value)
	{
		LogVector((std::string(label) + ".rvec").c_str(), value.rvec);
		LogVector((std::string(label) + ".uvec").c_str(), value.uvec);
		LogVector((std::string(label) + ".fvec").c_str(), value.fvec);
	}

	static void TestObjectPosition()
	{
	msafe_struct source = {};
		source.objhandle = 101;
		source.pos = {1.25f, -2.5f, 3.75f};
		emuabi::msafe_struct32 encoded = {};
		emuabi::encode_msafe_getvalue_struct(MSAFE_OBJECT_POS, source, &encoded, {});

		msafe_struct decoded = {};
		emuabi::decode_msafe_getvalue_struct(MSAFE_OBJECT_POS, &encoded, decoded, {});
		g_log += "object_pos encoded.objhandle=" + std::to_string(encoded.objhandle) + "\n";
		LogVector("object_pos encoded.pos", {encoded.pos.x, encoded.pos.y, encoded.pos.z});
		g_log += "object_pos decoded.objhandle=" + std::to_string(decoded.objhandle) + "\n";
		LogVector("object_pos decoded.pos", decoded.pos);
	}

	static void TestWorldPosition()
	{
	msafe_struct source = {};
		source.objhandle = 202;
		source.roomnum = 17;
		source.pos = {-1.0f, 2.0f, -3.0f};
		source.orient.rvec = {1.0f, 0.0f, 0.0f};
		source.orient.uvec = {0.0f, 1.0f, 0.0f};
		source.orient.fvec = {0.0f, 0.0f, 1.0f};
		emuabi::msafe_struct32 encoded = {};
		emuabi::encode_msafe_getvalue_struct(MSAFE_OBJECT_WORLD_POSITION, source, &encoded, {});

		msafe_struct decoded = {};
		emuabi::decode_msafe_getvalue_struct(MSAFE_OBJECT_WORLD_POSITION, &encoded, decoded, {});
		g_log += "world_position encoded.objhandle=" + std::to_string(encoded.objhandle) +
			" roomnum=" + std::to_string(encoded.roomnum) + "\n";
		LogVector("world_position encoded.pos", {encoded.pos.x, encoded.pos.y, encoded.pos.z});
		LogVector("world_position encoded.orient.rvec", {encoded.orient.rvec.x, encoded.orient.rvec.y, encoded.orient.rvec.z});
		LogVector("world_position decoded.pos", decoded.pos);
		LogMatrix("world_position decoded.orient", decoded.orient);
		g_log += "world_position decoded.roomnum=" + std::to_string(decoded.roomnum) + "\n";
	}

	static void TestScalarAndMultipleFields()
	{
	msafe_struct source = {};
		source.objhandle = 303;
		source.state = 1;
		source.lifetime = 4.5f;
		emuabi::msafe_struct32 encoded = {};
		emuabi::encode_msafe_getvalue_struct(MSAFE_OBJECT_CLOAK, source, &encoded, {});
		msafe_struct decoded = {};
		emuabi::decode_msafe_getvalue_struct(MSAFE_OBJECT_CLOAK, &encoded, decoded, {});
		g_log += "cloak encoded.objhandle=" + std::to_string(encoded.objhandle) +
			" state=" + std::to_string(encoded.state) + " lifetime=" + std::to_string(encoded.lifetime) + "\n";
		g_log += "cloak decoded.objhandle=" + std::to_string(decoded.objhandle) +
			" state=" + std::to_string(decoded.state) + " lifetime=" + std::to_string(decoded.lifetime) + "\n";

		source = {};
		source.type = 7;
		source.id = 88;
		source.count = 9;
		emuabi::encode_msafe_getvalue_struct(MSAFE_OBJECT_COUNT_TYPE, source, &encoded, {});
		emuabi::decode_msafe_getvalue_struct(MSAFE_OBJECT_COUNT_TYPE, &encoded, decoded, {});
		g_log += "count_type encoded.type=" + std::to_string(encoded.type) +
			" id=" + std::to_string(encoded.id) + " count=" + std::to_string(encoded.count) + "\n";
		g_log += "count_type decoded.type=" + std::to_string(decoded.type) +
			" id=" + std::to_string(decoded.id) + " count=" + std::to_string(decoded.count) + "\n";
	}

	static void TestRoomDoorAndName()
	{
	msafe_struct source = {};
		source.roomnum = 12;
		source.portalnum = 4;
		source.state = 1;
		emuabi::msafe_struct32 encoded = {};
		emuabi::encode_msafe_getvalue_struct(MSAFE_ROOM_PORTAL_RENDER, source, &encoded, {});
		msafe_struct decoded = {};
		emuabi::decode_msafe_getvalue_struct(MSAFE_ROOM_PORTAL_RENDER, &encoded, decoded, {});
		g_log += "portal encoded.roomnum=" + std::to_string(encoded.roomnum) +
			" portalnum=" + std::to_string(encoded.portalnum) +
			" state=" + std::to_string(encoded.state) + "\n";
		g_log += "portal decoded.roomnum=" + std::to_string(decoded.roomnum) +
			" portalnum=" + std::to_string(decoded.portalnum) +
			" state=" + std::to_string(decoded.state) + "\n";

		source = {};
		source.objhandle = 404;
		source.ithandle = 505;
		source.state = 1;
		emuabi::encode_msafe_getvalue_struct(MSAFE_DOOR_OPENABLE, source, &encoded, {});
		emuabi::decode_msafe_getvalue_struct(MSAFE_DOOR_OPENABLE, &encoded, decoded, {});
		g_log += "door encoded.objhandle=" + std::to_string(encoded.objhandle) +
			" ithandle=" + std::to_string(encoded.ithandle) +
			" state=" + std::to_string(encoded.state) + "\n";
		g_log += "door decoded.objhandle=" + std::to_string(decoded.objhandle) +
			" ithandle=" + std::to_string(decoded.ithandle) + "\n";

		source = {};
		source.objhandle = 404;
		source.ithandle = 505;
		source.state = 1;
		std::strncpy(source.name, "ship_alpha", MSAFE_NAME_LENGTH);
		emuabi::encode_msafe_getvalue_struct(MSAFE_MISC_ENABLE_SHIP, source, &encoded, {});
		emuabi::decode_msafe_getvalue_struct(MSAFE_MISC_ENABLE_SHIP, &encoded, decoded, {});
		g_log += "enable_ship encoded.name=" + std::string(encoded.name) +
			" decoded.name=" + std::string(decoded.name) + "\n";
	}

	static void TestNullAndUnknown()
	{
	msafe_struct decoded = {};
		decoded.objhandle = 999;
		emuabi::decode_msafe_getvalue_struct(MSAFE_OBJECT_POS, nullptr, decoded, {});
		g_log += "null_decode.objhandle=" + std::to_string(decoded.objhandle) + "\n";

		msafe_struct source = {};
		source.objhandle = 606;
		emuabi::msafe_struct32 encoded;
		std::memset(&encoded, 0x7f, sizeof(encoded));
		emuabi::encode_msafe_getvalue_struct(-1, source, &encoded, {});
		g_log += "unknown_encode.objhandle=" + std::to_string(encoded.objhandle) +
			" first_byte=" + std::to_string(static_cast<unsigned char*>(static_cast<void*>(&encoded))[0]) + "\n";
	}
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: %s <expected-log> <actual-log>\n", argv[0]);
		return 2;
	}

	TestObjectPosition();
	TestWorldPosition();
	TestScalarAndMultipleFields();
	TestRoomDoorAndName();
	TestNullAndUnknown();

	if (!WriteFile(argv[2], g_log))
	{
		fprintf(stderr, "failed to write actual log: %s\n", argv[2]);
		return 2;
	}

	if (!FileExists(argv[1]))
	{
		if (!WriteFile(argv[1], g_log))
			return 2;
		return 0;
	}

	std::string command = "diff -u \"";
	command += argv[1];
	command += "\" \"";
	command += argv[2];
	command += "\"";
	return system(command.c_str());
}
