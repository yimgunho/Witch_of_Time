#pragma once
#pragma comment(lib,"ws2_32.lib")
//#include <WinSock2.h>
#include <vector>
#include <string>
#include <algorithm>
#define BUFSIZE 1024
#define CHATSIZE 256
#define BLOCKSIZE 64



enum PacketId
{
	RECV, CHATTING, BLOCK, DESTROY, PLAYER
};
#pragma pack(push, 1)
typedef struct ChattingPacket
{
	char id = CHATTING;
	int packetsize = sizeof(ChattingPacket);
	char chatting[CHATSIZE] = "none";
}ChattingPacket;

typedef struct BlockPacket
{
	char id = BLOCK;
	int packetsize = sizeof(BlockPacket);
	int blockindex = 0;
	int block_id = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;

}BlockPacket;

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
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
}PlayerPacket;

typedef struct BlockListPacket
{
	int blockindex = 0;
	int block_id = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;

}BlockListPacket;

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