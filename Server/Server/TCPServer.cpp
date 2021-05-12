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
	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	char* filename;
	int length;
	//사용할 포트번호


	// 소켓 배열   다중 클라이언트 접속을 하기위해 배열을 사용.
	SOCKET socket_arry[MAX_SOCKET] = { 0 };   //최대값은 위에서 정의해줌.
	SOCKET socket_client;
	// 대기용 소켓 생성
	socket_arry[0] = socket(AF_INET, SOCK_STREAM, 0);

	//printf("사용하려는 포트번호 : ");
	//scanf("%s", &PORT);

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

	//char* pText = "클라이언트 접속을 기다리고 있습니다.\n";
	printf("클라이언트 접속을 기다리고 있습니다.\n");

	unsigned long noblock = 1;
	int nRet = ioctlsocket(socket_arry[0], FIONBIO, &noblock);
	//char buffer[BUFSIZE];

	//std::vector<BlockListPacket> blockvector;
	//BlockListPacket blocklistpacket;

	std::vector<BlockPacket> blockvector;
	BlockPacket blocklistpacket;
	// 메인 루프
	while (1)
	{
		ChattingPacket chattingpacket;
		BlockPacket blockpacket;
		DestroyPacket destroypacket;
		PlayerPacket playerpacket;
		RecvPacket recvpacket;
		PlayerPacket playerspos[MAX_SOCKET];
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
				// 배열에 소켓 저장
				socket_arry[index] = socket_client;
				unsigned long noblock = 1;
				int nRet = ioctlsocket(socket_arry[index], FIONBIO, &noblock);

				/////////////////////////////////////////////////////////////////
				//recv_all(socket_client, (char*)&length, sizeof(int), 0);
				////if (retval == SOCKET_ERROR) {
				////	err_display("recv()");
				////	break;
				////}
				////else if (retval == 0)
				////	break;

				//// 파일 이름 받기(가변 길이)
				//filename = (char*)malloc(length);
				//if (filename == NULL) {
				//	fprintf(stderr, "malloc() error!\n");
				//	break;
				//}

				//recv_all(socket_client, filename, length, 0);
				////if (retval == SOCKET_ERROR) {
				////	err_display("recv()");
				////	break;
				////}
				//printf("-> 받을 파일 이름: %s\n", filename);

				//// 파일 데이터 길이 받기(고정 길이)
				//recv_all(socket_client, (char*)&length, sizeof(int), 0);
				////if (retval == SOCKET_ERROR) {
				////	err_display("recv()");
				////	break;
				////}
				////else if (retval == 0)
				////	break;

				//printf("-> 받을 파일 크기: %d\n", length);

				//// 파일 열기
				//FILE* fp = fopen(filename, "wb");
				//if (fp == NULL) {
				//	perror("fopen()");
				//	break;
				//}

				//// 파일 데이터 받기(가변 길이)
				//char* buf = (char*)malloc(length);
				//if (buf == NULL) {
				//	fprintf(stderr, "malloc() error!\n");
				//	exit(1);
				//}

				//myRecvn(socket_client, buf, length, 0);
				////if (retval == SOCKET_ERROR) {
				////	err_display("recv()");
				////	break;
				////}
				////else if (retval == 0)
				////	break;
				/////////////////////////////////////////////////////////////////

				//int totalBufferNum;
				//int BufferNum = 0;
				//int readBytes;
				//int file_size;
				//char buf[BUFSIZE];

				//FILE* fp;
				//fp = fopen("card10.png", "wb"); //파일열고 
				//recv(socket_client, buf, BUFSIZE, 0); //파일사이즈받기 
				//file_size = atol(buf); //char->long변환 
				//totalBufferNum = file_size / BUFSIZE + 1;
				////전체사이즈 = 파일전체사이즈 / 받고있는데이터
				//while (BufferNum != totalBufferNum) {
				//	readBytes = recv(socket_client, buf, BUFSIZE, 0);
				//	//데이터와 데이터의크기 받기 
				//	BufferNum++;
				//	fwrite(buf, sizeof(char), readBytes, fp);
				//	//데이터와 데이터의크기만큼 쓰기 
				//}
				////fclose(fp);

				for (int i = 0; i < blockvector.size(); ++i)
				{
					//auto cast = reinterpret_cast<BlockPacket*>(buffer);
					send(socket_arry[index], (char*)&blockvector[i], sizeof(BlockPacket), 0);
					//std::cout << "작동 중" << std::endl;
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
				closesocket(socket_arry[index]);
				socket_arry[index] = 0;
				continue;
			}

			switch (buffer[0])
			{
			case CHATTING:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(ChattingPacket) - 5, 0);
				auto cast = reinterpret_cast<ChattingPacket*>(buffer);

				chattingpacket.id = cast->id;
				chattingpacket.packetsize = cast->packetsize;
				//chattingpacket.chatting = cast->chatting;
				strcpy_s(chattingpacket.chatting, sizeof(chattingpacket.chatting), cast->chatting);


				for (int c = 1; c < MAX_SOCKET; c++)
				{
					//if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(ChattingPacket), 0);
					//std::cout << "send_chat" << std::endl;

				}
			}
				break;
			case BLOCK:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(BlockPacket) - 5, 0);
				//std::cout << buffer << std::endl;
				auto cast = reinterpret_cast<BlockPacket*>(buffer);

				blocklistpacket.id = cast->id;
				blocklistpacket.packetsize = cast->packetsize;
				blocklistpacket.blockindex = cast->blockindex;
				blocklistpacket.block_id = cast->block_id;
				blocklistpacket.blocklocation_x = cast->blocklocation_x;
				blocklistpacket.blocklocation_y = cast->blocklocation_y;
				blocklistpacket.blocklocation_z = cast->blocklocation_z;
				blockvector.push_back(blocklistpacket);

				//for (int i = 0; i < blockvector.size(); ++i)
				//{
				//	std::cout << blockvector[i].id << " ";
				//	std::cout << blockvector[i].packetsize << " ";
				//	std::cout << blockvector[i].blockindex << " ";
				//	std::cout << blockvector[i].block_id << " ";
				//	std::cout << blockvector[i].blocklocation_x << " ";
				//	std::cout << blockvector[i].blocklocation_y << " ";
				//	std::cout << blockvector[i].blocklocation_z << " ";
				//	std::cout << "/ ";
				//}
				//std::cout << std::endl;

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(BlockPacket), 0);
					//std::cout << "send_block" << std::endl;

				}
			}
				break;
			case DESTROY:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(DestroyPacket) - 5, 0);
				auto cast = reinterpret_cast<DestroyPacket*>(buffer);

				for (int i = 0; i < blockvector.size(); ++i)
				{
					if (blockvector[i].block_id == cast->block_id)
					{
						blockvector.erase(blockvector.begin() + i);
						break;
					}
				}

				//for (int i = 0; i < blockvector.size(); ++i)
				//{
				//	std::cout << blockvector[i].id << " ";
				//	std::cout << blockvector[i].packetsize << " ";
				//	std::cout << blockvector[i].blockindex << " ";
				//	std::cout << blockvector[i].block_id << " ";
				//	std::cout << blockvector[i].blocklocation_x << " ";
				//	std::cout << blockvector[i].blocklocation_y << " ";
				//	std::cout << blockvector[i].blocklocation_z << " ";
				//	std::cout << "/ ";
				//}
				//std::cout << std::endl;

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(DestroyPacket), 0);
					//std::cout << "send_destroy" << std::endl;

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
					//std::cout << "send_player" << std::endl;

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