// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveEditorLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Widget_SaveData.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_OF_TIME_API UWidget_SaveData : public UUserWidget
{
	GENERATED_BODY()

protected:

	void NativeConstruct() override;

	UTextBlock* SaveDate;

	UEditableTextBox* SaveName;

	USaveEditorLevel* LoadGameInstance;

public:
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void LoadGame();


};
