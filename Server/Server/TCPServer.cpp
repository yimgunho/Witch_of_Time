#include <winsock2.h>
#include "TCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define MAX_SOCKET  10 // �ִ� ���� ������ ������ ����!
#define BUFSIZE 64

BlockPacket recv_Block(SOCKET sock) {
	int retval;
	int len;

	recv(sock, (char*)&len, sizeof(int), 0); // ������ �ޱ�(���� ����)


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

	recv(sock, (char*)&len, sizeof(int), 0); // ������ �ޱ�(���� ����)


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
	// ���� ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	//����� ��Ʈ��ȣ


	// ���� �迭   ���� Ŭ���̾�Ʈ ������ �ϱ����� �迭�� ���.
	SOCKET socket_arry[MAX_SOCKET] = { 0 };   //�ִ밪�� ������ ��������.
	HANDLE hEvent_arry[MAX_SOCKET] = { 0 };
	char Packet[BUFSIZE] = {};

	// ���� ���� ����
	socket_arry[0] = socket(AF_INET, SOCK_STREAM, 0);

	//printf("����Ϸ��� ��Ʈ��ȣ : ");
	//scanf("%s", &PORT);

	// ���� �ּ� ���� �ۼ�
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi("9000")); // ��Ʈ ��ȣ�� �޾Ƽ� ���.

	// ���� ���ε�
	if (bind(socket_arry[0], (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		closesocket(socket_arry[0]);
		return -1;
	}

	// ���� ���
	if (listen(socket_arry[0], SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(socket_arry[0]);
		//printf("listen() Error");
		return -1;
	}

	// �̺�Ʈ �ڵ� ����
	for (int i = 0; i < MAX_SOCKET; i++)
	{
		hEvent_arry[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hEvent_arry[i] == INVALID_HANDLE_VALUE)
		{
			closesocket(socket_arry[0]);
			printf("���� �̺�Ʈ ���� ����");
			return SOCKET_ERROR;
		}
	}

	// ��� ���� �̺�Ʈ �ڵ� ����
	if (WSAEventSelect(socket_arry[0], hEvent_arry[0], FD_ACCEPT) == SOCKET_ERROR)
	{
		closesocket(socket_arry[0]);
		CloseHandle(hEvent_arry[0]);
		printf("���� �̺�Ʈ ���� ����");
		return SOCKET_ERROR;
	}

	// ���� ���
	DWORD dwTmp;
	//char* pText = "Ŭ���̾�Ʈ ������ ��ٸ��� �ֽ��ϴ�.\n";
	printf("Ŭ���̾�Ʈ ������ ��ٸ��� �ֽ��ϴ�.\n");

	// ������ �̺�Ʈ �ڵ� ����
	int client = 1;

	// ���� ��Ǫ
	while (1)
	{
		ChattingPacket chattingpacket;
		BlockPacket blockpacket;
		DestroyPacket destroypacket;
		PlayerPacket playerpacket;
		RecvPacket recvpacket;
		// ���� ���� ���
		DWORD dwObject = WaitForMultipleObjectsEx(client, hEvent_arry, FALSE, INFINITE, 0);
		if (dwObject == INFINITE) continue;

		if (dwObject == WAIT_OBJECT_0)
		{
			// Ŭ���̾�Ʈ ���� ����
			SOCKADDR_IN clntAddr;
			int clntLen = sizeof(clntAddr);
			SOCKET socket_client = accept(socket_arry[0], (SOCKADDR*)&clntAddr, &clntLen);

			// �� �迭 �˻�
			int index = -1;
			for (int c = 1; c < MAX_SOCKET; c++)
			{
				if (socket_arry[c] == 0)
				{
					index = c;
					break;
				}
			}

			if (index > 0)  //�ϳ��� ����
			{
				// Ŭ���̾�Ʈ �̺�Ʈ �ڵ� ����
				if (WSAEventSelect(socket_client, hEvent_arry[index], FD_READ | FD_CLOSE) == SOCKET_ERROR)
				{
					DWORD dwTmp;
					printf("Ŭ���̾�Ʈ �̺�Ʈ ���� ���� �Ͽ����ϴ�.\n");
					continue;
				}

				char buffer[BUFSIZE] = { 0 };

				printf("%d�� -> Ŭ���̾�Ʈ ����\n", index);

				// �迭�� ���� ����
				socket_arry[index] = socket_client;
				client = max(client, index + 1);
			}
			else  //��� ���� �ʰ�
			{
				DWORD dwTmp;
				printf("���̻� ������ ���� �Ҽ� �����ϴ�..\n");
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


			
			// �޽��� ����
			memset(buffer, 0, sizeof(buffer));
			//int cnt = recv(socket_arry[index], buffer, sizeof(buffer), 0);
			recv(socket_arry[index], (char*)&recvpacket.id, sizeof(recvpacket.id), 0);

			//recv(socket_arry[index], (char*)&recvpacket, sizeof(RecvPacket), 0);

			//printf("%d\n", cnt);
			//if (cnt <= 0)
			//{
			//	// Ŭ���̾�Ʈ ���� ����
			//	closesocket(socket_arry[index]);
			//	printf("Ŭ���̾�Ʈ ���� ����..\n");

			//	// �迭�� ���� ����
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

			// �޽��� ���
			//if (buffer[0] == '0')
			//{
			//	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, cnt, &dwTmp, NULL);
			//	printf("\n");
			//}

			// ���� ó��
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
	// ���� ���� ����
	closesocket(socket_arry[0]);
	CloseHandle(hEvent_arry[0]);
	return 0;
}