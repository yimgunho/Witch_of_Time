// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "BlockBase.h"
#include "SaveEditorLevel.h"
#include "Kismet/GameplayStatics.h"
#include "LevelEditorPawn.generated.h"



/**
 * 
 */
UCLASS()
class WITCH_OF_TIME_API ALevelEditorPawn : public ADefaultPawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UClass* PlaceActor;

	UPROPERTY(EditAnywhere)
	UClass* DummyActor;

	AActor* DummyBlock;


	DECLARE_DELEGATE_OneParam(OneParamBool, bool);

	FTimerHandle LeftButtonTimer;

	FTimerHandle RightButtonTimer;

public:
	ALevelEditorPawn();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int blockindex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int blockid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* TempPlaceActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString type_of_block;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float location_x;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float location_y;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float location_z;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float temp_location_x;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float temp_location_y;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float temp_location_z;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* ToDestroyBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPlaceTime = 0.33f;

	bool LeftMousePressed = false;
	bool RightMousePressed = false;

	bool CommandBlockMode = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool VisibleFlag = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString BlockName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString ToDestroyBlockName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* ClassOfPlacedBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int todestroyblockid = 0;

	int dummycnt = 0;


protected:
	virtual void BeginPlay() override;

	void LeftMouseFunc(bool flag);
	void RightMouseFunc(bool flag);

	void SwitchPlaceMode();

	void PlaceBlock();
	void DestroyBlock();

	void DrawDummyBlock(float value);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Placing")
	void SetVisibleFlag(bool flag);

	UFUNCTION(BlueprintCallable, Category = "Placing")
	void SetPlaceActor(UClass* Actor);

	UFUNCTION(BlueprintCallable, Category = "Placing")
	UClass* GetPlaceActor();

	UFUNCTION(BlueprintCallable, Category = "Placing")
	AActor* GetDummyBlock();

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void LoadGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "CommandBlock")
	void OpenCommandList(ABlockBase* target);
	
	UFUNCTION(BlueprintCallable)
	int GetBlockIndex();

	UFUNCTION(BlueprintCallable)
	void SetClassOfPlacedBlock(UClass* blockclass);
	
	UFUNCTION(BlueprintCallable)
	AActor* SpawnDummyActor(UClass* blockclass);

public:
	virtual void Tick(float DeltaTime) override;
};
