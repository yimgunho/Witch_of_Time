// Fill out your copyright notice in the Description page of Project Settings.

#pragma warning (default : 4668)
#include "client.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>


#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

//DWORD WINAPI SendThread(LPVOID lpData);
//DWORD WINAPI RecvThread(LPVOID lpData);

SOCKET sock;

char* SendResult;
char RecvResult[1024];

const TCHAR* chars;
const TCHAR* tempchars;

float Elapsed_Time;
// Sets default values
Aclient::Aclient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetChat();
	Elapsed_Time = 0;
	////소켓 해제
	//closesocket(sock);
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

	// 전송 & 수신 루푸
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


	if (TempSendStr != "DEL")
	{
		//보내기	
		SendResult = TCHAR_TO_ANSI(*TempSendStr);
		send(sock, SendResult, strlen(SendResult), 0);
	}

	if (Elapsed_Time > 0.1f)
	{

		recv(sock, RecvResult, sizeof(RecvResult), 0);
		TempRecvStr = FString(ANSI_TO_TCHAR(RecvResult));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempRecvStr);

		memset(RecvResult, 0, sizeof(RecvResult));
		Elapsed_Time = 0.f;
	}
	//////받기
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