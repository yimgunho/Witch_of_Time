#pragma warning (default : 4668)
#include "client.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
<<<<<<< Updated upstream
#define BUFSIZE 128
=======
#define BUFSIZE 64
>>>>>>> Stashed changes

//DWORD WINAPI SendThread(LPVOID lpData);
//DWORD WINAPI RecvThread(LPVOID lpData);

SOCKET sock;

char* SendResult;
char RecvResult[BUFSIZE];
char ChatPacket[BUFSIZE] = {};
char PositionPacket[BUFSIZE] = {};
<<<<<<< Updated upstream
=======
char BlockPositionPacket[BUFSIZE] = {};
char ToDestroyBlockPacket[BUFSIZE] = {};

>>>>>>> Stashed changes

char Angle_x_char[BUFSIZE] = {};
char Angle_y_char[BUFSIZE] = {};
char Angle_z_char[BUFSIZE] = {};
char Position_x_char[BUFSIZE] = {};
char Position_y_char[BUFSIZE] = {};
char Position_z_char[BUFSIZE] = {};
char is_moving_char[BUFSIZE] = {};


char To_Destroy_Block_Name[BUFSIZE] = {};



const TCHAR* chars;
const TCHAR* tempchars;

float Elapsed_Time;
<<<<<<< Updated upstream
=======

int PositionCnt;

int BlockPositionCnt;

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}


	return (len - left);
}

>>>>>>> Stashed changes
// Sets default values
Aclient::Aclient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetChat();
	Elapsed_Time = 0;
	////º“ƒœ «ÿ¡¶
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

<<<<<<< Updated upstream
=======
	PositionCnt = 0;
	BlockPositionCnt = 0;

	ToDestroyBlockName_CL = "none";
	ToDestroyBlockName_CL_2 = "none";
	ToDestroyBlock_cnt = 0;

>>>>>>> Stashed changes
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

	// ¿¸º€ & ºˆΩ≈ ∑Á«™
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


//Packet packet;

// Called every frame
void Aclient::Tick(float DeltaTime)
{


	Super::Tick(DeltaTime);

	ChattingPacket chattingpacket;
	BlockPacket blockpacket;
	DestroyPacket destroypacket;
	PlayerPacket playerpacket;
	RecvPacket recvpacket;

	int len = 0;
	Elapsed_Time += DeltaTime;

	tempchars = *TempSendStr;

<<<<<<< Updated upstream
	char SendChatID[BUFSIZE] = "0";
	char SendPositionID[BUFSIZE] = " ";
	if (Elapsed_Time > 0.4f)
	{
		if (cnt == 0)
		{
			//∫∏≥ª±‚	
			SendResult = TCHAR_TO_ANSI(*TempSendStr);
			strcat(SendChatID, SendResult);
			strcpy_s(ChatPacket, SendChatID);
			send(sock, ChatPacket, strlen(ChatPacket), 0);
			//send(sock, SendResult, strlen(SendResult), 0);
			//memset(ChatPacket, 0, sizeof(ChatPacket));

			cnt++;
		}
		else
		{

			//¡¬«•∏¶ char∑Œ πŸ≤ﬁ
			sprintf(Angle_x_char, "%f", angle_x);
			sprintf(Angle_y_char, "%f", angle_y);
			sprintf(Angle_z_char, "%f", angle_z);
			sprintf(Position_x_char, "%f", position_x);
			sprintf(Position_y_char, "%f", position_y);
			sprintf(Position_z_char, "%f", position_z);
			sprintf(is_moving_char, "%d", is_moving);

			//æ’ø° ¡¬«•∏¶ ∫∏≥¬¥Ÿ¥¬ ID∏¶ ∫Ÿ»˚
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

			//¡¬«• ∆–≈∂ø° ∫πªÁ
			strcpy_s(PositionPacket, SendPositionID);
			//∫∏≥ø
			send(sock, PositionPacket, strlen(PositionPacket), 0);

			//memset(PositionPacket, 0, sizeof(PositionPacket));


		}
		Elapsed_Time = 0.f;
	}
	memset(ChatPacket, 0, sizeof(ChatPacket));
	memset(PositionPacket, 0, sizeof(PositionPacket));
=======
	char SendChatID[BUFSIZE] = "1";
	char SendPositionID[BUFSIZE] = "  ";
	char SendBlockPositionID[BUFSIZE] = "0";
	char SendToDestroyBlockID[BUFSIZE] = " ";


	//packet.actor = ToDestroyBlock_CL;

	//if (PositionCnt != 0 && cnt == 0)
	//{
	//	//∫∏≥ª±‚	
	//	SendResult = TCHAR_TO_ANSI(*TempSendStr);
	//	strcat(SendChatID, SendResult);
	//	strcpy_s(ChatPacket, SendChatID);
	//	send(sock, ChatPacket, strlen(ChatPacket), 0);
	//	//send(sock, SendResult, strlen(SendResult), 0);
	//	//memset(ChatPacket, 0, sizeof(ChatPacket));

	//	cnt++;
	//}

	if (PositionCnt != 0 && cnt == 0)
	{
		std::string chatting_string(TCHAR_TO_UTF8(*TempSendStr));

		//chattingpacket.chatting = chatting_string;
		//2
		send(sock, (char*)&chattingpacket.id, sizeof(chattingpacket.id), 0);
		send(sock, (char*)&chattingpacket.chatting, sizeof(chattingpacket.chatting), 0);

		cnt++;
	}



	//else if (ToDestroyBlock_cnt != 0)
	//{
	//	char* result = TCHAR_TO_ANSI(*ToDestroyBlockName_CL);
	//	strcat(SendToDestroyBlockID, result);
	//	strcpy_s(ToDestroyBlockPacket, SendToDestroyBlockID);
	//	send(sock, ToDestroyBlockPacket, strlen(ToDestroyBlockPacket), 0);
	//	ToDestroyBlockName_CL = "null";
	//	ToDestroyBlock_cnt = 0;
	//}

	//if (Elapsed_Time > 1.0f)
	//{
	else if (ToDestroyBlock_cnt != 0)
	{
		destroypacket.id = 3;
		std::string ToDestroyBlockName_CL_String(TCHAR_TO_UTF8(*ToDestroyBlockName_CL));
		strcpy_s(destroypacket.todestroyblock, ToDestroyBlockName_CL_String.c_str());

		len = sizeof(destroypacket);
		send(sock, (char*)&len, sizeof(int), 0);
		//send(sock, (const char*)&destroypacket, sizeof(DestroyPacket), 0);

		////2
		send(sock, (const char*)&destroypacket.id, sizeof(DestroyPacket), 0);
		send(sock, (const char*)&destroypacket.todestroyblock, sizeof(DestroyPacket), 0);


		std::string testdes_str(destroypacket.todestroyblock);
		FString testdes(testdes_str.c_str());
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, testdes);

		ToDestroyBlockName_CL = "none";
		ToDestroyBlock_cnt = 0;
	}

	//else if (Elapsed_Time > 0.1f)
	//	{
	//		sprintf(Block_Position_x_char, "%f", block_position_x);
	//		sprintf(Block_Position_y_char, "%f", block_position_y);
	//		sprintf(Block_Position_z_char, "%f", block_position_z);

	//		strcat(SendBlockPositionID, Block_Position_x_char);
	//		strcat(SendBlockPositionID, ",");
	//		strcat(SendBlockPositionID, Block_Position_y_char);
	//		strcat(SendBlockPositionID, ",");
	//		strcat(SendBlockPositionID, Block_Position_z_char);

	//		//¡¬«• ∆–≈∂ø° ∫πªÁ
	//		strcpy_s(BlockPositionPacket, SendBlockPositionID);

	//		send(sock, BlockPositionPacket, strlen(BlockPositionPacket), 0);

	//		Elapsed_Time = 0.f;
	//	}

	else if (Block_cnt != 0)
	{
		blockpacket.id = 2;
		blockpacket.blocklocation_x = block_position_x;
		blockpacket.blocklocation_y = block_position_y;
		blockpacket.blocklocation_z = block_position_z;

		//len = sizeof(blockpacket);
		//send(sock, (char*)&len, sizeof(int), 0);
		//send(sock, (const char*)&blockpacket, sizeof(BlockPacket), 0);

		//4
		send(sock, (const char*)&blockpacket.id, sizeof(BlockPacket), 0);
		send(sock, (const char*)&blockpacket.blocklocation_x, sizeof(BlockPacket), 0);
		send(sock, (const char*)&blockpacket.blocklocation_y, sizeof(BlockPacket), 0);
		send(sock, (const char*)&blockpacket.blocklocation_z, sizeof(BlockPacket), 0);
		Block_cnt = 0;


		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(blockpacket.blocklocation_x));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(blockpacket.blocklocation_x) + ", " + FString::SanitizeFloat(blockpacket.blocklocation_y) + ", " + FString::SanitizeFloat(blockpacket.blocklocation_z) + ", " + "Block Sent");
	}

	//else if (Elapsed_Time > 0.1f && (PositionCnt == 0 || is_moving == 1))
//{

//	//¡¬«•∏¶ char∑Œ πŸ≤ﬁ
//	sprintf(Angle_x_char, "%f", angle_x);
//	sprintf(Angle_y_char, "%f", angle_y);
//	sprintf(Angle_z_char, "%f", angle_z);
//	sprintf(Position_x_char, "%f", position_x);
//	sprintf(Position_y_char, "%f", position_y);
//	sprintf(Position_z_char, "%f", position_z);
//	sprintf(is_moving_char, "%d", is_moving);

//	//æ’ø° ¡¬«•∏¶ ∫∏≥¬¥Ÿ¥¬ ID∏¶ ∫Ÿ»˚
//	strcat(SendPositionID, Angle_x_char);
//	strcat(SendPositionID, ",");
//	strcat(SendPositionID, Angle_y_char);
//	strcat(SendPositionID, ",");
//	strcat(SendPositionID, Angle_z_char);
//	strcat(SendPositionID, ",");
//	strcat(SendPositionID, Position_x_char);
//	strcat(SendPositionID, ",");
//	strcat(SendPositionID, Position_y_char);
//	strcat(SendPositionID, ",");
//	strcat(SendPositionID, Position_z_char);
//	strcat(SendPositionID, ",");
//	strcat(SendPositionID, is_moving_char);

//	//¡¬«• ∆–≈∂ø° ∫πªÁ
//	strcpy_s(PositionPacket, SendPositionID);
//	//∫∏≥ø
//	send(sock, PositionPacket, strlen(PositionPacket), 0);

//	//memset(PositionPacket, 0, sizeof(PositionPacket));


//	Elapsed_Time = 0.f;
//	if (PositionCnt == 0)
//	{
//		PositionCnt++;
//	}

//}

	//else
	//else if ((PositionCnt == 0 || is_moving == 1))
	//{
	//	playerpacket.angle_x = angle_x;
	//	playerpacket.angle_y = angle_y;
	//	playerpacket.angle_z = angle_z;
	//	playerpacket.playerlocation_x = position_x;
	//	playerpacket.playerlocation_y = position_y;
	//	playerpacket.playerlocation_z = position_z;

	//	//7
	//	send(sock, (char*)&playerpacket.id, sizeof(playerpacket.id), 0);
	//	send(sock, (char*)&playerpacket.angle_x, sizeof(playerpacket.angle_x), 0);
	//	send(sock, (char*)&playerpacket.angle_y, sizeof(playerpacket.angle_y), 0);
	//	send(sock, (char*)&playerpacket.angle_z, sizeof(playerpacket.angle_z), 0);
	//	send(sock, (char*)&playerpacket.playerlocation_x, sizeof(playerpacket.playerlocation_x), 0);
	//	send(sock, (char*)&playerpacket.playerlocation_y, sizeof(playerpacket.playerlocation_y), 0);
	//	send(sock, (char*)&playerpacket.playerlocation_z, sizeof(playerpacket.playerlocation_z), 0);

	//	if (PositionCnt == 0)
	//	{
	//		PositionCnt++;
	//	}
	//}

	//Elapsed_Time = 0.f;
//}
//memset(ChatPacket, 0, sizeof(ChatPacket));
//memset(PositionPacket, 0, sizeof(PositionPacket));
//memset(BlockPositionPacket, 0, sizeof(BlockPositionPacket));


//////////////////////////////////////////////////////////////////////////////
//recv(sock, RecvResult, sizeof(RecvResult), 0);
	recv(sock, (char*)&recvpacket.id, sizeof(recvpacket.id), 0);
	//recv(sock, (char*)&recvpacket, sizeof(recvpacket), 0);
	FString IntAsStr = FString::FromInt(recvpacket.id);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, IntAsStr);

	//if(RecvResult[0] == '1')
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, RecvResult);

	//	RecvResult[0] = ' ';
	//	TempRecvStr = FString(ANSI_TO_TCHAR(RecvResult));
	//	if (TempRecvStr != "1" || TempRecvStr != "1 ")
	//	{
	//		recv_cnt = 0;
	//	}
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempRecvStr);
>>>>>>> Stashed changes

	//	//memset(RecvResult, 0, sizeof(RecvResult));
	//	//Elapsed_Time = 0.f;
	//}

<<<<<<< Updated upstream
	if(RecvResult[0] == '0')
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, RecvResult);

		RecvResult[0] = ' ';
		TempRecvStr = FString(ANSI_TO_TCHAR(RecvResult));
		if (TempRecvStr != "0" || TempRecvStr != "0 ")
		{
			recv_cnt = 0;
		}
=======
	if (recvpacket.id == 1)
	{
		recv(sock, (char*)&recvpacket.chatting, BUFSIZE, 0);

		std::string Chatting_String(recvpacket.todestroyblock);
		FString Chatting_FString(Chatting_String.c_str());
		TempRecvStr = Chatting_FString;
		recv_cnt = 0;

>>>>>>> Stashed changes
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TempRecvStr);

		//memset(RecvResult, 0, sizeof(RecvResult));
		//Elapsed_Time = 0.f;
	}

<<<<<<< Updated upstream
	else if (RecvResult[0] == ' ')
	{
=======
	//else if (RecvResult[0] == '0')
	//{
	//	string a(RecvResult);
	//	istringstream ss(a);
	//	string stringbuffer;
	//	vector<string> x;
	//	x.clear();

	//	while (getline(ss, stringbuffer, ','))
	//	{
	//		x.push_back(stringbuffer);
	//		cout << stringbuffer << " ";
	//	}

	//	block_position_x_2 = stof(x[0]);
	//	block_position_y_2 = stof(x[1]);
	//	block_position_z_2 = stof(x[2]);

	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, RecvResult);
	//}

	else if (recvpacket.id == 2)
	{
		//recv(sock, (char*)&recvpacket.blocklocation_x, sizeof(recvpacket.blocklocation_x), 0);
		//recv(sock, (char*)&recvpacket.blocklocation_y, sizeof(recvpacket.blocklocation_y), 0);
		//recv(sock, (char*)&recvpacket.blocklocation_z, sizeof(recvpacket.blocklocation_z), 0);

		recv(sock, (char*)&recvpacket.blocklocation_x, sizeof(recvpacket.blocklocation_x), 0);
		recv(sock, (char*)&recvpacket.blocklocation_y, sizeof(recvpacket.blocklocation_y), 0);
		recv(sock, (char*)&recvpacket.blocklocation_z, sizeof(recvpacket.blocklocation_z), 0);

		block_position_x_2 = recvpacket.blocklocation_x;
		block_position_y_2 = recvpacket.blocklocation_y;
		block_position_z_2 = recvpacket.blocklocation_z;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(recvpacket.blocklocation_x) + FString::SanitizeFloat(recvpacket.blocklocation_y) + FString::SanitizeFloat(recvpacket.blocklocation_z) + "Block recv");

	}

>>>>>>> Stashed changes

	//else if (RecvResult[0] == ' '/* && RecvResult[1] == ' '*/)
	//{

	//	string a(RecvResult);
	//	istringstream ss(a);
	//	string stringbuffer;
	//	vector<string> x;
	//	x.clear();

	//	while (getline(ss, stringbuffer, ','))
	//	{
	//		x.push_back(stringbuffer);
	//		cout << stringbuffer << " ";
	//	}

	//	FString Temp_FStirng(x[0].c_str());
	//	ToDestroyBlockName_CL_2 = Temp_FStirng;

	//	FString HappyString(x[0].c_str());
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HappyString);


	//}

	else if (recvpacket.id == 3)
	{
		recv(sock, (char*)&recvpacket.todestroyblock, sizeof(recvpacket.todestroyblock), 0);

		std::string todestroyblock_String(recvpacket.todestroyblock);
		FString todestroyblock_FString(todestroyblock_String.c_str());
		ToDestroyBlockName_CL_2 = todestroyblock_FString;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Destroy recv");
	}

<<<<<<< Updated upstream
	memset(RecvResult, 0, sizeof(RecvResult));
	
=======


	//else if (RecvResult[0] == ' ' && RecvResult[1] != ' ')
	//{

	//	string a(RecvResult);
	//	istringstream ss(a);
	//	string stringbuffer;
	//	vector<string> x;
	//	x.clear();
	//		
	//	while (getline(ss, stringbuffer, ','))
	//	{
	//		x.push_back(stringbuffer);
	//		cout << stringbuffer << " ";
	//	}

	//	angle_x_2 = stof(x[0]);
	//	angle_y_2 = stof(x[1]);
	//	angle_z_2 = stof(x[2]);
	//	position_x_2 = stof(x[3]);
	//	position_y_2 = stof(x[4]);
	//	position_z_2 = stof(x[5]);
	//	is_moving_2 = stoi(x[6]);

	//	
	//	//position_x_2 = atof(RecvResult);
	//	//position_y_2 = atof(RecvResult);
	//	//position_z_2 = atof(RecvResult);

	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, RecvResult);
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, x[0]);

	//	//memset(RecvResult, 0, sizeof(RecvResult));
	//}

	else if (recvpacket.id == 4)
	{
		recv(sock, (char*)&recvpacket.angle_x, sizeof(recvpacket.angle_x), 0);
		recv(sock, (char*)&recvpacket.angle_y, sizeof(recvpacket.angle_y), 0);
		recv(sock, (char*)&recvpacket.angle_z, sizeof(recvpacket.angle_z), 0);
		recv(sock, (char*)&recvpacket.playerlocation_x, sizeof(recvpacket.playerlocation_x), 0);
		recv(sock, (char*)&recvpacket.playerlocation_y, sizeof(recvpacket.playerlocation_y), 0);
		recv(sock, (char*)&recvpacket.playerlocation_z, sizeof(recvpacket.playerlocation_z), 0);

		angle_x_2 = recvpacket.angle_x;
		angle_y_2 = recvpacket.angle_y;
		angle_z_2 = recvpacket.angle_z;
		position_x_2 = recvpacket.playerlocation_x;
		position_y_2 = recvpacket.playerlocation_y;
		position_z_2 = recvpacket.playerlocation_z;
		//is_moving_2 = ;
	}

>>>>>>> Stashed changes

	//////πﬁ±‚
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