#include <winsock2.h>
#include "TCPServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define MAX_SOCKET  10 // �ִ� ���� ������ ������ ����!

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
	// ���� ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	//����� ��Ʈ��ȣ


	// ���� �迭   ���� Ŭ���̾�Ʈ ������ �ϱ����� �迭�� ���.
	SOCKET socket_arry[MAX_SOCKET] = { 0 };   //�ִ밪�� ������ ��������.

	// ���� ���� ����
	socket_arry[0] = socket(AF_INET, SOCK_STREAM, 0);

	//printf("����Ϸ��� ��Ʈ��ȣ : ");
	//scanf("%s", &PORT);

	// ���� �ּ� ���� �ۼ�
	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(9000); // ��Ʈ ��ȣ�� �޾Ƽ� ���.

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

	//char* pText = "Ŭ���̾�Ʈ ������ ��ٸ��� �ֽ��ϴ�.\n";
	printf("Ŭ���̾�Ʈ ������ ��ٸ��� �ֽ��ϴ�.\n");

	unsigned long noblock = 1;
	int nRet = ioctlsocket(socket_arry[0], FIONBIO, &noblock);
	char buffer[BUFSIZE];

	// ���� ����
	while (1)
	{
		ChattingPacket chattingpacket;
		BlockPacket blockpacket;
		DestroyPacket destroypacket;
		PlayerPacket playerpacket;
		RecvPacket recvpacket;
		// ���� ���� ���


		SOCKADDR_IN clntAddr;
		int clntLen = sizeof(clntAddr);
		SOCKET socket_client = accept(socket_arry[0], (SOCKADDR*)&clntAddr, &clntLen);

		if (INVALID_SOCKET != socket_client) {

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
				printf("%d�� -> Ŭ���̾�Ʈ ����\n", index);
				// �迭�� ���� ����
				socket_arry[index] = socket_client;
				unsigned long noblock = 1;
				int nRet = ioctlsocket(socket_arry[index], FIONBIO, &noblock);
			}
			else  //��� ���� �ʰ�
			{
				printf("���̻� ������ ���� �Ҽ� �����ϴ�..\n");
				closesocket(socket_client);
			}
		}

		// �޽��� ����
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
	// ���� ���� ����
	closesocket(socket_arry[0]);
	return 0;
}