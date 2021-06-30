#pragma once
#pragma comment(lib,"ws2_32.lib")
//#include <WinSock2.h>
#include <vector>
#include <string>
#include <algorithm>
#define BUFSIZE 1024
#define CHATSIZE 256
#define BLOCKSIZE 64
#define MAX_SOCKET  10 // 최대 접속 가능한 소켓의 갯수!
#define COMMANDS 9
#define MAXLOADBLOCK 50

enum PacketId
{
	RECV, CHATTING, BLOCK, TIMEBLOCK, DESTROY, PLAYER, COMMAND, MODECHANGE, LOAD
};
#pragma pack(push, 1)
typedef struct ChattingPacket
{
	char id = CHATTING;
	int packetsize = sizeof(ChattingPacket);
	char chatting[CHATSIZE] = "";
	//std::string chatting = "";
}ChattingPacket;

typedef struct BlockPacket
{
	char id = BLOCK;
	int packetsize = sizeof(BlockPacket);
	int blockindex = 1;
	int block_id = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;
	//int commandblockindex = -1;
	//std::vector<int> commandblockdata;
}BlockPacket;

typedef struct TimeBlockPacket
{
	char id = TIMEBLOCK;
	int packetsize = sizeof(TimeBlockPacket);
	int timeblock_id = 0;
	int timetype = 0;
}TimeBlockPacket;

typedef struct LoadPacket
{
	char id = LOAD;
	int packetsize = sizeof(LoadPacket);
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
	char id = DESTROY;
	int packetsize = sizeof(DestroyPacket);
	int block_id = 0;
	//char todestroyblock[BLOCKSIZE] = "none";
}DestroyPacket;

typedef struct PlayerPacket
{
	char id = PLAYER;
	int packetsize = sizeof(PlayerPacket);
	int playerindex = 0;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
}PlayerPacket;

typedef struct BlockListPacket
{
	int blockindex = 1;
	int block_id = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;

}BlockListPacket;

typedef struct CommandPacket
{
	char id = COMMAND;
	int packetsize = sizeof(CommandPacket);
	int commandblock_id = -1;
	//std::vector<int> commandblockindex{ -1 };
	//std::vector<int> commandblockdata_0{ 0 };
	//std::vector<int> commandblockdata_1{ 0 };
	//std::vector<int> commandblockdata_2{ 0 };
	//std::vector<int> commandblockdata_3{ 0 };

	int commandblockindex[COMMANDS] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int commandblockdata_0[COMMANDS] = { 0, };
	int commandblockdata_1[COMMANDS] = { 0, };
	int commandblockdata_2[COMMANDS] = { 0, };
	int commandblockdata_3[COMMANDS] = { 0, };
	

}CommandPacket;

typedef struct ModeChangePacket
{
	char id = MODECHANGE;
	int packetsize = sizeof(ModeChangePacket);
	int readycount = 0;
	int all_ready_set = 0;
}ModeChangePacket;

typedef struct RecvPacket
{
	char id = RECV;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;
	char chatting[CHATSIZE] = "none";
	char todestroyblock[BLOCKSIZE] = "none";
}RecvPacket;

#pragma pack(pop)