#include <winsock2.h>
#include "TCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define MAX_SOCKET  10 // 최대 접속 가능한 소켓의 갯수!
#define BUFSIZE 64

BlockPacket recv_Block(SOCKET sock) {
	int retval;
	int len;

	recv(sock, (char*)&len, sizeof(int), 0); // 데이터 받기(고정 길이)


	int GetSize;
	char suBuffer[BUFSIZE];
	BlockPacket* blockpacket;
	recv(sock, suBuffer, len, 0);


	suBuffer[GetSize] = '\0';
	blockpacket = (BlockPacket*)suBuffer;

	return *blockpacket;
}

DestroyPacket recv_Destroy(SOCKET sock) {
	int retval;
	int len;

	recv(sock, (char*)&len, sizeof(int), 0); // 데이터 받기(고정 길이)


	int GetSize;
	char suBuffer[BUFSIZE];
	DestroyPacket* destroypacket;
	recv(sock, suBuffer, len, 0);


	suBuffer[GetSize] = '\0';
	destroypacket = (DestroyPacket*)suBuffer;

	return *destroypacket;
}

int main()
{
	// 소켓 라이브러리 초기화
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	//사용할 포트번호


	// 소켓 배열   다중 클라이언트 접속을 하기위해 배열을 사용.
	SOCKET socket_arry[MAX_SOCKET] = { 0 };   //최대값은 위에서 정의해줌.
	HANDLE hEvent_arry[MAX_SOCKET] = { 0 };
	char Packet[BUFSIZE] = {};

	// 대기용 소켓 생성
	socket_arry[0] = socket(AF_INET, SOCK_STREAM, 0);

	//printf("사용하려는 포트번호 : ");
	//scanf("%s", &PORT);

	// 소켓 주소 정보 작성
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi("9000")); // 포트 번호를 받아서 사용.

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

	// 이벤트 핸들 생성
	for (int i = 0; i < MAX_SOCKET; i++)
	{
		hEvent_arry[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hEvent_arry[i] == INVALID_HANDLE_VALUE)
		{
			closesocket(socket_arry[0]);
			printf("서버 이벤트 생성 실패");
			return SOCKET_ERROR;
		}
	}

	// 대기 소켓 이벤트 핸들 설정
	if (WSAEventSelect(socket_arry[0], hEvent_arry[0], FD_ACCEPT) == SOCKET_ERROR)
	{
		closesocket(socket_arry[0]);
		CloseHandle(hEvent_arry[0]);
		printf("서버 이벤트 설정 실패");
		return SOCKET_ERROR;
	}

	// 상태 출력
	DWORD dwTmp;
	//char* pText = "클라이언트 접속을 기다리고 있습니다.\n";
	printf("클라이언트 접속을 기다리고 있습니다.\n");

	// 설정된 이벤트 핸들 갯수
	int client = 1;

	// 접속 루푸
	while (1)
	{
		ChattingPacket chattingpacket;
		BlockPacket blockpacket;
		DestroyPacket destroypacket;
		PlayerPacket playerpacket;
		RecvPacket recvpacket;
		// 소켓 접속 대기
		DWORD dwObject = WaitForMultipleObjectsEx(client, hEvent_arry, FALSE, INFINITE, 0);
		if (dwObject == INFINITE) continue;

		if (dwObject == WAIT_OBJECT_0)
		{
			// 클라이언트 소켓 생성
			SOCKADDR_IN clntAddr;
			int clntLen = sizeof(clntAddr);
			SOCKET socket_client = accept(socket_arry[0], (SOCKADDR*)&clntAddr, &clntLen);

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
				// 클라이언트 이벤트 핸들 설정
				if (WSAEventSelect(socket_client, hEvent_arry[index], FD_READ | FD_CLOSE) == SOCKET_ERROR)
				{
					DWORD dwTmp;
					printf("클라이언트 이벤트 설정 실패 하였습니다.\n");
					continue;
				}

				char buffer[BUFSIZE] = { 0 };

				printf("%d번 -> 클라이언트 접속\n", index);

				// 배열에 소켓 저장
				socket_arry[index] = socket_client;
				client = max(client, index + 1);
			}
			else  //허용 소켓 초과
			{
				DWORD dwTmp;
				printf("더이상 서버에 접속 할수 없습니다..\n");
				closesocket(socket_client);
			}
		}
		else
		{
			int index = (dwObject - WAIT_OBJECT_0);

			DWORD dwTmp;
			char buffer[BUFSIZE] = { };

			wsprintfA(buffer, "%d user : ", index);
			//if (buffer[0] == '0')
			//{
			//	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwTmp, NULL);
			//	printf("\n");
			//}


			
			// 메시지 수신
			memset(buffer, 0, sizeof(buffer));
			//int cnt = recv(socket_arry[index], buffer, sizeof(buffer), 0);
			recv(socket_arry[index], (char*)&recvpacket.id, sizeof(recvpacket.id), 0);

			//recv(socket_arry[index], (char*)&recvpacket, sizeof(RecvPacket), 0);

			//printf("%d\n", cnt);
			//if (cnt <= 0)
			//{
			//	// 클라이언트 접속 종료
			//	closesocket(socket_arry[index]);
			//	printf("클라이언트 접속 종료..\n");

			//	// 배열에 소켓 제거
			//	socket_arry[index] = 0;
			//	continue;
			//}

			if (recvpacket.id == 1)
			{
				recv(socket_arry[index], (char*)&recvpacket.chatting, sizeof(recvpacket.chatting), 0);
			}

			else if (recvpacket.id == 2)
			{
				recv(socket_arry[index], (char*)&recvpacket.blocklocation_x, sizeof(recvpacket.blocklocation_x), 0);
				recv(socket_arry[index], (char*)&recvpacket.blocklocation_y, sizeof(recvpacket.blocklocation_y), 0);
				recv(socket_arry[index], (char*)&recvpacket.blocklocation_z, sizeof(recvpacket.blocklocation_z), 0);

				//printf("%f, %f, %f\n", recvpacket.blocklocation_x, recvpacket.blocklocation_y, recvpacket.blocklocation_z);
			}
			
			else if (recvpacket.id == 3)
			{
				recv(socket_arry[index], (char*)&recvpacket.todestroyblock, BUFSIZE, 0);
				std::string fordebug(recvpacket.todestroyblock);
				//printf("%s\n", fordebug);
				//std::cout << fordebug << std::endl;
			}
			
			else if (recvpacket.id == 4)
			{
				recv(socket_arry[index], (char*)&recvpacket.angle_x, sizeof(recvpacket.angle_x), 0);
				recv(socket_arry[index], (char*)&recvpacket.angle_y, sizeof(recvpacket.angle_y), 0);
				recv(socket_arry[index], (char*)&recvpacket.angle_z, sizeof(recvpacket.angle_z), 0);
				recv(socket_arry[index], (char*)&recvpacket.playerlocation_x, sizeof(recvpacket.playerlocation_x), 0);
				recv(socket_arry[index], (char*)&recvpacket.playerlocation_y, sizeof(recvpacket.playerlocation_y), 0);
				recv(socket_arry[index], (char*)&recvpacket.playerlocation_z, sizeof(recvpacket.playerlocation_z), 0);
			}

			//std::cout << recvpacket.id << ", " << recvpacket.blocklocation_x << ", " << recvpacket.blocklocation_y << ", " << recvpacket.blocklocation_z << ", " << recvpacket.todestroyblock << std::endl;
			//std::cout << recvpacket.id << ", " << recvpacket.blocklocation_x << ", " << recvpacket.blocklocation_y << ", " << recvpacket.blocklocation_z << ", " << recvpacket.todestroyblock << std::endl;

			// 메시지 출력
			//if (buffer[0] == '0')
			//{
			//	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, cnt, &dwTmp, NULL);
			//	printf("\n");
			//}

			// 애코 처리
			//wsprintfA(send_buffer, "%d user(SEND) : %s", index, buffer);
			//printf("%d user(SEND) : ", index);


			if (recvpacket.id == 1)
			{
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					//chattingpacket.chatting = recvpacket.chatting;
					strcpy_s(chattingpacket.chatting, recvpacket.chatting);

					if (socket_arry[c] != 0 && c != index)
					{
						//send(socket_arry[c], (const char*)&chattingpacket, sizeof(ChattingPacket), 0);

						send(socket_arry[c], (char*)&chattingpacket.id, sizeof(ChattingPacket), 0);
						send(socket_arry[c], (char*)&chattingpacket.chatting, sizeof(ChattingPacket), 0);
						std::cout << "send_chat" << std::endl;
					}
				}
			}

			else if (recvpacket.id == 2)
			{
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					blockpacket.blocklocation_x = recvpacket.blocklocation_x;
					blockpacket.blocklocation_y = recvpacket.blocklocation_y;
					blockpacket.blocklocation_z = recvpacket.blocklocation_z;

					if (socket_arry[c] != 0 && c != index)
					{
						//send(socket_arry[c], (const char*)&blockpacket, sizeof(BlockPacket), 0);

						send(socket_arry[c], (char*)&blockpacket.id, sizeof(BlockPacket), 0);
						send(socket_arry[c], (char*)&blockpacket.blocklocation_x, sizeof(blockpacket.blocklocation_x), 0);
						send(socket_arry[c], (char*)&blockpacket.blocklocation_y, sizeof(blockpacket.blocklocation_y), 0);
						send(socket_arry[c], (char*)&blockpacket.blocklocation_z, sizeof(blockpacket.blocklocation_z), 0);
						std::cout << blockpacket.blocklocation_x << ", " << blockpacket.blocklocation_y << ", " << blockpacket.blocklocation_z << ", " << "send_block" << std::endl;
					}
				}
			}

			else if (recvpacket.id == 3)
			{
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					//destroypacket.todestroyblock = recvpacket.todestroyblock;
					strcpy_s(destroypacket.todestroyblock, recvpacket.todestroyblock);

					if (socket_arry[c] != 0 && c != index)
					{
						//send(socket_arry[c], (const char*)&destroypacket, sizeof(DestroyPacket), 0);

						send(socket_arry[c], (char*)&destroypacket.id, sizeof(destroypacket.id), 0);
						send(socket_arry[c], (char*)&destroypacket.todestroyblock, sizeof(destroypacket.todestroyblock), 0);
						std::cout << "send_destroy" << std::endl;
					}
				}
			}

			else if (recvpacket.id == 4)
			{
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					playerpacket.angle_x = recvpacket.angle_x;
					playerpacket.angle_y = recvpacket.angle_y;
					playerpacket.angle_z = recvpacket.angle_z;
					playerpacket.playerlocation_x = recvpacket.playerlocation_x;
					playerpacket.playerlocation_y = recvpacket.playerlocation_y;
					playerpacket.playerlocation_z = recvpacket.playerlocation_z;

					if (socket_arry[c] != 0 && c != index)
					{
						//send(socket_arry[c], (const char*)&playerpacket, sizeof(PlayerPacket), 0);

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

		}//

	}
	// 서버 소켓 해제
	closesocket(socket_arry[0]);
	CloseHandle(hEvent_arry[0]);
	return 0;
}