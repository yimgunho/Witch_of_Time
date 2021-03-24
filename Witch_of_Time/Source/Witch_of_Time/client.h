#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "client.generated.h"

UCLASS()
class WITCH_OF_TIME_API Aclient : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	Aclient();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		FString TempSendStr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		FString TempRecvStr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		int cnt;
	UFUNCTION(BlueprintImplementableEvent, Category = Default)
		void SetChat();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float position_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float position_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float position_z;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float angle_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float angle_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float angle_z;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float block_position_x;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float block_position_y;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		float block_position_z;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		int recv_cnt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
		int is_key_up;

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
		int is_set;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int is_set_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_x_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_y_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float block_position_z_2;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* ToDestroyBlock_CL;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//static DWORD WINAPI SendThread(void* lpData);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};