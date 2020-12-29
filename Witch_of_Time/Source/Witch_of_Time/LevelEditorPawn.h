// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
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

	
};
