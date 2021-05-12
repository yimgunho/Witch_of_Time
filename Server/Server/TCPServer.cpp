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
		printf("���� �뷮 : %d\n�� ���� �뷮 : %d\n", received, left);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
		printf(" -- �ٿ�ε� ��... %f %%\n\n", (float)((len - left) * 100.f / len));
	}

	return (len - left);
}

int main()
{
	// ���� ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);
	char* filename;
	int length;
	//����� ��Ʈ��ȣ


	// ���� �迭   ���� Ŭ���̾�Ʈ ������ �ϱ����� �迭�� ���.
	SOCKET socket_arry[MAX_SOCKET] = { 0 };   //�ִ밪�� ������ ��������.
	SOCKET socket_client;
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
	//char buffer[BUFSIZE];

	//std::vector<BlockListPacket> blockvector;
	//BlockListPacket blocklistpacket;

	std::vector<BlockPacket> blockvector;
	BlockPacket blocklistpacket;
	// ���� ����
	while (1)
	{
		ChattingPacket chattingpacket;
		BlockPacket blockpacket;
		DestroyPacket destroypacket;
		PlayerPacket playerpacket;
		RecvPacket recvpacket;
		PlayerPacket playerspos[MAX_SOCKET];
		// ���� ���� ���
		SOCKADDR_IN clntAddr;
		int clntLen = sizeof(clntAddr);
		socket_client = accept(socket_arry[0], (SOCKADDR*)&clntAddr, &clntLen);

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

				/////////////////////////////////////////////////////////////////
				//recv_all(socket_client, (char*)&length, sizeof(int), 0);
				////if (retval == SOCKET_ERROR) {
				////	err_display("recv()");
				////	break;
				////}
				////else if (retval == 0)
				////	break;

				//// ���� �̸� �ޱ�(���� ����)
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
				//printf("-> ���� ���� �̸�: %s\n", filename);

				//// ���� ������ ���� �ޱ�(���� ����)
				//recv_all(socket_client, (char*)&length, sizeof(int), 0);
				////if (retval == SOCKET_ERROR) {
				////	err_display("recv()");
				////	break;
				////}
				////else if (retval == 0)
				////	break;

				//printf("-> ���� ���� ũ��: %d\n", length);

				//// ���� ����
				//FILE* fp = fopen(filename, "wb");
				//if (fp == NULL) {
				//	perror("fopen()");
				//	break;
				//}

				//// ���� ������ �ޱ�(���� ����)
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
				//fp = fopen("card10.png", "wb"); //���Ͽ��� 
				//recv(socket_client, buf, BUFSIZE, 0); //���ϻ�����ޱ� 
				//file_size = atol(buf); //char->long��ȯ 
				//totalBufferNum = file_size / BUFSIZE + 1;
				////��ü������ = ������ü������ / �ް��ִµ�����
				//while (BufferNum != totalBufferNum) {
				//	readBytes = recv(socket_client, buf, BUFSIZE, 0);
				//	//�����Ϳ� ��������ũ�� �ޱ� 
				//	BufferNum++;
				//	fwrite(buf, sizeof(char), readBytes, fp);
				//	//�����Ϳ� ��������ũ�⸸ŭ ���� 
				//}
				////fclose(fp);

				for (int i = 0; i < blockvector.size(); ++i)
				{
					//auto cast = reinterpret_cast<BlockPacket*>(buffer);
					send(socket_arry[index], (char*)&blockvector[i], sizeof(BlockPacket), 0);
					//std::cout << "�۵� ��" << std::endl;
				}
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
			int ret = recv(socket_arry[index], &buffer[0], sizeof(char) + sizeof(int), 0);
			
			if (ret == 0)
			{
				std::cout << index << "�� �÷��̾ ������ ������" << std::endl;
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
	// ���� ���� ����
	closesocket(socket_arry[0]);
	closesocket(socket_client);

	WSACleanup();
	return 0;
}