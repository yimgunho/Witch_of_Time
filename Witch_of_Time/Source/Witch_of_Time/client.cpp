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
#include <array>
#include <WS2tcpip.h>
#include <MSWSock.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "MSWSock.lib")

//#define SERVERIP "192.168.60.64"
#define SERVERIP "127.0.0.1"

#define SERVERPORT 9000

enum OP_TYPE {
	OP_RECV, OP_SEND
};



struct EX_OVER
{
	WSAOVERLAPPED m_over;
	WSABUF m_wsabuf[1];
	unsigned char m_packetbuf[BUFSIZE];
	OP_TYPE m_op;
};

SOCKET sock;

const TCHAR* chars;
const TCHAR* tempchars;

float Elapsed_Time;

int PositionCnt;

int BlockPositionCnt;
EX_OVER m_recv_over;
int m_prev_size;
HANDLE h_iocp;
void Aclient::send_packet(void* p)
{
	int p_size = reinterpret_cast<int*>(p)[0];
	int p_type = reinterpret_cast<char*>(p)[4];
	EX_OVER* s_over = new EX_OVER;
	s_over->m_op = OP_SEND;
	memset(&s_over->m_over, 0, sizeof(s_over->m_over));
	memcpy(&s_over->m_packetbuf, p, p_size);
	s_over->m_wsabuf[0].buf = reinterpret_cast<CHAR*>(s_over->m_packetbuf);
	s_over->m_wsabuf[0].len = p_size;

	int ret = WSASend(sock, (s_over->m_wsabuf), 1, NULL, 0,
		&s_over->m_over, NULL);

}

void Aclient::do_recv(int s_id)
{
	m_recv_over.m_wsabuf[0].buf =
		reinterpret_cast<char*>(m_recv_over.m_packetbuf) + m_prev_size;
	m_recv_over.m_wsabuf[0].len = BUFSIZE - m_prev_size;
	memset(&m_recv_over.m_over, 0, sizeof(m_recv_over.m_over));
	DWORD r_flag = 0;
	int ret = WSARecv(sock, m_recv_over.m_wsabuf, 1, NULL,
		&r_flag, &m_recv_over.m_over, NULL);
}

void recv_all(SOCKET socket, char* buf, size_t len, int flag)
{
	int total_received = 0;
	while (total_received != len) {
		int ret = recv(socket, buf + total_received, len - total_received, flag);
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



Aclient::Aclient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Elapsed_Time = 0;
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

	players.SetNum(10);
	TempCommandBlockId = -1;
	TempCommandBlockId_recv = -1;

	is_ready = false;
	ready_switch = false;
	is_all_ready = 0;
	id_arr_to_levelEditor.Init(0, MAXLOADBLOCK);
	location_x_arr_to_levelEditor.Init(0, MAXLOADBLOCK);
	location_y_arr_to_levelEditor.Init(0, MAXLOADBLOCK);
	location_z_arr_to_levelEditor.Init(0, MAXLOADBLOCK);
	blockindex_arr_to_levelEditor.Init(0, MAXLOADBLOCK);


	WSADATA wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);

	h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(sock), h_iocp, 0, 0);

	SOCKADDR_IN serveraddr;
	//ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	worker_thread = new std::thread(&Aclient::worker, this);
	do_recv(0);
}

void Aclient::send_block_packet(int blockindex, float block_pos_x, float block_pos_y, float block_pos_z)
{
	BlockPacket blockpacket;
	blockpacket.blockindex = blockindex;
	blockpacket.blocklocation_x = block_pos_x;
	blockpacket.blocklocation_y = block_pos_y;
	blockpacket.blocklocation_z = block_pos_z;

	send_packet(&blockpacket);
}

void Aclient::send_block_with_command_packet(int blockindex, float block_pos_x, float block_pos_y, float block_pos_z,
	TArray<int32> commandindex, TArray<int32> commanddata_0, TArray<int32> commanddata_1, TArray<int32> commanddata_2, TArray<int32> commanddata_3)
{
	BlockWithCommandPacket blockwithcommandpacket;
	blockwithcommandpacket.blockindex = blockindex;
	blockwithcommandpacket.blocklocation_x = block_pos_x;
	blockwithcommandpacket.blocklocation_y = block_pos_y;
	blockwithcommandpacket.blocklocation_z = block_pos_z;
	
	for (int i = 0; i < commandindex.Num(); ++i)
	{
		if (commandindex[i] >= 0)
		{
				blockwithcommandpacket.commandblockindex[i] = commandindex[i];
			if (commanddata_0.IsValidIndex(i) == true)
				blockwithcommandpacket.commandblockdata_0[i] = commanddata_0[i];
			else
				blockwithcommandpacket.commandblockdata_0[i] = -1;
			if (commanddata_1.IsValidIndex(i) == true)
				blockwithcommandpacket.commandblockdata_1[i] = commanddata_1[i];
			else
				blockwithcommandpacket.commandblockdata_1[i] = -1;
			if (commanddata_2.IsValidIndex(i) == true)
				blockwithcommandpacket.commandblockdata_2[i] = commanddata_2[i];
			else
				blockwithcommandpacket.commandblockdata_2[i] = -1;
			if (commanddata_3.IsValidIndex(i) == true)
				blockwithcommandpacket.commandblockdata_3[i] = commanddata_3[i];
			else
				blockwithcommandpacket.commandblockdata_3[i] = -1;
		}
	}

	send_packet(&blockwithcommandpacket);
}

void Aclient::send_command_packet(int block_id, TArray<int32> commandindex, TArray<int32> commanddata_0, TArray<int32> commanddata_1, TArray<int32> commanddata_2, TArray<int32> commanddata_3)
{
	CommandPacket commandpacket;
	commandpacket.commandblock_id = block_id;
	for (int i = 0; i < lengthofcommandlist; ++i)
	{
		if (commandindex[i] >= 0)
		{
			commandpacket.commandblockindex[i] = commandindex[i];
			commandpacket.commandblockdata_0[i] = commanddata_0[i];
			commandpacket.commandblockdata_1[i] = commanddata_1[i];
			commandpacket.commandblockdata_2[i] = commanddata_2[i];
			commandpacket.commandblockdata_3[i] = commanddata_3[i];
		}
	}
	send_packet(&commandpacket);
}

void Aclient::send_destroy_packet(int block_id)
{
	DestroyPacket destroypacket;
	destroypacket.block_id = block_id;
	send_packet(&destroypacket);

}

void Aclient::send_attack_packet(int block_id)
{
	AttackPacket attackpacket;
	attackpacket.block_id = block_id;
	send_packet(&attackpacket);

}
void Aclient::send_chatting_packet(FString chat)
{
	ChattingPacket chattingpacket;
	std::string TempSendString(TCHAR_TO_UTF8(*chat));
	strcpy_s(chattingpacket.chatting, sizeof(chattingpacket.chatting), TempSendString.c_str());
	send_packet(&chattingpacket);
}

void Aclient::send_change_packet()
{
	ModeChangePacket packet;
	send_packet(&packet);
}

void Aclient::send_playerinfo_packet(int hp)
{
	PlayerInfoPacket packet;
	packet.hp = hp;
	send_packet(&packet);
}

void Aclient::send_player_packet(FVector player_pos, FRotator player_angle)
{
	PlayerPacket playerpacket;

	playerpacket.playerlocation_x = player_pos.X;
	playerpacket.playerlocation_y = player_pos.Y;
	playerpacket.playerlocation_z = player_pos.Z;
	playerpacket.angle_x = player_angle.Yaw;
	playerpacket.angle_y = player_angle.Pitch;
	playerpacket.angle_z = player_angle.Roll;
	send_packet(&playerpacket);
}

void Aclient::send_time_packet(int block_id, int time_type)
{
	TimePacket timepacket;

	timepacket.timeblock_id = block_id;
	timepacket.timetype = time_type;

	send_packet(&timepacket);
}

void Aclient::process_packet(int p_id, unsigned char* p_buf)
{
	switch (p_buf[4])
	{
	case LOGINOK:
	{
		auto cast = reinterpret_cast<LoginOKPacket*>(p_buf);
		my_index = cast->playerindex;
	}
	break;
	case LOAD:
	{
		auto cast = reinterpret_cast<LoadPacket*>(p_buf);

		for (int i = 0; i < MAXLOADBLOCK; ++i)
		{
			id_arr_to_levelEditor[i] = cast->block_id[i];
			location_x_arr_to_levelEditor[i] = cast->blocklocation_x[i];
			location_y_arr_to_levelEditor[i] = cast->blocklocation_y[i];
			location_z_arr_to_levelEditor[i] = cast->blocklocation_z[i];
			blockindex_arr_to_levelEditor[i] = cast->blockindex[i];
		}
	}
	break;
	case CHATTING:
	{
		auto cast = reinterpret_cast<ChattingPacket*>(p_buf);

		std::string test(cast->chatting);
		TempRecvStr = (test.c_str());
		receive_chatting();
	}
	break;
	case BLOCK:
	{
		auto cast = reinterpret_cast<BlockPacket*>(p_buf);

		spawn_block(cast->blockindex, cast->block_id, cast->blocklocation_x, cast->blocklocation_y, cast->blocklocation_z);

	}
	break;
	case BLOCKWITHCMD:
	{
		auto cast = reinterpret_cast<BlockWithCommandPacket*>(p_buf);

		TArray<int32> commandblock_index;
		TArray<int32> block_data_0;
		TArray<int32> block_data_1;
		TArray<int32> block_data_2;
		TArray<int32> block_data_3;

		for (int i = 0; i < COMMANDS; ++i)
		{
			if (cast->commandblockindex[i] >= 0) {
				commandblock_index.Insert(cast->commandblockindex[i], i);
				block_data_0.Insert(cast->commandblockdata_0[i], i);
				block_data_1.Insert(cast->commandblockdata_1[i], i);
				block_data_2.Insert(cast->commandblockdata_2[i], i);
				block_data_3.Insert(cast->commandblockdata_3[i], i);
			}
		}

		spawn_with_command_block(cast->blockindex, cast->block_id, cast->blocklocation_x, cast->blocklocation_y, cast->blocklocation_z,
			commandblock_index, block_data_0, block_data_1, block_data_2, block_data_3);

	}
	break;
	case TIME:
	{
		auto cast = reinterpret_cast<TimePacket*>(p_buf);

		casting_magic(cast->timeblock_id, cast->timetype);
	}
	break;
	case DESTROY:
	{
		auto cast = reinterpret_cast<DestroyPacket*>(p_buf);
		int todestroyblockid_recv(cast->block_id);
		FString todestroyblockid_2_FString = FString::FromInt(todestroyblockid_2);
		destroy_block(cast->block_id);
	}
	break;
	case ATTACK:
	{
		auto cast = reinterpret_cast<AttackPacket*>(p_buf);
		attack_monster(cast->block_id);
	}
		break;
	case TRACE:
	{
		auto cast = reinterpret_cast<TracePacket*>(p_buf);
		move_monster(cast->block_id, cast->player_id);
	}
	break;
	case PLAYER:
	{
		auto cast = reinterpret_cast<PlayerPacket*>(p_buf);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (cast->playerindex != my_index)
		{
			players[cast->playerindex].player_ang.Yaw = cast->angle_x;
			players[cast->playerindex].player_ang.Pitch = cast->angle_y;
			players[cast->playerindex].player_ang.Roll = cast->angle_z;
			players[cast->playerindex].player_pos.X = cast->playerlocation_x;
			players[cast->playerindex].player_pos.Y = cast->playerlocation_y;
			players[cast->playerindex].player_pos.Z = cast->playerlocation_z;

			if (IsValid(players[cast->playerindex].playeractor) == true) {
				players[cast->playerindex].playeractor->SetActorLocation(players[cast->playerindex].player_pos);
				players[cast->playerindex].playeractor->SetActorRotation(players[cast->playerindex].player_ang);
			}
			else {
				spawn_dummy_player(cast->playerindex, players[cast->playerindex].player_pos, players[cast->playerindex].player_ang);
			}
		}
		else
		{
			move_player(cast->playerlocation_x, cast->playerlocation_y, cast->playerlocation_z, cast->angle_x, cast->angle_y, cast->angle_z);
		}
	}
	break;
	case COMMAND:
	{
		auto cast = reinterpret_cast<CommandPacket*>(p_buf);
		TempCommandBlockId_recv = cast->commandblock_id;

		commandblockindex_recv.Init(-1, 0);
		commandblockdata_0_recv.Init(0, 0);
		commandblockdata_1_recv.Init(0, 0);
		commandblockdata_2_recv.Init(0, 0);
		commandblockdata_3_recv.Init(0, 0);

		for (int i = 0; i < COMMANDS; ++i)
		{
			if (cast->commandblockindex[i] >= 0)
			{
				commandblockindex_recv.Add(cast->commandblockindex[i]);
				commandblockdata_0_recv.Add(cast->commandblockdata_0[i]);
				commandblockdata_1_recv.Add(cast->commandblockdata_1[i]);
				commandblockdata_2_recv.Add(cast->commandblockdata_2[i]);
				commandblockdata_3_recv.Add(cast->commandblockdata_3[i]);
			}
		}
		paste_commandlist(TempCommandBlockId_recv, commandblockindex_recv, commandblockdata_0_recv, commandblockdata_1_recv, commandblockdata_2_recv, commandblockdata_3_recv);

	}
	break;
	case MODECHANGE:
	{
		auto cast = reinterpret_cast<ModeChangePacket*>(p_buf);
		mode_change();
	}
	break;
	case PLAYERINFO:
	{
		auto cast = reinterpret_cast<PlayerInfoPacket*>(p_buf);
		players[cast->playerindex].player_hp = cast->hp;
		if (cast->playerindex != my_index)
		{
			apply_damage(players[cast->playerindex].playeractor, cast->hp, true);
		}
		else
		{
			apply_damage(nullptr, cast->hp, false);
		}
	}
		break;
	default:
		break;
	}
}

void Aclient::worker()
{
	while (true) {
		DWORD num_bytes;
		ULONG_PTR ikey;
		WSAOVERLAPPED* over;

		BOOL ret = GetQueuedCompletionStatus(h_iocp, &num_bytes, &ikey, &over, INFINITE);
		int key = static_cast<int>(ikey);

		EX_OVER* ex_over = reinterpret_cast<EX_OVER*>(over);


		switch (ex_over->m_op)
		{
		case OP_RECV: {
			unsigned char* packet_ptr = ex_over->m_packetbuf;
			int num_data = num_bytes + m_prev_size;
			int packet_size = packet_ptr[0];

			while (num_data >= packet_size)
			{
				process_packet(key, packet_ptr);
				num_data -= packet_size;
				packet_ptr += packet_size;
				if (0 >= num_data)	break;
				packet_size = packet_ptr[0];
			}
			m_prev_size = num_data;
			if (num_data != 0)	memcpy(ex_over->m_packetbuf, packet_ptr, num_data);

			do_recv(key);
		}
					break;
		case OP_SEND:
			delete ex_over;
			break;
		}
	}
}

// Called every frame
void Aclient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void Aclient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	closesocket(sock);
	WSACleanup();
}



