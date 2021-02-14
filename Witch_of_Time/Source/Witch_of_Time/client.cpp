#pragma warning (default : 4668)
#include "client.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

//DWORD WINAPI SendThread(LPVOID lpData);
//DWORD WINAPI RecvThread(LPVOID lpData);

SOCKET sock;

char* SendResult;
char RecvResult[BUFSIZE];
char ChatPacket[BUFSIZE] = {};
char PositionPacket[BUFSIZE] = {};

char Angle_x_char[BUFSIZE] = {};
char Angle_y_char[BUFSIZE] = {};
char Angle_z_char[BUFSIZE] = {};
char Position_x_char[BUFSIZE] = {};
char Position_y_char[BUFSIZE] = {};
char Position_z_char[BUFSIZE] = {};
char is_moving_char[BUFSIZE] = {};



const TCHAR* chars;
const TCHAR* tempchars;

float Elapsed_Time;

int PositionCnt;

// Sets default values
Aclient::Aclient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetChat();
	Elapsed_Time = 0;
	////¼ÒÄÏ ÇØÁ¦
	//closesocket(sock);
	cnt = 0;
	recv_cnt = 0;
}

// Called when the game starts or when spawned
void Aclient::BeginPlay()
{
	Super::BeginPlay();



	SetChat();

	int retval;

	TempSendStr = TEXT("STANDBY");
	TempRecvStr = TEXT("TEST");
	chars = *TempSendStr;

	PositionCnt = 0;

	WSADATA wsaData;
	int token = WSAStartup(WINSOCK_VERSION, &wsaData);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	u_long on = 1;
	retval = ioctlsocket(sock, FIONBIO, &on);

	int optval = 10;
	retval = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval));

	SOCKADDR_IN serveraddr;
	//ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	//CreateThread(NULL, 0, SendThread, (void*)sock, 0, NULL);

	// Àü¼Û & ¼ö½Å ·çÇª
	//while (1)
	//{
	//	int len = recv(sock, RecvResult, sizeof(RecvResult), 0);
	//	TempRecvStr = FString(ANSI_TO_TCHAR(RecvResult));
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempRecvStr);
	//	if (len <= 0)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "OUT");
	//		break;
	//	}
	//}
}

DWORD WINAPI SendThread(void* lpData)
{
	SOCKET socket_client = (SOCKET)lpData;
	//while (1)
	//if (tempchars != "DEL")
	//{
		//char text[1024] = { 0 };
		//send(socket_client, text, strlen(text), 0);
	SendResult = TCHAR_TO_ANSI(chars);
	send(sock, SendResult, strlen(SendResult), 0);
	//}
	return 0;
}

// Called every frame
void Aclient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Elapsed_Time += DeltaTime;

	tempchars = *TempSendStr;

	char SendChatID[BUFSIZE] = "0";
	char SendPositionID[BUFSIZE] = " ";

		if (PositionCnt != 0 && cnt == 0)
		{
			//º¸³»±â	
			SendResult = TCHAR_TO_ANSI(*TempSendStr);
			strcat(SendChatID, SendResult);
			strcpy_s(ChatPacket, SendChatID);
			send(sock, ChatPacket, strlen(ChatPacket), 0);
			//send(sock, SendResult, strlen(SendResult), 0);
			//memset(ChatPacket, 0, sizeof(ChatPacket));

			cnt++;
		}

		else if (Elapsed_Time > 0.1f && (PositionCnt == 0 || is_moving == 1))
		{

			//ÁÂÇ¥¸¦ char·Î ¹Ù²Þ
			sprintf(Angle_x_char, "%f", angle_x);
			sprintf(Angle_y_char, "%f", angle_y);
			sprintf(Angle_z_char, "%f", angle_z);
			sprintf(Position_x_char, "%f", position_x);
			sprintf(Position_y_char, "%f", position_y);
			sprintf(Position_z_char, "%f", position_z);
			sprintf(is_moving_char, "%d", is_moving);

			//¾Õ¿¡ ÁÂÇ¥¸¦ º¸³Â´Ù´Â ID¸¦ ºÙÈû
			strcat(SendPositionID, Angle_x_char);
			strcat(SendPositionID, ",");
			strcat(SendPositionID, Angle_y_char);
			strcat(SendPositionID, ",");
			strcat(SendPositionID, Angle_z_char);
			strcat(SendPositionID, ",");
			strcat(SendPositionID, Position_x_char);
			strcat(SendPositionID, ",");
			strcat(SendPositionID, Position_y_char);
			strcat(SendPositionID, ",");
			strcat(SendPositionID, Position_z_char);
			strcat(SendPositionID, ",");
			strcat(SendPositionID, is_moving_char);

			//ÁÂÇ¥ ÆÐÅ¶¿¡ º¹»ç
			strcpy_s(PositionPacket, SendPositionID);
			//º¸³¿
			send(sock, PositionPacket, strlen(PositionPacket), 0);

			//memset(PositionPacket, 0, sizeof(PositionPacket));


		Elapsed_Time = 0.f;
		if (PositionCnt == 0)
		{
			PositionCnt++;
		}

		}
	
	memset(ChatPacket, 0, sizeof(ChatPacket));
	memset(PositionPacket, 0, sizeof(PositionPacket));

	recv(sock, RecvResult, sizeof(RecvResult), 0);

	if(RecvResult[0] == '0')
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, RecvResult);

		RecvResult[0] = ' ';
		TempRecvStr = FString(ANSI_TO_TCHAR(RecvResult));
		if (TempRecvStr != "0" || TempRecvStr != "0 ")
		{
			recv_cnt = 0;
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempRecvStr);

		//memset(RecvResult, 0, sizeof(RecvResult));
		//Elapsed_Time = 0.f;
	}

	else if (RecvResult[0] == ' ')
	{

		string a(RecvResult);
		istringstream ss(a);
		string stringbuffer;
		vector<string> x;
		x.clear();
			
		while (getline(ss, stringbuffer, ','))
		{
			x.push_back(stringbuffer);
			cout << stringbuffer << " ";
		}

		angle_x_2 = stof(x[0]);
		angle_y_2 = stof(x[1]);
		angle_z_2 = stof(x[2]);
		position_x_2 = stof(x[3]);
		position_y_2 = stof(x[4]);
		position_z_2 = stof(x[5]);
		is_moving_2 = stoi(x[6]);

		
		//position_x_2 = atof(RecvResult);
		//position_y_2 = atof(RecvResult);
		//position_z_2 = atof(RecvResult);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, RecvResult);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, x[0]);

		//memset(RecvResult, 0, sizeof(RecvResult));
	}

	memset(RecvResult, 0, sizeof(RecvResult));
	

	//////¹Þ±â
	////RecvResult = TCHAR_TO_ANSI(*TempRecvStr);
	//recv(sock, RecvResult, sizeof(RecvResult), 0);
	//TempRecvStr = FString(ANSI_TO_TCHAR(RecvResult));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempRecvStr);


}

void Aclient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	closesocket(sock);
	WSACleanup();
}