#pragma once

#include "../../../Server/Server/TCPServer.h"
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
