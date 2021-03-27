#pragma once
#include <string>
#define BUFSIZE 1024
enum PacketId
{
	RECV, CHATTING, BLOCK, DESTROY, PLAYER
};
typedef struct ChattingPacket
{
	int id = CHATTING;
	char chatting[BUFSIZE] = "none";
}ChattingPacket;

typedef struct BlockPacket
{
	int id = BLOCK;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;

}BlockPacket;

typedef struct DestroyPacket
{
	int id = DESTROY;
	char todestroyblock[BUFSIZE] = "none";
}DestroyPacket;

typedef struct PlayerPacket
{
	int id = PLAYER;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
}PlayerPacket;

typedef struct RecvPacket
{
	int id = RECV;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;
	char chatting[BUFSIZE] = "none";
	char todestroyblock[BUFSIZE] = "none";
}RecvPacket;