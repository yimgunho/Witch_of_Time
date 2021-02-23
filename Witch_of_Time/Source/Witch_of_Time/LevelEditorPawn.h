// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
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





public:
	ALevelEditorPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int cnt_for_blocktype;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass* TempPlaceActor;

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


protected:
	virtual void BeginPlay() override;

	void PlaceBlock();
	void DestroyBlock();

	void DrawDummyBlock(float value);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Placing")
	void SetPlaceActor(UClass* Actor);

	UFUNCTION(BlueprintCallable, Category = "Placing")
	UClass* GetPlaceActor();

	void SaveGame();
	void LoadGame();
	
public:
	virtual void Tick(float DeltaTime) override;
};
