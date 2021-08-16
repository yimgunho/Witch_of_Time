// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SaveData.h"
#include "../../../Server/Server/TCPServer.h"
#include "client.h"
#include "LevelEditorPawn.h"


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
				temp.index_of_block = casted->blockindex;
				FString blockindex_FString = FString::FromInt(temp.index_of_block);
			}
			else
			{
				temp.location = target->GetActorLocation();
			}
			temp.blockclass = target->GetClass();


			Instance->blockarray.Add(temp);
		}

		Instance->Savetime = FDateTime::Now();
		UGameplayStatics::SaveGameToSlot(Instance, SaveName->GetText().ToString(), Instance->UserIndex);
	}

}

void UWidget_SaveData::LoadGame()
{
	int Generated_block_id = 11;
	LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));
	if (LoadGameInstance)
	{
		location_x_SaveData = 0;
		location_y_SaveData = 0;
		location_z_SaveData = 0;
		blockindex_SaveData = 0;

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
				//auto spawned = GetWorld()->SpawnActor<AActor>(block.blockclass, block.location, Rotator, SpawnParams);
				//client->send_block_packet(block.index_of_block, block.location.X, block.location.Y, block.location.Z);
				TArray<int32> block_index;
				TArray<float> block_data_0;
				TArray<float> block_data_1;
				TArray<float> block_data_2;
				TArray<float> block_data_3;

				for (int i = 0; i < block.CommandArray.Num(); ++i)
				{
					block_index.Emplace(block.CommandArray[i].index);
					
					if(block.CommandArray[i].data.IsValidIndex(0) == true)
						block_data_0.Insert(block.CommandArray[i].data[0], i);
					else
						block_data_0.Insert(-1, i);
					if (block.CommandArray[i].data.IsValidIndex(1) == true)
						block_data_1.Insert(block.CommandArray[i].data[1], i);
					else
						block_data_1.Insert(-1, i);
					if (block.CommandArray[i].data.IsValidIndex(2) == true)
						block_data_2.Insert(block.CommandArray[i].data[2], i);
					else
						block_data_2.Insert(-1, i);
					if (block.CommandArray[i].data.IsValidIndex(3) == true)
						block_data_3.Insert(block.CommandArray[i].data[3], i);
					else
						block_data_3.Insert(-1, i);
				}

				client->send_block_with_command_packet(block.index_of_block, block.location.X, block.location.Y, block.location.Z, block_index,
					block_data_0, block_data_1, block_data_2, block_data_3);

			}
		}

		


	}
}

int UWidget_SaveData::Transblockid()
{
	return blockid_SaveData;

}

float UWidget_SaveData::Transblocklocation_x()
{
	return location_x_SaveData;
}

float UWidget_SaveData::Transblocklocation_y()
{
	return location_y_SaveData;
}

float UWidget_SaveData::Transblocklocation_z()
{
	return location_z_SaveData;
}

int UWidget_SaveData::Transblockindex()
{
	return blockindex_SaveData;
}
