#include "TCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <thread>
#include <array>
#include <mutex>
#include <math.h>
#include <atomic>
#include <unordered_map>


#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "MSWSock.lib")

using namespace std;

bool is_play_mode = false;

void recv_all(SOCKET sock, char* buf, size_t len, int flag)
{
	int total_received = 0;
	while (total_received != len) {
		int ret = recv(sock, buf + total_received, len - total_received, flag);
		total_received += ret;
	}
}

enum OBJ_STATE {
	STATE_READY, STATE_CONNECTED, STATE_INGAME
};

enum OP_TYPE {
	OP_RECV, OP_SEND, OP_ACCEPT
};

struct OVERLAPPED_EXTENDED
{
	WSAOVERLAPPED overlapped;
	WSABUF wsabuf[1];
	unsigned char packetbuf[BUFSIZE];
	OP_TYPE op_type;
	SOCKET socket;
};
struct COMMAND_BLOCK
{
	int commandindex;
	vector<int> commandblockdata;
};

struct OBJECT
{
	SOCKET socket;
	mutex state_lock;
	atomic <OBJ_STATE> object_state;
	OVERLAPPED_EXTENDED m_recv_over;
	int prev_packet_size;
	int id;
	int obj_class;

	float x, y, z;
	float angle_x, angle_y, angle_z;
	int hp;

	int block_index;
	vector <COMMAND_BLOCK> command_block;
};

array <OBJECT, MAX_OBJECTS> objects;

HANDLE iocp_handle;

bool ready_count[MAX_USER] = { 0, };
int current_players = 0;
unordered_map<int, int> monster_block_id;

float start_x = 1000, start_y = 10000, start_z = 200;


void display_error(const char* msg, int err_no)
{
	WCHAR* lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wcout << lpMsgBuf << endl;
	LocalFree(lpMsgBuf);

}


void do_recv(int id)
{
	objects[id].m_recv_over.wsabuf[0].buf = reinterpret_cast<char*>(objects[id].m_recv_over.packetbuf) + objects[id].prev_packet_size;
	objects[id].m_recv_over.wsabuf[0].len = BUFSIZE - objects[id].prev_packet_size;

	memset(&objects[id].m_recv_over.overlapped, 0, sizeof(objects[id].m_recv_over.overlapped));
	DWORD r_flag = 0;
	int ret = WSARecv(objects[id].socket, objects[id].m_recv_over.wsabuf, 1, NULL,
		&r_flag, &objects[id].m_recv_over.overlapped, NULL);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			display_error("Error in RecvPacket:", err_no);
	}
}

int get_new_player_id(SOCKET p_socket)
{
	for (int i = 1; i <= MAX_USER ;++i)
	{
		lock_guard<mutex> lg{ objects[i].state_lock };
		if (objects[i].object_state == STATE_READY) {
			objects[i].object_state = STATE_CONNECTED;
			objects[i].socket = p_socket;
			return i;
		}
	}
	return -1;
}

int get_new_block_id()
{
	for (int i = NPC_ID_START; i <= MAX_OBJECTS; ++i)
	{
		lock_guard<mutex> lg{ objects[i].state_lock };
		if (objects[i].object_state == STATE_READY) {
			objects[i].object_state = STATE_INGAME;
			return i;
		}
	}
	return -1;
}

void disconnect(int p_id) {
	{
		if (STATE_READY == objects[p_id].object_state) return;
		closesocket(objects[p_id].socket);
		objects[p_id].object_state = STATE_READY;
		current_players--;
		if (current_players == 0)
		{
			is_play_mode = false;
			for (int i = NPC_ID_START; i <= MAX_OBJECTS; ++i)
			{
				lock_guard<mutex> lg{ objects[i].state_lock };
				if (objects[i].object_state == STATE_INGAME) {
					objects[i].object_state = STATE_READY;
				}
			}
		}
	}
}

void send_packet(int p_id, void* p)
{
	int p_size = reinterpret_cast<unsigned char*>(p)[0];
	OVERLAPPED_EXTENDED* overlapped = new OVERLAPPED_EXTENDED;
	overlapped->op_type = OP_SEND;
	memset(&overlapped->overlapped, 0, sizeof(overlapped->overlapped));
	memcpy(&overlapped->packetbuf, p, p_size);
	overlapped->wsabuf[0].buf = reinterpret_cast<char*>(overlapped->packetbuf);
	overlapped->wsabuf[0].len = p_size;

	int ret = WSASend(objects[p_id].socket, (overlapped->wsabuf), 1, NULL, 0,
		&overlapped->overlapped, NULL);

	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			display_error("Error in SendPacket:", err_no);
	}
}


void Broadcast_Packet(void* data) {
	for (int i = 0; i < NPC_ID_START; i++) {
		if (objects[i].object_state != STATE_INGAME) continue;
		send_packet(i, data);
	}
}

void Broadcast_Packet_Except_me(int p_id, void* data) {
	for (int i = 0; i < NPC_ID_START; i++) {
		if (i == p_id) continue;
		if (objects[i].object_state != STATE_INGAME) continue;
		send_packet(i, data);
	}
}

void process_packet(int p_id, unsigned char* buffer)
{
	switch (buffer[4])
	{
	case LOAD:
	{
		auto cast = reinterpret_cast<LoadPacket*>(buffer);

		Broadcast_Packet(cast);
	}
	break;
	case CHATTING:
	{
		auto cast = reinterpret_cast<ChattingPacket*>(buffer);
		
		string temp = to_string(p_id);
		char name[CHATSIZE];
		strcpy_s(name, "player_");
		strcat(name, temp.c_str());
		char dot[4] = " : ";
		strcat(name, dot);
		strcat(name, cast->chatting);


		ChattingPacket chattingpacket;
		chattingpacket.id = cast->id;
		chattingpacket.packetsize = cast->packetsize;
		strcpy_s(chattingpacket.chatting, sizeof(chattingpacket.chatting), name);

		Broadcast_Packet(&chattingpacket);
		//Broadcast_Packet(cast);
	}
	break;
	case BLOCK:
	{
		auto cast = reinterpret_cast<BlockPacket*>(buffer);

		BlockPacket blocklistpacket;
		blocklistpacket.blockindex = cast->blockindex;
		if (blocklistpacket.blockindex == 76)
		{
			start_x = cast->blocklocation_x;
			start_y = cast->blocklocation_y;
			start_z = cast->blocklocation_z;

			blocklistpacket.blocklocation_x = cast->blocklocation_x;
			blocklistpacket.blocklocation_y = cast->blocklocation_y;
			blocklistpacket.blocklocation_z = cast->blocklocation_z;

			Broadcast_Packet(&blocklistpacket);

			break;
		}

		int blockid = get_new_block_id();
		blocklistpacket.block_id = blockid;

		blocklistpacket.blocklocation_x = cast->blocklocation_x;
		blocklistpacket.blocklocation_y = cast->blocklocation_y;
		blocklistpacket.blocklocation_z = cast->blocklocation_z;

		objects[blockid].block_index = cast->blockindex;
		objects[blockid].x = cast->blocklocation_x;
		objects[blockid].y = cast->blocklocation_y;
		objects[blockid].z = cast->blocklocation_z;

		if (cast->blockindex == 68 || cast->blockindex == 75)
		{
			// 몬스터 블럭일 경우
			objects[blockid].hp = 100;
			monster_block_id[blockid] = cast->blockindex;
		}

		Broadcast_Packet(&blocklistpacket);

	}
	break;
	case BLOCKWITHCMD:
	{
		auto cast = reinterpret_cast<BlockWithCommandPacket*>(buffer);

		BlockWithCommandPacket blocklistpacket;
		blocklistpacket.blockindex = cast->blockindex;
		if (blocklistpacket.blockindex == 76)
		{
			start_x = cast->blocklocation_x;
			start_y = cast->blocklocation_y;
			start_z = cast->blocklocation_z;

			blocklistpacket.blocklocation_x = cast->blocklocation_x;
			blocklistpacket.blocklocation_y = cast->blocklocation_y;
			blocklistpacket.blocklocation_z = cast->blocklocation_z;

			Broadcast_Packet(&blocklistpacket);

			break;
		}

		int blockid = get_new_block_id();
		cast->block_id = blockid;


		objects[blockid].block_index = cast->blockindex;
		objects[blockid].x = cast->blocklocation_x;
		objects[blockid].y = cast->blocklocation_y;
		objects[blockid].z = cast->blocklocation_z;

		if (cast->blockindex == 68 || cast->blockindex == 75)
		{
			// 몬스터 블럭일 경우
			objects[blockid].hp = 100;
			monster_block_id[blockid] = cast->blockindex;
		}

		Broadcast_Packet(cast);

	}
	break;
	case TIME:
	{
		auto cast = reinterpret_cast<TimePacket*>(buffer);

		TimePacket timeblockpacket;

		timeblockpacket.id = cast->id;
		timeblockpacket.packetsize = cast->packetsize;
		timeblockpacket.timeblock_id = cast->timeblock_id;
		timeblockpacket.timetype = cast->timetype;

		Broadcast_Packet(&timeblockpacket);

	}
	break;
	case DESTROY:
	{
		auto cast = reinterpret_cast<DestroyPacket*>(buffer);

		lock_guard<mutex> lg{ objects[cast->block_id].state_lock };
		objects[cast->block_id].object_state = STATE_READY;

		if (monster_block_id.count(cast->block_id))
		{
			// 몬스터 블럭일 경우
			monster_block_id.erase(cast->block_id);
		}

		Broadcast_Packet(cast);

	}
	break;
	case ATTACK:
	{
		auto cast = reinterpret_cast<AttackPacket*>(buffer);
		objects[cast->block_id].hp = 0;
		Broadcast_Packet(cast);
	}
		break;
	case PLAYER:
	{
		auto cast = reinterpret_cast<PlayerPacket*>(buffer);

		PlayerPacket playerpacket;

		playerpacket.id = cast->id;
		playerpacket.packetsize = cast->packetsize;
		playerpacket.playerindex = p_id;
		playerpacket.angle_x = cast->angle_x;
		playerpacket.angle_y = cast->angle_y;
		playerpacket.angle_z = cast->angle_z;
		playerpacket.playerlocation_x = cast->playerlocation_x;
		playerpacket.playerlocation_y = cast->playerlocation_y;
		playerpacket.playerlocation_z = cast->playerlocation_z;

		objects[p_id].angle_x = cast->angle_x;
		objects[p_id].angle_y = cast->angle_y;
		objects[p_id].angle_z = cast->angle_z;
		objects[p_id].x = cast->playerlocation_x;
		objects[p_id].y = cast->playerlocation_y;
		objects[p_id].z = cast->playerlocation_z;

		Broadcast_Packet_Except_me(p_id, &playerpacket);

	}
	break;
	case COMMAND:
	{
		auto cast = reinterpret_cast<CommandPacket*>(buffer);
		CommandPacket commandpacket;

		commandpacket.commandblock_id = cast->commandblock_id;

		for (int i = 0; i < COMMANDS; ++i)
		{
			if (cast->commandblockindex[i] >= 0) 
			{
				commandpacket.commandblockindex[i] = cast->commandblockindex[i];
				commandpacket.commandblockdata_0[i] = cast->commandblockdata_0[i];
				commandpacket.commandblockdata_1[i] = cast->commandblockdata_1[i];
				commandpacket.commandblockdata_2[i] = cast->commandblockdata_2[i];
				commandpacket.commandblockdata_3[i] = cast->commandblockdata_3[i];
			}
		}

		Broadcast_Packet(&commandpacket);

	}
	break;
	case MODECHANGE:
	{
		auto cast = reinterpret_cast<ModeChangePacket*>(buffer);

		ModeChangePacket modepacket;

		modepacket.id = cast->id;
		modepacket.packetsize = cast->packetsize;
		ready_count[p_id] = true;

		int ready_c = 0;
		for (int c = 1; c < MAX_SOCKET; c++)
		{
			if (ready_count[c] == true)
			{
				ready_c++;
			}
		}
		if (ready_c >= current_players)
		{
			Broadcast_Packet(&modepacket);
			for (int c = 1; c < MAX_SOCKET; c++)
			{
				ready_count[c] = false;
			}
			is_play_mode = !is_play_mode;
			if (is_play_mode)
			{
				for (auto i : monster_block_id)
				{
					objects[i.first].hp = 100;
				}
			}
		}
	}
	break;
	case PLAYERINFO:
	{
		auto cast = reinterpret_cast<PlayerInfoPacket*>(buffer);
		objects[p_id].hp = cast->hp;
		cast->playerindex = p_id;

		if (objects[p_id].hp <= 0)
		{
			PlayerPacket playerpacket;
			playerpacket.playerindex = p_id;
			playerpacket.angle_x = 0;
			playerpacket.angle_y = 0;
			playerpacket.angle_z = 0;
			playerpacket.playerlocation_x = start_x;
			playerpacket.playerlocation_y = start_y;
			playerpacket.playerlocation_z = start_z;

			objects[p_id].angle_x = 0;
			objects[p_id].angle_y = 0;
			objects[p_id].angle_z = 0;
			objects[p_id].x = start_x;
			objects[p_id].y = start_y;
			objects[p_id].z = start_z;

			Broadcast_Packet(&playerpacket);

			objects[p_id].hp = 100;
			cast->hp = 100;
		}

		Broadcast_Packet(cast);
	}
		break;
	default:
		//exit(-1);
		break;
	}
}

void worker(HANDLE h_iocp, SOCKET l_socket) {
	while (true) {
		DWORD bytes_recved;
		ULONG_PTR ikey;
		WSAOVERLAPPED* over;

		BOOL ret = GetQueuedCompletionStatus(h_iocp, &bytes_recved, &ikey, &over, INFINITE);
		int key = static_cast<int>(ikey);

		if (FALSE == ret) {
			if (0 == key) {
				display_error("GQCS : ", WSAGetLastError());
				exit(-1);
			}
			else {
				display_error("GQCS : ", WSAGetLastError());
				disconnect(key);
			}
		}

		if ((key != 0) && (0 == bytes_recved)) {
			disconnect(key);
			continue;
		}


		OVERLAPPED_EXTENDED* over_ex = reinterpret_cast<OVERLAPPED_EXTENDED*>(over);

		switch (over_ex->op_type)
		{
		case OP_RECV: {
			unsigned char* packet_ptr = over_ex->packetbuf;
			int data_bytes = bytes_recved + objects[key].prev_packet_size;
			int packet_size = packet_ptr[0];

			while (data_bytes >= packet_size)
			{
				process_packet(key, packet_ptr);
				data_bytes -= packet_size;
				packet_ptr += packet_size;
				if (0 >= data_bytes)	break;
				packet_size = packet_ptr[0];
			}
			objects[key].prev_packet_size = data_bytes;
			if (data_bytes > 0)
				memcpy(over_ex->packetbuf, packet_ptr, data_bytes);

			do_recv(key);
		}
					break;
		case OP_SEND:
			delete over_ex;
			break;
		case OP_ACCEPT:
		{
			int c_id = get_new_player_id(over_ex->socket);
			if (-1 == c_id) {
				closesocket(over_ex->socket);
				break;
			}

			objects[c_id].object_state = STATE_INGAME;
			objects[c_id].m_recv_over.op_type = OP_RECV;
			objects[c_id].prev_packet_size = 0;
			current_players++;

			cout << c_id << "번 플레이어가 접속됨" << endl;


			CreateIoCompletionPort(reinterpret_cast<HANDLE>(over_ex->socket), h_iocp, c_id, 0);

			LoginOKPacket packet;
			packet.playerindex = c_id;

			send_packet(c_id, &packet);

			objects[c_id].m_recv_over.op_type = OP_RECV;
			do_recv(c_id);


			memset(&over_ex->overlapped, 0, sizeof(over_ex->overlapped));
			SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			over_ex->socket = c_socket;
			AcceptEx(l_socket, c_socket, over_ex->packetbuf, 0, 32, 32, NULL, &over_ex->overlapped);
			break;
		}
		}


	}
}

void Tick()
{
	while (1)
	{
		if (!is_play_mode || monster_block_id.empty())
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			continue;
		}
		for (auto monster : monster_block_id)
		{
			if (monster.second == 75) // 바이킹
			{
				TracePacket packet;
				packet.block_id = monster.first;

				float mindistance = 999999;
				int minid = -1;
				for (int i = 1; i <= MAX_USER; i++)
				{
					if (objects[i].object_state != STATE_INGAME) continue;
					float x = powf((objects[i].x - objects[monster.first].x), 2);
					float y = powf((objects[i].y - objects[monster.first].y), 2);
					float z = powf((objects[i].z - objects[monster.first].z), 2);
					float temp = sqrtf(x + y + z);

					if (mindistance > temp)
						mindistance = temp;
					minid = i;
				}
				if (minid != -1 && mindistance < 1000)
				{
					packet.player_id = minid;
					Broadcast_Packet(&packet);
				}
					
			}
		}
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}

int main()
{
	setlocale(LC_ALL, "korean");
	wcout.imbue(locale("korean"));
	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	//사용할 포트번호
	iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	SOCKET l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(l_socket), iocp_handle, 0, 0);

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	listen(l_socket, SOMAXCONN);

	OVERLAPPED_EXTENDED accept_over;
	accept_over.op_type = OP_ACCEPT;
	memset(&accept_over.overlapped, 0, sizeof(accept_over.overlapped));
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	accept_over.socket = c_socket;
	BOOL ret = AcceptEx(l_socket, c_socket, accept_over.packetbuf, 0, 32, 32, NULL, &accept_over.overlapped);
	if (FALSE == ret) {
		int err_num = WSAGetLastError();
		if (err_num != WSA_IO_PENDING)
			display_error("AcceptEX : ", err_num);
	}
	vector <thread> worker_threads;
	thread tick_thread{ Tick };
	for (int i = 0; i < 4; ++i)
	{
		worker_threads.emplace_back(worker, iocp_handle, l_socket);
	}
	for (auto& th : worker_threads)
	{
		th.join();
	}
	tick_thread.join();
	closesocket(l_socket);

	WSACleanup();


}