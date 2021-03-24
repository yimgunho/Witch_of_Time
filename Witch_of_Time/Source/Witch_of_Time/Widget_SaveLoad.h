// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "SaveEditorLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Widget_SaveLoad.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_OF_TIME_API UWidget_SaveLoad : public UUserWidget
{
	GENERATED_BODY()
protected:
	UTextBlock* MainText;

	UTextBlock* Slot0Text;
	UTextBlock* Slot1Text;
	UTextBlock* Slot2Text;
	UTextBlock* Slot3Text;

	UBorder* Slot0Button;
	UBorder* Slot1Button;
	UBorder* Slot2Button;
	UBorder* Slot3Button;

	void NativeConstruct() override;
public:
	void SetMode(bool Mode);
	bool IsSaveMode = true;
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void SetSaveInfo();

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void SlotFunction(int saveslot);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void SaveGame(int saveslot);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void LoadGame(int saveslot);
};
