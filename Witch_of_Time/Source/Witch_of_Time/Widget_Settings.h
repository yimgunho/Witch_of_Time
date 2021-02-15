// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Widget_SaveLoad.h"
#include "Widget_Settings.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_OF_TIME_API UWidget_Settings : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UWidget_SaveLoad* SaveLoadWidget;

	UButton* SaveButton;

	UButton* LoadButton;

	void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetWidget(UWidget_SaveLoad* Widget);

	UFUNCTION(BlueprintCallable)
	UWidget_SaveLoad* GetWidget();

	UFUNCTION()
	void OnSaveButtonClicked();

	UFUNCTION()
	void OnLoadButtonClicked();

	
};
