#pragma warning (default : 4668)
#include "client.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000


void recv_all(SOCKET sock, char* buf, size_t len, int flag)
{
	int total_received = 0;
	while (total_received != len) {
		int ret = recv(sock, buf + total_received, len - total_received, flag);
		total_received += ret;
	}

}

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

SOCKET sock;

const TCHAR* chars;
const TCHAR* tempchars;

float Elapsed_Time;

int PositionCnt;

int BlockPositionCnt;

Aclient::Aclient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Elapsed_Time = 0;
	cnt = 0;
	recv_cnt = 0;
}

void Aclient::SetBlockIndex(int index)
{
	block_index = index;
}

// Called when the game starts or when spawned
void Aclient::BeginPlay()
{
	Super::BeginPlay();

	TempSendStr = TEXT("");
	TempRecvStr = TEXT("");
	chars = *TempSendStr;

	PositionCnt = 0;
	BlockPositionCnt = 0;

	ToDestroyBlockName_CL = "none";
	ToDestroyBlockName_CL_2 = "none";
	ToDestroyBlock_cnt = 0;

	todestroyblockid = -1;
	todestroyblockid_2 = -1;
	is_changed_mode = false;

	angle_x_recv.Init(0, 10);
	angle_y_recv.Init(0, 10);
	angle_z_recv.Init(0, 10);
	position_x_recv.Init(0, 10);
	position_y_recv.Init(0, 10);
	position_z_recv.Init(0, 10);

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

	char buffer[BUFSIZE];
	ChattingPacket chattingpacket;
	BlockPacket blockpacket;
	DestroyPacket destroypacket;
	PlayerPacket playerpacket;
	RecvPacket recvpacket;

	int len = 0;
	Elapsed_Time += DeltaTime;

	tempchars = *TempSendStr;


	if (/*PositionCnt != 0 && cnt == 0*/TempSendStr != "")
	{
		std::string TempSendString(TCHAR_TO_UTF8(*TempSendStr));

		strcpy_s(chattingpacket.chatting, sizeof(chattingpacket.chatting), TempSendString.c_str());

		send(sock, (char*)&chattingpacket, sizeof(chattingpacket), 0);

		TempSendStr = "";
	}

	else if (ToDestroyBlock_cnt != 0)
	{
		destroypacket.block_id = todestroyblockid;
		

		send(sock, (char*)&destroypacket, sizeof(destroypacket), 0);

		FString todestroyblockid_FString = FString::FromInt(todestroyblockid);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Send " + todestroyblockid_FString);

		ToDestroyBlockName_CL = "none";
		ToDestroyBlock_cnt = 0;
	}

	else if (Block_cnt != 0)
	{
		blockpacket.blockindex = block_index;
		blockpacket.block_id = block_id_CL;
		blockpacket.blocklocation_x = block_position_x;
		blockpacket.blocklocation_y = block_position_y;
		blockpacket.blocklocation_z = block_position_z;

		send(sock, (char*)&blockpacket, sizeof(blockpacket), 0);
		Block_cnt = 0;
	}

	else if (is_moving != 0 || is_changed_mode == true)
	{
		playerpacket.angle_x = angle_x;
		playerpacket.angle_y = angle_y;
		playerpacket.angle_z = angle_z;
		playerpacket.playerlocation_x = position_x;
		playerpacket.playerlocation_y = position_y;
		playerpacket.playerlocation_z = position_z;

		send(sock, (char*)&playerpacket, sizeof(playerpacket), 0);

		if (is_changed_mode == true)
		{
			is_changed_mode = false;
		}
	}


/////////////////////////////////////////////////////////////////////////////////////////////

	recv(sock, &buffer[0], sizeof(char) + sizeof(int), 0);

	switch (buffer[0])
	{
	case CHATTING:
	{
		recv_all(sock, buffer + 5, sizeof(ChattingPacket) - 5, 0);
		auto cast = reinterpret_cast<ChattingPacket*>(buffer);
		
		std::string test(cast->chatting);
		TempRecvStr = (test.c_str());
	}
	break;
	case BLOCK:
	{
		recv_all(sock, buffer + 5, sizeof(BlockPacket) - 5, 0);
		auto cast = reinterpret_cast<BlockPacket*>(buffer);
		block_index = cast->blockindex;
		block_id_CL_2 = cast->block_id;
		block_position_x_2 = cast->blocklocation_x;
		block_position_y_2 = cast->blocklocation_y;
		block_position_z_2 = cast->blocklocation_z;
	}
	break;
	case DESTROY:
	{
		recv_all(sock, buffer + 5, sizeof(DestroyPacket) - 5, 0);
		auto cast = reinterpret_cast<DestroyPacket*>(buffer);
		int todestroyblockid_recv(cast->block_id);
		todestroyblockid_2 = todestroyblockid_recv;

		FString todestroyblockid_2_FString = FString::FromInt(todestroyblockid_2);
	}
	break;
	case PLAYER:
	{
		recv_all(sock, buffer + 5, sizeof(PlayerPacket) - 5, 0);
		auto cast = reinterpret_cast<PlayerPacket*>(buffer);
		
		angle_x_recv[cast->playerindex] = cast->angle_x;
		angle_y_recv[cast->playerindex] = cast->angle_y;
		angle_z_recv[cast->playerindex] = cast->angle_z;
		position_x_recv[cast->playerindex] = cast->playerlocation_x;
		position_y_recv[cast->playerindex] = cast->playerlocation_y;
		position_z_recv[cast->playerindex] = cast->playerlocation_z;
	}
	break;
	default:
		break;
	}


}

void Aclient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	closesocket(sock);
	WSACleanup();
}



