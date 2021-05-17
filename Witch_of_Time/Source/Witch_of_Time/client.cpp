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

//#define SERVERIP "192.168.60.64"
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

	TempCommandBlockId = -1;
	TempCommandBlockId_recv = -1;

	is_ready = false;
	ready_switch = false;
	is_all_ready = 0;

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
	CommandPacket commandpacket;
	RecvPacket recvpacket;
	ModeChangePacket modepacket;

	int len = 0;
	Elapsed_Time += DeltaTime;

	tempchars = *TempSendStr;

	
	if (ready_switch == true)
	{
		modepacket.readycount = int(is_ready);
		send(sock, (char*)&modepacket, sizeof(modepacket), 0);
		ready_switch = false;
	}

	else if (/*PositionCnt != 0 && cnt == 0*/TempSendStr != "")
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
		//blockpacket.commandblockindex = ;
		//blockpacket.
		send(sock, (char*)&blockpacket, sizeof(blockpacket), 0);
		Block_cnt = 0;
	}
	else if (TempCommandBlockId != -1)
	{
		commandpacket.commandblock_id = TempCommandBlockId;
		//for (int index : commandblockindex_CL) {
		//	commandpacket.commandblockindex.push_back(index);
		//}
		//for (int data_0 : commandblockdata_0) {
		//	commandpacket.commandblockdata_0.push_back(data_0);
		//}
		//for (int data_1 : commandblockdata_1) {
		//	commandpacket.commandblockdata_1.push_back(data_1);
		//}
		//for (int data_2 : commandblockdata_2) {
		//	commandpacket.commandblockdata_2.push_back(data_2);
		//}
		//for (int data_3 : commandblockdata_3) {
		//	commandpacket.commandblockdata_3.push_back(data_3);
		//}
		//commandpacket.commandblockindex = commandblockindex_CL[0];
		//commandpacket.commandblockdata_0 = commandblockdata_0[0];
		//commandpacket.commandblockdata_1 = commandblockdata_1[0];
		//commandpacket.commandblockdata_2 = commandblockdata_2[0];
		//commandpacket.commandblockdata_3 = commandblockdata_3[0];

		for (int i = 0; i < lengthofcommandlist; ++i)
		{
			commandpacket.commandblockindex[i] = commandblockindex_CL[i];
			commandpacket.commandblockdata_0[i] = commandblockdata_0[i];
			commandpacket.commandblockdata_1[i]	= commandblockdata_1[i];
			commandpacket.commandblockdata_2[i]	= commandblockdata_2[i];
			commandpacket.commandblockdata_3[i]	= commandblockdata_3[i];
		}

		send(sock, (char*)&commandpacket, sizeof(commandpacket), 0);

		//for (int i = 0; i < lengthofcommandlist; ++i)
		//{
		//	commandpacket.commandblockindex[i] = -1;
		//	commandpacket.commandblockdata_0[i] = 0;
		//	commandpacket.commandblockdata_1[i] = 0;
		//	commandpacket.commandblockdata_2[i] = 0;
		//	commandpacket.commandblockdata_3[i] = 0;
		//}
		TempCommandBlockId = -1;
		commandblockindex_CL.Empty();
		commandblockdata_0.Empty();
		commandblockdata_1.Empty();
		commandblockdata_2.Empty();
		commandblockdata_3.Empty();
	}
	
	else/* if (is_moving != 0 || is_changed_mode == true)*/
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
	case COMMAND:
	{
		recv_all(sock, buffer + 5, sizeof(CommandPacket) - 5, 0);
		auto cast = reinterpret_cast<CommandPacket*>(buffer);

		TempCommandBlockId_recv = cast->commandblock_id;
		commandblockindex_recv.Reserve(9);
		commandblockdata_0_recv.Reserve(9);
		commandblockdata_1_recv.Reserve(9);
		commandblockdata_2_recv.Reserve(9);
		commandblockdata_3_recv.Reserve(9);

		for (int i = 0; i < COMMANDS; ++i)
		{
			commandblockindex_recv.Add(cast->commandblockindex[i]);
			commandblockdata_0_recv.Add(cast->commandblockdata_0[i]);
			commandblockdata_1_recv.Add(cast->commandblockdata_1[i]);
			commandblockdata_2_recv.Add(cast->commandblockdata_2[i]);
			commandblockdata_3_recv.Add(cast->commandblockdata_3[i]);
		}

	}
	break;
	case MODECHANGE:
	{
		recv_all(sock, buffer + 5, sizeof(ModeChangePacket) - 5, 0);
		auto cast = reinterpret_cast<ModeChangePacket*>(buffer);

		//FString allreadystr = FString::FromInt(cast->all_ready_set);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, allreadystr);
		is_all_ready = cast->all_ready_set;

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



