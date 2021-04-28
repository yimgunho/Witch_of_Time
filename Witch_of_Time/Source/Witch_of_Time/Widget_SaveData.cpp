// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SaveData.h"
#include "../../../Server/Server/TCPServer.h"
#include "client.h"


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
		blockid_SaveData = 0;
		location_x_SaveData = 0;
		location_y_SaveData = 0;
		location_z_SaveData = 0;
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
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "LoadPlace_By_SaveData");
				auto spawned = GetWorld()->SpawnActor<AActor>(block.blockclass, block.location, Rotator, SpawnParams);

				//blockpacket.blockindex = block_index;
				//blockpacket.block_id = block_id_CL;
				//blockpacket.blocklocation_x = block_position_x;
				//blockpacket.blocklocation_y = block_position_y;
				//blockpacket.blocklocation_z = block_position_z;

				blockid_SaveData++;

				location_x_SaveData = block.location.X;
				location_y_SaveData = block.location.Y;
				location_z_SaveData = block.location.Z;

				id_arr.Add(blockid_SaveData);
				location_x_arr.Add(block.location.X);
				location_y_arr.Add(block.location.Y);
				location_z_arr.Add(block.location.Z);


				auto casted = Cast<ABlockBase>(spawned);

				if (casted)
				{
					casted->CommandBlockArray = block.CommandArray;
				}
			}
		}

		


	}
}

int UWidget_SaveData::Transblockid()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Transblockid");
	return blockid_SaveData;

}

float UWidget_SaveData::Transblocklocation_x()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Transblocklocation_x");
	return location_x_SaveData;
}

float UWidget_SaveData::Transblocklocation_y()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Transblocklocation_y");
	return location_y_SaveData;
}

float UWidget_SaveData::Transblocklocation_z()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Transblocklocation_z");
	return location_z_SaveData;
}
