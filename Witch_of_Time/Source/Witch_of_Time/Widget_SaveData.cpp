// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SaveData.h"

void UWidget_SaveData::NativeConstruct()
{
	Super::NativeConstruct();

	SaveDate = Cast<UTextBlock>(GetWidgetFromName(TEXT("SaveDate")));

	SaveName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("Filename")));

	LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));
	if (LoadGameInstance)
	{
		LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot(SaveName->GetText().ToString(), LoadGameInstance->UserIndex));

		if (!IsValid(LoadGameInstance))
		{
			return;
		}
		SaveDate->SetText(FText::FromString(LoadGameInstance->Savetime.ToString()));
	}

}



void UWidget_SaveData::SaveGame()
{
	USaveEditorLevel* Instance = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));

	if (SaveName->GetText().ToString() != "New File")
	{
		TArray<AActor*> actors;

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Saveable", actors);

		for (auto target : actors)
		{
			Fblockinfo temp;
			auto casted = Cast<ABlockBase>(target);
			if (casted)
			{
				temp.location = casted->GetOrigin();
				temp.CommandArray = casted->CommandBlockArray;
			}
			else
				temp.location = target->GetActorLocation();

			temp.blockclass = target->GetClass();


			Instance->blockarray.Add(temp);
		}

		Instance->Savetime = FDateTime::Now();
		UGameplayStatics::SaveGameToSlot(Instance, SaveName->GetText().ToString(), Instance->UserIndex);
	}
	
}

void UWidget_SaveData::LoadGame()
{
	LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));
	if (LoadGameInstance)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Removable", actors);
		for (auto target : actors)
		{
			target->Destroy();
		}

		FRotator Rotator = { 0,0,0 };

		if (SaveName->GetText().ToString() != "New File") {
			LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot(SaveName->GetText().ToString(), LoadGameInstance->UserIndex));

			if (!IsValid(LoadGameInstance))
			{
				return;
			}
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			for (auto block : LoadGameInstance->blockarray)
			{
				auto spawned = GetWorld()->SpawnActor<AActor>(block.blockclass, block.location, Rotator, SpawnParams);

				auto casted = Cast<ABlockBase>(spawned);

				if (casted)
				{
					casted->CommandBlockArray = block.CommandArray;
				}
			}
		}

		


	}
}