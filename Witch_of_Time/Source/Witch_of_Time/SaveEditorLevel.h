// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveEditorLevel.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct Fblockinfo {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FVector location;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	UClass* blockclass;
};

UCLASS()
class WITCH_OF_TIME_API USaveEditorLevel : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<Fblockinfo> blockarray;

	USaveEditorLevel();
};
