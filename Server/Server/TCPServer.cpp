#include <winsock2.h>
#include "TCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


void recv_all(SOCKET sock, char* buf, size_t len, int flag)
{
	int total_received = 0;
	while (total_received != len) {
		int ret = recv(sock, buf + total_received, len - total_received, flag);
		total_received += ret;
	}
}

int myRecvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, BUFSIZE, flags);
		printf("받은 용량 : %d\n총 남은 용량 : %d\n", received, left);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
		printf(" -- 다운로드 중... %f %%\n\n", (float)((len - left) * 100.f / len));
	}

	return (len - left);
}

int main()
{
	int generated_blockid = 0;

	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	//사용할 포트번호


	// 소켓 배열   다중 클라이언트 접속을 하기위해 배열을 사용.
	SOCKET socket_arry[MAX_SOCKET] = { 0 };   //최대값은 위에서 정의해줌.
	SOCKET socket_client;
	// 대기용 소켓 생성
	socket_arry[0] = socket(AF_INET, SOCK_STREAM, 0);

	// 소켓 주소 정보 작성
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(9000); // 포트 번호를 받아서 사용.

	// 소켓 바인드
	if (bind(socket_arry[0], (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_arry[0]);
		return -1;
	}

	// 소케 대기
	if (listen(socket_arry[0], SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(socket_arry[0]);
		//printf("listen() Error");
		return -1;
	}

	printf("클라이언트 접속을 기다리고 있습니다.\n");

	unsigned long noblock = 1;
	int nRet = ioctlsocket(socket_arry[0], FIONBIO, &noblock);

	std::vector<BlockPacket> blockvector;
	BlockPacket blocklistpacket;
	bool ready_count[MAX_SOCKET] = { 0, };
	int all_ready_count = 0;
	int current_players = 0;
	// 메인 루프
	while (1)
	{
		ChattingPacket chattingpacket;
		BlockPacket blockpacket;
		TimeBlockPacket timeblockpacket;
		DestroyPacket destroypacket;
		PlayerPacket playerpacket;
		RecvPacket recvpacket;
		PlayerPacket playerspos[MAX_SOCKET];
		CommandPacket commandpacket;
		ModeChangePacket modepacket;

		// 소켓 접속 대기
		SOCKADDR_IN clntAddr;
		int clntLen = sizeof(clntAddr);
		socket_client = accept(socket_arry[0], (SOCKADDR*)&clntAddr, &clntLen);

		if (INVALID_SOCKET != socket_client) {

			// 빈 배열 검색
			int index = -1;

			for (int c = 1; c < MAX_SOCKET; c++)
			{
				if (socket_arry[c] == 0)
				{
					index = c;
					break;
				}
			}

			if (index > 0)  //하나라도 접속
			{
				printf("%d번 -> 클라이언트 접속\n", index);
				current_players++;
				//printf("현재 클라이언트 수 %d\n", current_players);
				// 배열에 소켓 저장
				socket_arry[index] = socket_client;
				unsigned long noblock = 1;
				int nRet = ioctlsocket(socket_arry[index], FIONBIO, &noblock);

				for (unsigned int i = 0; i < blockvector.size(); ++i)
				{
					//auto cast = reinterpret_cast<BlockPacket*>(buffer);
					send(socket_arry[index], (char*)&blockvector[i], sizeof(BlockPacket), 0);
				}
			}

			else  //허용 소켓 초과
			{
				printf("더이상 서버에 접속 할수 없습니다..\n");
				closesocket(socket_client);
			}
		}

		// 메시지 수신
		for (int index = 1; index < MAX_SOCKET; ++index) {
			if (socket_arry[index] == 0) continue;
			
			char buffer[BUFSIZE];
			memset(buffer, 0, sizeof(buffer));
			int ret = recv(socket_arry[index], &buffer[0], sizeof(char) + sizeof(int), 0);
			
			if (ret == 0)
			{
				std::cout << index << "번 플레이어가 접속을 종료함" << std::endl;
				current_players--;
				//printf("현재 클라이언트 수 %d\n", current_players);
				closesocket(socket_arry[index]);
				socket_arry[index] = 0;
				continue;
			}

			switch (buffer[0])
			{
			case LOAD:
			{
				std::cout << "loadpacketrecv" << std::endl;
				recv_all(socket_arry[index], buffer + 5, sizeof(LoadPacket) - 5, 0);
				auto cast = reinterpret_cast<LoadPacket*>(buffer);

				std::cout << cast->blocklocation_x[0] << std::endl;

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(LoadPacket), 0);
				}
			}
			break;
			case CHATTING:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(ChattingPacket) - 5, 0);
				auto cast = reinterpret_cast<ChattingPacket*>(buffer);

				chattingpacket.id = cast->id;
				chattingpacket.packetsize = cast->packetsize;
				strcpy_s(chattingpacket.chatting, sizeof(chattingpacket.chatting), cast->chatting);


				for (int c = 1; c < MAX_SOCKET; c++)
				{
					//if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(ChattingPacket), 0);

				}
			}
				break;
			case BLOCK:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(BlockPacket) - 5, 0);
				auto cast = reinterpret_cast<BlockPacket*>(buffer);

				blocklistpacket.id = cast->id;
				blocklistpacket.packetsize = cast->packetsize;
				blocklistpacket.blockindex = cast->blockindex;

				blocklistpacket.block_id = generated_blockid;
				++generated_blockid;

				blocklistpacket.blocklocation_x = cast->blocklocation_x;
				blocklistpacket.blocklocation_y = cast->blocklocation_y;
				blocklistpacket.blocklocation_z = cast->blocklocation_z;
				blockvector.push_back(blocklistpacket);

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					//if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], (char*)&blocklistpacket, sizeof(BlockPacket), 0);
				}
			}
				break;
			case TIMEBLOCK:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(TimeBlockPacket) - 5, 0);
				auto cast = reinterpret_cast<TimeBlockPacket*>(buffer);
				
				std::cout << cast->timeblock_id << ", " << cast->timetype << std::endl;
				timeblockpacket.id = cast->id;
				timeblockpacket.packetsize = cast->packetsize;
				timeblockpacket.timeblock_id = cast->timeblock_id;
				timeblockpacket.timetype = cast->timetype;

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(TimeBlockPacket), 0);
				}
			}
			break;
			case DESTROY:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(DestroyPacket) - 5, 0);
				auto cast = reinterpret_cast<DestroyPacket*>(buffer);

				for (unsigned int i = 0; i < blockvector.size(); ++i)
				{
					if (blockvector[i].block_id == cast->block_id)
					{
						blockvector.erase(blockvector.begin() + i);
						break;
					}
				}

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(DestroyPacket), 0);

				}
			}
				break;
			case PLAYER:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(PlayerPacket) - 5, 0);
				auto cast = reinterpret_cast<PlayerPacket*>(buffer);

				playerpacket.id = cast->id;
				playerpacket.packetsize = cast->packetsize;
				playerpacket.playerindex = index;
				playerpacket.angle_x = cast->angle_x;
				playerpacket.angle_y = cast->angle_y;
				playerpacket.angle_z = cast->angle_z;
				playerpacket.playerlocation_x = cast->playerlocation_x;
				playerpacket.playerlocation_y = cast->playerlocation_y;
				playerpacket.playerlocation_z = cast->playerlocation_z;

				playerspos[index].angle_x = cast->angle_x;
				playerspos[index].angle_y = cast->angle_y;
				playerspos[index].angle_z = cast->angle_z;
				playerspos[index].playerlocation_x = cast->playerlocation_x;
				playerspos[index].playerlocation_y = cast->playerlocation_y;
				playerspos[index].playerlocation_z = cast->playerlocation_z;

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(PlayerPacket), 0);

				}
			}
				break;
			case COMMAND:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(CommandPacket) - 5, 0);
				auto cast = reinterpret_cast<CommandPacket*>(buffer);

				commandpacket.commandblock_id = cast->commandblock_id;
				//std::cout << "블록 id 번호: " << cast->commandblock_id << std::endl;
				//strcpy_s(commandpacket.blockname, sizeof(commandpacket.blockname), cast->blockname);
				//for (int i = 0; i < sizeof(cast->commandblockindex); ++i)
				//{
				//	commandpacket.commandblockindex.emplace_back(cast->commandblockindex[i]);
				//}
				for (int i = 0; i < COMMANDS; ++i)
				{
					commandpacket.commandblockindex[i] = cast->commandblockindex[i];
					commandpacket.commandblockdata_0[i] = cast->commandblockdata_0[i];
					commandpacket.commandblockdata_1[i] = cast->commandblockdata_1[i];
					commandpacket.commandblockdata_2[i] = cast->commandblockdata_2[i];
					commandpacket.commandblockdata_3[i] = cast->commandblockdata_3[i];
				}

				//for (int i = 0; i < COMMANDS; ++i)
				//{
				//	std::cout << i << "번째 데이터" << std::endl;
				//	std::cout << commandpacket.commandblockindex[i] << std::endl;
				//	std::cout << commandpacket.commandblockdata_0[i] << std::endl;
				//	std::cout << commandpacket.commandblockdata_1[i] << std::endl;
				//	std::cout << commandpacket.commandblockdata_2[i] << std::endl;
				//	std::cout << commandpacket.commandblockdata_3[i] << std::endl;
				//	std::cout << "----------------------------------" << std::endl;
				//}

				//for (int i = 0; i < sizeof(commandpacket.commandblockindex); ++i)
				//{
				//	std::cout << commandpacket.commandblockindex[i] << std::endl;
				//}

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(CommandPacket), 0);
				}
			}
				break;
			case MODECHANGE:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(ModeChangePacket) - 5, 0);
				auto cast = reinterpret_cast<ModeChangePacket*>(buffer);

				modepacket.id = cast->id;
				modepacket.packetsize = cast->packetsize;
				modepacket.readycount = 0;
				//std::cout << index << "번 클라이언트 " << cast->readycount << std::endl;
				ready_count[index] = cast->readycount;
				
				int ready_c = 0;
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (ready_count[c] == true)
					{
						ready_c++;
					}
					//std::cout << index << ":" << ready_count[c] << std::endl;
				}
				//std::cout << "ready 상황 " << ready_c << "클라이언트 수 " << current_players << std::endl;
				if (ready_c == current_players)
				{
					modepacket.all_ready_set = 1;
					//std::cout << "all ready!: "<< modepacket.all_ready_set << std::endl;

					for (int c = 1; c < MAX_SOCKET; c++)
					{
						//if (c == index) continue;
						if (0 == socket_arry[c]) continue;
						send(socket_arry[c], (char*)&modepacket, sizeof(ModeChangePacket), 0);
					}

					for (int c = 1; c < MAX_SOCKET; c++)
					{
						ready_count[c] = false;
					}
				}
			}
			break;
			default:
				break;
			}

		}

	}
	// 서버 소켓 해제
	closesocket(socket_arry[0]);
	closesocket(socket_client);

	WSACleanup();
	return 0;
}