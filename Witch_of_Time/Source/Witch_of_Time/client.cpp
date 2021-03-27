#pragma warning (default : 4668)
#include "client.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 1024

SOCKET sock;

const TCHAR* chars;
const TCHAR* tempchars;

float Elapsed_Time;

int PositionCnt;

int BlockPositionCnt;

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}


	return (len - left);
}

Aclient::Aclient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Elapsed_Time = 0;
	cnt = 0;
	recv_cnt = 0;
}

// Called when the game starts or when spawned
void Aclient::BeginPlay()
{
	Super::BeginPlay();

	int retval;

	TempSendStr = TEXT("STANDBY");
	TempRecvStr = TEXT("TEST");
	chars = *TempSendStr;

	PositionCnt = 0;
	BlockPositionCnt = 0;

	ToDestroyBlockName_CL = "none";
	ToDestroyBlockName_CL_2 = "none";
	ToDestroyBlock_cnt = 0;

	WSADATA wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	int optval = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval));

	SOCKADDR_IN serveraddr;
	//ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
}

// Called every frame
void Aclient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChattingPacket chattingpacket;
	BlockPacket blockpacket;
	DestroyPacket destroypacket;
	PlayerPacket playerpacket;
	RecvPacket recvpacket;

	int len = 0;
	Elapsed_Time += DeltaTime;

	tempchars = *TempSendStr;

	if (PositionCnt != 0 && cnt == 0)
	{
		std::string chatting_string(TCHAR_TO_UTF8(*TempSendStr));
		chattingpacket.chatting = chatting_string;
		//2
		send(sock, (char*)&chattingpacket.id, sizeof(chattingpacket.id), 0);
		send(sock, (char*)&chattingpacket.chatting, sizeof(chattingpacket.chatting), 0);

		cnt++;
	}

	//if (Elapsed_Time > 1.0f)
	//{
	else if (ToDestroyBlock_cnt != 0)
	{
		destroypacket.id = 3;
		std::string ToDestroyBlockName_CL_String(TCHAR_TO_UTF8(*ToDestroyBlockName_CL));
		strcpy_s(destroypacket.todestroyblock, ToDestroyBlockName_CL_String.c_str());
		
		////2
		send(sock, (char*)&destroypacket.id, sizeof(DestroyPacket), 0);
		send(sock, (char*)&destroypacket.todestroyblock, sizeof(DestroyPacket), 0);


		//std::string testdes_str(destroypacket.todestroyblock);
		//FString testdes(testdes_str.c_str());
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, testdes);

		ToDestroyBlockName_CL = "none";
		ToDestroyBlock_cnt = 0;
	}

	else if (Block_cnt != 0)
	{
		blockpacket.id = 2;
		blockpacket.blocklocation_x = block_position_x;
		blockpacket.blocklocation_y = block_position_y;
		blockpacket.blocklocation_z = block_position_z;

		//4
		send(sock, (char*)&blockpacket.id, sizeof(BlockPacket), 0);
		send(sock, (char*)&blockpacket.blocklocation_x, sizeof(BlockPacket), 0);
		send(sock, (char*)&blockpacket.blocklocation_y, sizeof(BlockPacket), 0);
		send(sock, (char*)&blockpacket.blocklocation_z, sizeof(BlockPacket), 0);
		Block_cnt = 0;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(blockpacket.blocklocation_x) + ", " + FString::SanitizeFloat(blockpacket.blocklocation_y) + ", " + FString::SanitizeFloat(blockpacket.blocklocation_z) + ", " + "Block Sent");
	}


/////////////////////////////////////////////////////////////////////////////////////////////
	recv(sock, (char*)&recvpacket.id, sizeof(recvpacket.id), 0);
	//FString IntAsStr = FString::FromInt(recvpacket.id);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, IntAsStr);

	if (recvpacket.id == 1)
	{
		recv(sock, (char*)&recvpacket.chatting, BUFSIZE, 0);

		std::string Chatting_String(recvpacket.todestroyblock);
		FString Chatting_FString(Chatting_String.c_str());
		TempRecvStr = Chatting_FString;
		recv_cnt = 0;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempRecvStr);
		//Elapsed_Time = 0.f;
	}


	else if (recvpacket.id == 2)
	{

		recv(sock, (char*)&recvpacket.blocklocation_x, sizeof(recvpacket.blocklocation_x), 0);
		recv(sock, (char*)&recvpacket.blocklocation_y, sizeof(recvpacket.blocklocation_y), 0);
		recv(sock, (char*)&recvpacket.blocklocation_z, sizeof(recvpacket.blocklocation_z), 0);

		block_position_x_2 = recvpacket.blocklocation_x;
		block_position_y_2 = recvpacket.blocklocation_y;
		block_position_z_2 = recvpacket.blocklocation_z;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(recvpacket.blocklocation_x) + FString::SanitizeFloat(recvpacket.blocklocation_y) + FString::SanitizeFloat(recvpacket.blocklocation_z) + "Block recv");

	}

	else if (recvpacket.id == 3)
	{
		recv(sock, (char*)&recvpacket.todestroyblock, sizeof(recvpacket.todestroyblock), 0);

		std::string todestroyblock_String(recvpacket.todestroyblock);
		FString todestroyblock_FString(todestroyblock_String.c_str());
		ToDestroyBlockName_CL_2 = todestroyblock_FString;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Destroy recv");
	}

	

	else if (recvpacket.id == 4)
	{
		recv(sock, (char*)&recvpacket.angle_x, sizeof(recvpacket.angle_x), 0);
		recv(sock, (char*)&recvpacket.angle_y, sizeof(recvpacket.angle_y), 0);
		recv(sock, (char*)&recvpacket.angle_z, sizeof(recvpacket.angle_z), 0);
		recv(sock, (char*)&recvpacket.playerlocation_x, sizeof(recvpacket.playerlocation_x), 0);
		recv(sock, (char*)&recvpacket.playerlocation_y, sizeof(recvpacket.playerlocation_y), 0);
		recv(sock, (char*)&recvpacket.playerlocation_z, sizeof(recvpacket.playerlocation_z), 0);

		angle_x_2 = recvpacket.angle_x;
		angle_y_2 = recvpacket.angle_y;
		angle_z_2 = recvpacket.angle_z;
		position_x_2 = recvpacket.playerlocation_x;
		position_y_2 = recvpacket.playerlocation_y;
		position_z_2 = recvpacket.playerlocation_z;
		//is_moving_2 = ;
	}


}

void Aclient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	closesocket(sock);
	WSACleanup();
}