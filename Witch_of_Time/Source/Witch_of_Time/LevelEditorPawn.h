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

public:
	ALevelEditorPawn();

protected:
	void PlaceBlock();
	void DestroyBlock();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Placing")
	void SetPlaceActor(UClass* Actor);

	UFUNCTION(BlueprintCallable, Category = "Placing")
	UClass* GetPlaceActor();

	
};
