// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockBase.generated.h"

#define MAX_COMMANDBLOCK_COUNT 50


USTRUCT(BlueprintType)
struct FCommandBlockInfo {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		int index;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		FString data;
};



UCLASS()
class WITCH_OF_TIME_API ABlockBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlockBase();

	void ApplyHeatEnergy(float heat);

	UFUNCTION(BlueprintCallable)
	void ApplyMove();

	UFUNCTION(BlueprintCallable)
	void ApplyMoves(int Count);

	FVector GetOrigin();

	bool GetIsMovable();

	void GetMovement(int& maxcount, float& speed);

	bool GetApplyCommandBlocks();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<TEnumAsByte<EObjectTypeQuery>> Types;
	TArray<AActor*> Ignores;
	TArray<AActor*> Actors;

	FVector OriginLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCommandBlockInfo> CommandBlockArray;

	UPROPERTY(EditAnywhere)
	float temperature = 20.f;

	UPROPERTY(EditAnywhere)
	float FreezingPoint = 0.f;

	UPROPERTY(EditAnywhere)
	float MeltingPoint = 100.f;

	UPROPERTY(EditAnywhere)
	int hp = 100;

	UPROPERTY(EditAnywhere)
	float mess = 20.f;

	UPROPERTY(EditAnywhere)
	bool ApplyPhysics = false;
	UPROPERTY(EditAnywhere)
	bool ApplyTimeMagic = false;
	UPROPERTY(EditAnywhere)
	bool ApplyCommandBlocks = false;

	UPROPERTY(EditAnywhere)
	bool IsMovable = true;

	bool MoveChange = false;

	int Move_MaxCount = 0;
	float Move_Speed = 100;

	UPROPERTY(EditAnywhere)
	bool ApplyStateChange = false;

	UPROPERTY(EditAnywhere)
	bool ApplyHeatNearBlocks = false;

	UPROPERTY(EditAnywhere)
	UClass* m_FreezingClass = NULL;

	UPROPERTY(EditAnywhere)
	UClass* m_MeltingClass = NULL;

	float Elapsed_Time = 0.f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
