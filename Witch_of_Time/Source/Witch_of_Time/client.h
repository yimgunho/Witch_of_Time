#pragma once

#include "../../../Server/Server/TCPServer.h"
#include <string>
#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "client.generated.h"
#define MAX_SOCKET 10 

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
		float block_position_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_z;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_x_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_y_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_z_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockindex_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_3;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int block_index;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int todestroyblockid;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int todestroyblockid_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int block_id_CL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int block_id_CL_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString Chatting_FString;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool is_changed_mode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int playerindex_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> angle_x_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> angle_y_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> angle_z_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> position_x_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> position_y_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> position_z_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempCommandBlockName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int lengthofcommandlist;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockindex_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_0_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_1_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_2_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> commandblockdata_3_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString TempCommandBlockName_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TempCommandBlockId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TempCommandBlockId_recv;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool is_ready;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool ready_switch;

	UFUNCTION(BlueprintCallable)
		void SetBlockIndex(int index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//static DWORD WINAPI SendThread(void* lpData);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
