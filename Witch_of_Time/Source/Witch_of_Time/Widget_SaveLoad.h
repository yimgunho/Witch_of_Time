// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Widget_SaveLoad.generated.h"



UCLASS()
class WITCH_OF_TIME_API UWidget_SaveLoad : public UUserWidget
{
	GENERATED_BODY()
protected:
	UTextBlock* MainText;

	UScrollBox* SaveList;

	void NativeConstruct() override;
public:
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SetMode(bool Mode);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsSaveMode = true;

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		TArray<FString> GetAllSaveGameSlotNames();

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshSaveList();
};
