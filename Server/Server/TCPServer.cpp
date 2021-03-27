#include <winsock2.h>
#include "TCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define MAX_SOCKET  10 // 최대 접속 가능한 소켓의 갯수!

void recv_all(SOCKET sock, char* buf, size_t len, int flag)
{
	int total_received = 0;
	while (total_received != len) {
		int ret = recv(sock, buf + total_received, len - total_received, flag);
		total_received += ret;
	}
}

int main()
{
	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	//사용할 포트번호


	// 소켓 배열   다중 클라이언트 접속을 하기위해 배열을 사용.
	SOCKET socket_arry[MAX_SOCKET] = { 0 };   //최대값은 위에서 정의해줌.

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


	// 메인 루프
	while (1)
	{
		ChattingPacket chattingpacket;
		BlockPacket blockpacket;
		DestroyPacket destroypacket;
		PlayerPacket playerpacket;
		RecvPacket recvpacket;
		// 소켓 접속 대기


		SOCKADDR_IN clntAddr;
		int clntLen = sizeof(clntAddr);
		SOCKET socket_client = accept(socket_arry[0], (SOCKADDR*)&clntAddr, &clntLen);

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
			}
			else  //허용 소켓 초과
			{
				printf("더이상 서버에 접속 할수 없습니다..\n");
				closesocket(socket_client);
			}
		}

		// 메시지 수신
		for (int index = 1; index < MAX_SOCKET; ++index) {
			int ret = recv(socket_arry[index], (char*)&recvpacket.id, sizeof(recvpacket.id), 0);
			if (SOCKET_ERROR == ret) continue;

			if (recvpacket.id == 1)
			{
				recv_all(socket_arry[index], (char*)&recvpacket.chatting, sizeof(recvpacket.chatting), 0);
			}

			else if (recvpacket.id == 2)
			{
				recv_all(socket_arry[index], (char*)&recvpacket.blocklocation_x, sizeof(recvpacket.blocklocation_x), 0);
				recv_all(socket_arry[index], (char*)&recvpacket.blocklocation_y, sizeof(recvpacket.blocklocation_y), 0);
				recv_all(socket_arry[index], (char*)&recvpacket.blocklocation_z, sizeof(recvpacket.blocklocation_z), 0);

				//printf("%f, %f, %f\n", recvpacket.blocklocation_x, recvpacket.blocklocation_y, recvpacket.blocklocation_z);
			}

			else if (recvpacket.id == 3)
			{
				recv_all(socket_arry[index], (char*)&recvpacket.todestroyblock, BUFSIZE, 0);
				std::string fordebug(recvpacket.todestroyblock);
				//printf("%s\n", fordebug);
				//std::cout << fordebug << std::endl;
			}

			else if (recvpacket.id == 4)
			{
				recv_all(socket_arry[index], (char*)&recvpacket.angle_x, sizeof(recvpacket.angle_x), 0);
				recv_all(socket_arry[index], (char*)&recvpacket.angle_y, sizeof(recvpacket.angle_y), 0);
				recv_all(socket_arry[index], (char*)&recvpacket.angle_z, sizeof(recvpacket.angle_z), 0);
				recv_all(socket_arry[index], (char*)&recvpacket.playerlocation_x, sizeof(recvpacket.playerlocation_x), 0);
				recv_all(socket_arry[index], (char*)&recvpacket.playerlocation_y, sizeof(recvpacket.playerlocation_y), 0);
				recv_all(socket_arry[index], (char*)&recvpacket.playerlocation_z, sizeof(recvpacket.playerlocation_z), 0);
			}



			printf("%d user(SEND) : ", index);

			if (recvpacket.id == 1)
			{
				strcpy_s(chattingpacket.chatting, recvpacket.chatting);
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], (char*)&chattingpacket.id, sizeof(chattingpacket.id), 0);
					send(socket_arry[c], (char*)&chattingpacket.chatting, sizeof(chattingpacket.chatting), 0);
					std::cout << "send_chat" << std::endl;

				}
			}

			else if (recvpacket.id == 2)
			{
				blockpacket.blocklocation_x = recvpacket.blocklocation_x;
				blockpacket.blocklocation_y = recvpacket.blocklocation_y;
				blockpacket.blocklocation_z = recvpacket.blocklocation_z;
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;


					send(socket_arry[c], (char*)&blockpacket.id, sizeof(BlockPacket), 0);
					send(socket_arry[c], (char*)&blockpacket.blocklocation_x, sizeof(blockpacket.blocklocation_x), 0);
					send(socket_arry[c], (char*)&blockpacket.blocklocation_y, sizeof(blockpacket.blocklocation_y), 0);
					send(socket_arry[c], (char*)&blockpacket.blocklocation_z, sizeof(blockpacket.blocklocation_z), 0);
					std::cout << blockpacket.blocklocation_x << ", " << blockpacket.blocklocation_y << ", " << blockpacket.blocklocation_z << ", " << "send_block" << std::endl;
				}
			}

			else if (recvpacket.id == 3)
			{
				strcpy_s(destroypacket.todestroyblock, recvpacket.todestroyblock);
				for (int c = 1; c < MAX_SOCKET; c++)
				{

					if (c == index) continue;
					if (0 == socket_arry[c]) continue;


					//send(socket_arry[c], (const char*)&destroypacket, sizeof(DestroyPacket), 0);

					send(socket_arry[c], (char*)&destroypacket.id, sizeof(destroypacket.id), 0);
					send(socket_arry[c], (char*)&destroypacket.todestroyblock, sizeof(destroypacket.todestroyblock), 0);
					std::cout << "send_destroy" << std::endl;

				}
			}

			else if (recvpacket.id == 4)
			{
				playerpacket.angle_x = recvpacket.angle_x;
				playerpacket.angle_y = recvpacket.angle_y;
				playerpacket.angle_z = recvpacket.angle_z;
				playerpacket.playerlocation_x = recvpacket.playerlocation_x;
				playerpacket.playerlocation_y = recvpacket.playerlocation_y;
				playerpacket.playerlocation_z = recvpacket.playerlocation_z;
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;


					send(socket_arry[c], (char*)&playerpacket.id, sizeof(PlayerPacket), 0);
					send(socket_arry[c], (char*)&playerpacket.angle_x, sizeof(PlayerPacket), 0);
					send(socket_arry[c], (char*)&playerpacket.angle_y, sizeof(PlayerPacket), 0);
					send(socket_arry[c], (char*)&playerpacket.angle_z, sizeof(PlayerPacket), 0);
					send(socket_arry[c], (char*)&playerpacket.playerlocation_x, sizeof(PlayerPacket), 0);
					send(socket_arry[c], (char*)&playerpacket.playerlocation_y, sizeof(PlayerPacket), 0);
					send(socket_arry[c], (char*)&playerpacket.playerlocation_z, sizeof(PlayerPacket), 0);
					std::cout << "send_player" << std::endl;

				}
			}
		}
	}
	// 서버 소켓 해제
	closesocket(socket_arry[0]);
	return 0;
}