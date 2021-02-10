#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SOCKET  10 // �ִ� ���� ������ ������ ����!
#define BUFSIZE 128
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
			if (buffer[0] == '0')
			{
				WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwTmp, NULL);
				printf("\n");
			}


			
			// �޽��� ����
			memset(buffer, 0, sizeof(buffer));
			int cnt = recv(socket_arry[index], buffer, sizeof(buffer), 0);
			if (cnt <= 0)
			{
				// Ŭ���̾�Ʈ ���� ����
				closesocket(socket_arry[index]);
				printf("Ŭ���̾�Ʈ ���� ����..\n");

				// �迭�� ���� ����
				socket_arry[index] = 0;
				continue;
			}

			// �޽��� ���
			if (buffer[0] == '0')
			{
				WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, cnt, &dwTmp, NULL);
				printf("\n");
			}

			// ���� ó��
			char send_buffer[BUFSIZE] = {};
			//wsprintfA(send_buffer, "%d user(SEND) : %s", index, buffer);
			printf("%d user(SEND) : ", index);
			strcpy_s(send_buffer, buffer);

			//if (send_buffer[0] == '0')
			//{
				for (int c = 1; c < MAX_SOCKET; c++)
				{
					if (socket_arry[c] != 0 && c != index)
					{
						// ���Ź��� Ŭ���̾�Ʈ ���� �ϰ� ���� ó��

						auto i = send(socket_arry[c], send_buffer, strlen(send_buffer), 0);
						WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), send_buffer, strlen(send_buffer), &dwTmp, NULL);
						printf("\n");

					}
				}
			//}

			//else if (buffer[0] == '1')
			//{
			//	for (int c = 1; c < MAX_SOCKET; c++)
			//	{
			//		if (socket_arry[c] != 0 && c != index)
			//		{
			//			// ���Ź��� Ŭ���̾�Ʈ ���� �ϰ� ���� ó��

			//			auto i = send(socket_arry[c], send_buffer, strlen(send_buffer), 0);
			//			WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), send_buffer, strlen(send_buffer), &dwTmp, NULL);
			//			printf("\n");

			//		}
			//	}
			//}

		}//

	}
	// ���� ���� ����
	closesocket(socket_arry[0]);
	CloseHandle(hEvent_arry[0]);
	return 0;
}