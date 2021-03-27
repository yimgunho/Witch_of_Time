#pragma once

#include <string>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "client.generated.h"

UCLASS()
class WITCH_OF_TIME_API Aclient : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	//전역 변수로 선언하지 말고 여기 선언하고 변수이름 좀더 잘 알아볼수 있게 만들기
	Aclient();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempSendStr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempRecvStr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int cnt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float position_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float position_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float position_z;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float angle_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float angle_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float angle_z;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int recv_cnt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float position_x_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float position_y_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float position_z_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float angle_x_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float angle_y_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float angle_z_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int is_moving;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int is_moving_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_x_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_y_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_z_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* ToDestroyBlock_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int ToDestroyBlock_cnt = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString ToDestroyBlockName_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString ToDestroyBlockName_CL_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Block_cnt = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//static DWORD WINAPI SendThread(void* lpData);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

//헤더파일 TCPServer.h로 통일할것
typedef struct ChattingPacket
{
	int id = 1;
	char chatting[64] = "none";
}ChattingPacket;

typedef struct BlockPacket
{
	int id = 2;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;

}BlockPacket;

typedef struct DestroyPacket
{
	int id = 3;
	char todestroyblock[64] = "none";
}DestroyPacket;

typedef struct PlayerPacket
{
	int id = 4;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
}PlayerPacket;

typedef struct RecvPacket
{
	int id = 0;
	float angle_x = 0;
	float angle_y = 0;
	float angle_z = 0;
	float playerlocation_x = 0;
	float playerlocation_y = 0;
	float playerlocation_z = 0;
	float blocklocation_x = 0;
	float blocklocation_y = 0;
	float blocklocation_z = 0;
	char todestroyblock[64] = "none";
	char chatting[64] = "none";
}RecvPacket;