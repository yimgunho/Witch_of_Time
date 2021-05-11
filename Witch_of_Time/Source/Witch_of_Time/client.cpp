#pragma warning (default : 4668)
#include "client.h"
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000


void recv_all(SOCKET sock, char* buf, size_t len, int flag)
{
	int total_received = 0;
	while (total_received != len) {
		int ret = recv(sock, buf + total_received, len - total_received, flag);
		total_received += ret;
	}

}

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

SOCKET sock;

const TCHAR* chars;
const TCHAR* tempchars;

float Elapsed_Time;

int PositionCnt;

int BlockPositionCnt;

//int recvn(SOCKET s, char* buf, int len, int flags)
//{
//	int received;
//	char* ptr = buf;
//	int left = len;
//
//	while (left > 0)
//	{
//		received = recv(s, ptr, left, flags);
//		if (received == SOCKET_ERROR)
//			return SOCKET_ERROR;
//		else if (received == 0)
//			break;
//		left -= received;
//		ptr += received;
//	}
//
//
//	return (len - left);
//}



//int main() 
//{
//	int retval;
//
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "MAINTEST");
//
//	// 파일 열기
//	FILE* fp = fopen("card10.png", "rb");
//
//	if (fp == NULL) {
//		perror("fopen()");
//		return 1;
//	}
//
//	// 파일 이름 보내기(고정 길이+가변 길이)
//	char filename[1024] = "card10.png";
//	int length = strlen(filename) + 1;
//
//	retval = send(sock, (char*)&length, sizeof(int), 0); // 고정 길이
//	if (retval == SOCKET_ERROR) err_quit("send()");
//
//	retval = send(sock, filename, length, 0);             // 가변 길이
//	if (retval == SOCKET_ERROR) err_quit("send()");
//
//	// 파일 크기와 데이터 보내기(고정 길이+가변 길이)
//
//	//크기: 고정 길이
//	fseek(fp, 0, SEEK_END);
//	length = ftell(fp);
//	retval = send(sock, (char*)&length, sizeof(int), 0);
//	if (retval == SOCKET_ERROR) err_quit("send()");
//	rewind(fp);
//
//	//데이터: 가변 길이
//	char* buf = (char*)malloc(length);
//	if (buf == NULL) {
//		fprintf(stderr, "malloc() error!\n");
//		exit(1);
//	}
//
//	int nbytes = fread(buf, 1, length, fp);
//	if (nbytes == length) {
//		retval = send(sock, buf, nbytes, 0);
//		if (retval == SOCKET_ERROR) err_quit("send()");
//	}
//	else {
//		perror("fread()");
//	}
//	fclose(fp);
//	return 0;
//}


Aclient::Aclient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Elapsed_Time = 0;
	cnt = 0;
	recv_cnt = 0;
}

void Aclient::SetBlockIndex(int index)
{
	block_index = index;
}

// Called when the game starts or when spawned
void Aclient::BeginPlay()
{
	Super::BeginPlay();

	TempSendStr = TEXT("");
	TempRecvStr = TEXT("");
	chars = *TempSendStr;

	PositionCnt = 0;
	BlockPositionCnt = 0;

	ToDestroyBlockName_CL = "none";
	ToDestroyBlockName_CL_2 = "none";
	ToDestroyBlock_cnt = 0;

	todestroyblockid = -1;
	todestroyblockid_2 = -1;
	is_changed_mode = false;

	WSADATA wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);
	
	int optval = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval));

	SOCKADDR_IN serveraddr;
	//ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	//main();

	//int sendBytes;
	//int file_size;
	//char buf[BUFSIZE];
	//FILE* fp = fopen("card10.png", "rb"); //파일열기 
	//fseek(fp, 0, SEEK_END); //끝으로가서 
	//file_size = ftell(fp); //사이즈재고 
	//fseek(fp, 0, SEEK_SET); //처음으로와서 
	//snprintf(buf, sizeof(buf), "%d", file_size); //사이즈값을 buf에다가넣기 
	//send(sock, buf, sizeof(buf), 0); //사이즈값전송 
	//while ((sendBytes = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) send(sock, buf, sendBytes, 0);
	//파일이끝날때까지 읽고 보내고를 반복 
	//fclose(fp);

}



// Called every frame
void Aclient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	char buffer[BUFSIZE];
	ChattingPacket chattingpacket;
	BlockPacket blockpacket;
	DestroyPacket destroypacket;
	PlayerPacket playerpacket;
	RecvPacket recvpacket;

	int len = 0;
	Elapsed_Time += DeltaTime;

	tempchars = *TempSendStr;


	if (/*PositionCnt != 0 && cnt == 0*/TempSendStr != "")
	{
		//std::string Chatting_String(chattingpacket.chatting);
		//FString Chatting_FString(Chatting_String.c_str());
		//TempRecvStr = Chatting_FString;

		//chattingpacket.chatting = Chatting_String;

		std::string TempSendString(TCHAR_TO_UTF8(*TempSendStr));
		//chattingpacket.chatting = TempSendString;

		strcpy_s(chattingpacket.chatting, sizeof(chattingpacket.chatting), TempSendString.c_str());
		//FString HappyString(chattingpacket.chatting.c_str());

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HappyString);
		//recv_cnt = 0;

		//2
		send(sock, (char*)&chattingpacket, sizeof(chattingpacket), 0);

		TempSendStr = "";
		//cnt++;
	}

	//if (Elapsed_Time > 1.0f)
	//{
	else if (ToDestroyBlock_cnt != 0)
	{
		//std::string ToDestroyBlockName_CL_String(TCHAR_TO_UTF8(*ToDestroyBlockName_CL));
		//strcpy_s(destroypacket.todestroyblock, ToDestroyBlockName_CL_String.c_str());
		destroypacket.block_id = todestroyblockid;
		
		////2
		send(sock, (char*)&destroypacket, sizeof(destroypacket), 0);


		//std::string testdes_str(destroypacket.todestroyblock);
		//FString testdes(testdes_str.c_str());
		FString todestroyblockid_FString = FString::FromInt(todestroyblockid);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Send " + todestroyblockid_FString);

		//todestroyblockid = 0;
		ToDestroyBlockName_CL = "none";
		ToDestroyBlock_cnt = 0;
	}

	else if (Block_cnt != 0)
	{
		blockpacket.blockindex = block_index;
		blockpacket.block_id = block_id_CL;
		blockpacket.blocklocation_x = block_position_x;
		blockpacket.blocklocation_y = block_position_y;
		blockpacket.blocklocation_z = block_position_z;

		//4
		send(sock, (char*)&blockpacket, sizeof(blockpacket), 0);
		Block_cnt = 0;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::SanitizeFloat(blockpacket.blocklocation_x) + ", " + FString::SanitizeFloat(blockpacket.blocklocation_y) + ", " + FString::SanitizeFloat(blockpacket.blocklocation_z) + ", " + "Block Sent");
	}

	else if (is_moving != 0 || is_changed_mode == true)
	{
		playerpacket.angle_x = angle_x;
		playerpacket.angle_y = angle_y;
		playerpacket.angle_z = angle_z;
		playerpacket.playerlocation_x = position_x;
		playerpacket.playerlocation_y = position_y;
		playerpacket.playerlocation_z = position_z;

		//4
		send(sock, (char*)&playerpacket, sizeof(playerpacket), 0);

		if (is_changed_mode == true)
		{
			is_changed_mode = false;
		}
	}


/////////////////////////////////////////////////////////////////////////////////////////////
	//FString IntAsStr = FString::FromInt(recvpacket.id);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, IntAsStr);

	recv(sock, &buffer[0], sizeof(char) + sizeof(int), 0);

	switch (buffer[0])
	{
	case CHATTING:
	{
		recv_all(sock, buffer + 5, sizeof(ChattingPacket) - 5, 0);
		auto cast = reinterpret_cast<ChattingPacket*>(buffer);
		
		std::string test(cast->chatting);
		TempRecvStr = (test.c_str());
		/*FString recv_chatting_FString(test.c_str());*/


		//Chatting_FString = recv_chatting_FString;

		//std::string Chatting_String(cast->chatting);
		//FString Chatting_FString(Chatting_String.c_str());
		//TempRecvStr = Chatting_FString;
		//recv_cnt = 0;
	}
	break;
	case BLOCK:
	{
		recv_all(sock, buffer + 5, sizeof(BlockPacket) - 5, 0);
		auto cast = reinterpret_cast<BlockPacket*>(buffer);
		block_index = cast->blockindex;
		block_id_CL_2 = cast->block_id;
		block_position_x_2 = cast->blocklocation_x;
		block_position_y_2 = cast->blocklocation_y;
		block_position_z_2 = cast->blocklocation_z;
	}
	break;
	case DESTROY:
	{
		recv_all(sock, buffer + 5, sizeof(DestroyPacket) - 5, 0);
		auto cast = reinterpret_cast<DestroyPacket*>(buffer);
		//std::string todestroyblock_String(cast->todestroyblock);
		int todestroyblockid_recv(cast->block_id);
		todestroyblockid_2 = todestroyblockid_recv;

		FString todestroyblockid_2_FString = FString::FromInt(todestroyblockid_2);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Recv " + todestroyblockid_2_FString);
		//FString todestroyblock_FString(todestroyblock_String.c_str());
		//ToDestroyBlockName_CL_2 = todestroyblock_FString;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Destroy recv");
	}
	break;
	case PLAYER:
	{
		recv_all(sock, buffer + 5, sizeof(PlayerPacket) - 5, 0);
		auto cast = reinterpret_cast<PlayerPacket*>(buffer);
		angle_x_2 = cast->angle_x;
		angle_y_2 = cast->angle_y;
		angle_z_2 = cast->angle_z;
		position_x_2 = cast->playerlocation_x;
		position_y_2 = cast->playerlocation_y;
		position_z_2 = cast->playerlocation_z;
	}
	break;
	default:
		break;
	}


}

void Aclient::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	closesocket(sock);
	WSACleanup();
}



