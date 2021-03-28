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
	char buffer[BUFSIZE];

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
			if (socket_arry[index] == 0) continue;
			
			char buffer[BUFSIZE];
			memset(buffer, 0, sizeof(buffer));
			recv(socket_arry[index], &buffer[0], sizeof(char) + sizeof(int), 0);
			
			switch (buffer[0])
			{
			case CHATTING:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(ChattingPacket) - 5, 0);
				auto cast = reinterpret_cast<ChattingPacket*>(buffer);

				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(ChattingPacket), 0);
					std::cout << "send_chat" << std::endl;

				}
			}
				break;
			case BLOCK:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(BlockPacket) - 5, 0);
				//std::cout << buffer << std::endl;
				auto cast = reinterpret_cast<BlockPacket*>(buffer);
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(BlockPacket), 0);
					std::cout << "send_block" << std::endl;

				}
			}
				break;
			case DESTROY:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(DestroyPacket) - 5, 0);
				auto cast = reinterpret_cast<DestroyPacket*>(buffer);
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(DestroyPacket), 0);
					std::cout << "send_destroy" << std::endl;

				}
			}
				break;
			case PLAYER:
			{
				recv_all(socket_arry[index], buffer + 5, sizeof(PlayerPacket) - 5, 0);
				auto cast = reinterpret_cast<PlayerPacket*>(buffer);
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (c == index) continue;
					if (0 == socket_arry[c]) continue;

					send(socket_arry[c], buffer, sizeof(PlayerPacket), 0);
					std::cout << "send_player" << std::endl;

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
	return 0;
}