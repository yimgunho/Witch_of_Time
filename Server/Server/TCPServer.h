#pragma once
#include <string>
#define BUFSIZE 64
typedef struct ChattingPacket
{
	int id = 1;
	char chatting[BUFSIZE] = "none";
}ChattingPacket;

typedef struct BlockPacket
{
	int id = 2;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;

}BlockPacket;

typedef struct DestroyPacket
{
	int id = 3;
	char todestroyblock[BUFSIZE] = "none";
}DestroyPacket;

typedef struct PlayerPacket
{
	int id = 4;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
}PlayerPacket;

typedef struct RecvPacket
{
	int id = 0;
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