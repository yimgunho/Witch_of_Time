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
	// ���� ���� ����
	closesocket(socket_arry[0]);
	return 0;
}