// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveEditorLevel.h"
#include "client.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		Aclient* client;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int blockid_SaveData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float location_x_SaveData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float location_y_SaveData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float location_z_SaveData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int blockindex_SaveData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int> id_arr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_x_arr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_y_arr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<float> location_z_arr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int> blockindex_arr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int> TempCommandBlockid;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FCommandBlockInfo> TempCommandBlocklist;
	
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
		void LoadGame();

	UFUNCTION(BlueprintCallable)
		int Transblockid();

	UFUNCTION(BlueprintCallable)
		float Transblocklocation_x();

	UFUNCTION(BlueprintCallable)
		float Transblocklocation_y();

	UFUNCTION(BlueprintCallable)
		float Transblocklocation_z();

	UFUNCTION(BlueprintCallable)
		int Transblockindex();


};
