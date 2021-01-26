// Fill out your copyright notice in the Description page of Project Settings.

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
	int recv_cnt;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	int is_key_up;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//static DWORD WINAPI SendThread(void* lpData);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
