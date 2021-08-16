#pragma once
#pragma comment(lib,"ws2_32.lib")
//#include <WinSock2.h>
#include <vector>
#include <string>
#include <algorithm>
#define BUFSIZE 1024
#define CHATSIZE 128
#define BLOCKSIZE 64
#define MAX_SOCKET  10 // 최대 접속 가능한 소켓의 갯수!
#define COMMANDS 9
#define MAXLOADBLOCK 50
#define MAX_USER 10
#define NPC_ID_START MAX_USER + 1
#define MAX_OBJECTS 100000

enum PacketId
{
	RECV, CHATTING, BLOCK, BLOCKWITHCMD, TIMEBLOCK, DESTROY, PLAYER, COMMAND, MODECHANGE, LOAD, PLAYERINFO, LOGINOK, LOGOUT, OP, ATTACK, TRACE
};
#pragma pack(push, 1)
typedef struct ChattingPacket
{
	int packetsize = sizeof(ChattingPacket);
	unsigned char id = CHATTING;
	char chatting[CHATSIZE] = "";
	//std::string chatting = "";
}ChattingPacket;

typedef struct LoginOKPacket
{
	int packetsize = sizeof(LoginOKPacket);
	unsigned char id = LOGINOK;
	int playerindex = 0;
}LoginOKPacket;

typedef struct LogoutPacket
{
	int packetsize = sizeof(LogoutPacket);
	unsigned char id = LOGOUT;
	int playerindex = 0;
}LogoutPacket;

typedef struct BlockPacket
{
	int packetsize = sizeof(BlockPacket);
	unsigned char id = BLOCK;
	int blockindex = 1;
	int block_id = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;
	//int commandblockindex = -1;
	//std::vector<int> commandblockdata;
}BlockPacket;

typedef struct BlockWithCommandPacket
{
	int packetsize = sizeof(BlockWithCommandPacket);
	unsigned char id = BLOCKWITHCMD;
	int blockindex = 1;
	int block_id = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;

	float commandblockindex[COMMANDS] = { -1,-1,-1,-1,-1,-1,-1,-1,-1 };
	float commandblockdata_0[COMMANDS] = { 0, };
	float commandblockdata_1[COMMANDS] = { 0, };
	float commandblockdata_2[COMMANDS] = { 0, };
	float commandblockdata_3[COMMANDS] = { 0, };
}BlockWithCommandPacket;

typedef struct OperationPacket
{
	int packetsize = sizeof(OperationPacket);
	unsigned char id = OP;
	int block_id = 0;
	int op_id = 0;
}OperationPacket;

typedef struct AttackPacket
{
	int packetsize = sizeof(AttackPacket);
	unsigned char id = ATTACK;
	int block_id = 0;
}AttackPacket;

typedef struct TracePacket
{
	int packetsize = sizeof(TracePacket);
	unsigned char id = TRACE;
	int block_id = 0;
	int player_id = 0;
}TracePacket;

typedef struct TimeBlockPacket
{
	int packetsize = sizeof(TimeBlockPacket);
	unsigned char id = TIMEBLOCK;
	int timeblock_id = 0;
	int timetype = 0;
}TimeBlockPacket;

typedef struct LoadPacket
{
	int packetsize = sizeof(LoadPacket);
	unsigned char id = LOAD;
	int blockindex[MAXLOADBLOCK] = { 0, };
	int block_id[MAXLOADBLOCK] = { 0, };
	float blocklocation_x[MAXLOADBLOCK] = { 0, };
	float blocklocation_y[MAXLOADBLOCK] = { 0, };
	float blocklocation_z[MAXLOADBLOCK] = { 0, };
	//int commandblockindex = -1;
	//std::vector<int> commandblockdata;
}LoadPacket;

typedef struct DestroyPacket
{
	int packetsize = sizeof(DestroyPacket);
	unsigned char id = DESTROY;
	int block_id = 0;
	//char todestroyblock[BLOCKSIZE] = "none";
}DestroyPacket;

typedef struct PlayerPacket
{
	int packetsize = sizeof(PlayerPacket);
	unsigned char id = PLAYER;
	int playerindex = 0;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
}PlayerPacket;

typedef struct CommandPacket
{
	int packetsize = sizeof(CommandPacket);
	unsigned char id = COMMAND;
	int commandblock_id = 0;
	//std::vector<int> commandblockindex{ -1 };
	//std::vector<int> commandblockdata_0{ 0 };
	//std::vector<int> commandblockdata_1{ 0 };
	//std::vector<int> commandblockdata_2{ 0 };
	//std::vector<int> commandblockdata_3{ 0 };

	float commandblockindex[COMMANDS] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
	float commandblockdata_0[COMMANDS] = { 0, };
	float commandblockdata_1[COMMANDS] = { 0, };
	float commandblockdata_2[COMMANDS] = { 0, };
	float commandblockdata_3[COMMANDS] = { 0, };
	

}CommandPacket;

typedef struct ModeChangePacket
{
	int packetsize = sizeof(ModeChangePacket);
	unsigned char id = MODECHANGE;
}ModeChangePacket;

typedef struct PlayerInfoPacket
{
	int packetsize = sizeof(PlayerInfoPacket);
	unsigned char id = PLAYERINFO;
	int playerindex = 0;
	int hp = 100;
}PlayerInfoPacket;


#pragma pack(pop)