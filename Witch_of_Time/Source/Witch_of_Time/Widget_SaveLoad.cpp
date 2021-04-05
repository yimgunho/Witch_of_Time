// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SaveLoad.h"
#include "BlockBase.h"
#include "Misc/DateTime.h"



void UWidget_SaveLoad::NativeConstruct()
{
	Super::NativeConstruct();

	MainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Title")));
	Slot0Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuickDate")));
	Slot1Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Slot1Date")));
	Slot2Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Slot2Date")));
	Slot3Text = Cast<UTextBlock>(GetWidgetFromName(TEXT("Slot3Date")));

	Slot0Button = Cast<UBorder>(GetWidgetFromName(TEXT("Slot0Button")));
	Slot1Button = Cast<UBorder>(GetWidgetFromName(TEXT("Slot1Button")));
	Slot2Button = Cast<UBorder>(GetWidgetFromName(TEXT("Slot2Button")));
	Slot3Button = Cast<UBorder>(GetWidgetFromName(TEXT("Slot3Button")));
}

void UWidget_SaveLoad::SetMode(bool Mode)
{
	IsSaveMode = Mode;
	if (Mode)
	{
		MainText->SetText(FText::FromString("Save"));
	}
	else
	{
		MainText->SetText(FText::FromString("Load"));
	}
	SetSaveInfo();
}

void UWidget_SaveLoad::SetSaveInfo()
{
	USaveEditorLevel* LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));
	if (LoadGameInstance)
	{
		LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
		if (LoadGameInstance)
		{
			Slot0Text->SetText(FText::AsDateTime(LoadGameInstance->Savetime,
				EDateTimeStyle::Medium,
				EDateTimeStyle::Medium,
				FText::GetInvariantTimeZone()
			));
		}
		else
		{
			Slot0Text->SetText(FText::FromString("No Save"));
		}
	}


	USaveEditorLevel* LoadGameInstance2 = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));

	if (LoadGameInstance2)
	{
		LoadGameInstance2 = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot("Slot1", LoadGameInstance2->UserIndex));
		if (LoadGameInstance2)
		{
			Slot1Text->SetText(FText::AsDateTime(LoadGameInstance2->Savetime,
				EDateTimeStyle::Medium,     
				EDateTimeStyle::Medium,
				FText::GetInvariantTimeZone()
			));
		}
		else
		{
			Slot1Text->SetText(FText::FromString("No Save"));
		}
	}
	

	
	USaveEditorLevel* LoadGameInstance3 = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));

	if (LoadGameInstance3)
	{
		LoadGameInstance3 = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot("Slot2", LoadGameInstance3->UserIndex));
		if (LoadGameInstance3)
		{
			Slot2Text->SetText(FText::AsDateTime(LoadGameInstance3->Savetime,
				EDateTimeStyle::Medium,
				EDateTimeStyle::Medium,
				FText::GetInvariantTimeZone()
			));
		}
		else
		{
			Slot2Text->SetText(FText::FromString("No Save"));
		}
	}
	
	USaveEditorLevel* LoadGameInstance4 = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));

	if (LoadGameInstance4)
	{
		LoadGameInstance4 = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot("Slot3", LoadGameInstance4->UserIndex));
		if (LoadGameInstance4)
		{
			Slot3Text->SetText(FText::AsDateTime(LoadGameInstance4->Savetime,
				EDateTimeStyle::Medium,
				EDateTimeStyle::Medium,
				FText::GetInvariantTimeZone()
			));
		}
		else
		{
			Slot3Text->SetText(FText::FromString("No Save"));
		}
	}
	
	
}

void UWidget_SaveLoad::SlotFunction(int saveslot)
{
	if (IsSaveMode)
	{
		SaveGame(saveslot);
	}
	else
	{
		LoadGame(saveslot);
	}


}

void UWidget_SaveLoad::SaveGame(int saveslot)
{
	USaveEditorLevel* Instance = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));

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

	switch (saveslot)
	{
	case 0:
		UGameplayStatics::SaveGameToSlot(Instance, Instance->SaveSlotName, Instance->UserIndex);
		break;
	case 1:
		UGameplayStatics::SaveGameToSlot(Instance, "Slot1", Instance->UserIndex);
		break;
	case 2:
		UGameplayStatics::SaveGameToSlot(Instance, "Slot2", Instance->UserIndex);
		break;
	case 3:
		UGameplayStatics::SaveGameToSlot(Instance, "Slot3", Instance->UserIndex);

		break;
	}
	SetSaveInfo();
}

void UWidget_SaveLoad::LoadGame(int saveslot)
{
	USaveEditorLevel* LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::CreateSaveGameObject(USaveEditorLevel::StaticClass()));
	if (LoadGameInstance)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Removable", actors);
		for (auto target : actors)
		{
			target->Destroy();
		}

		FRotator Rotator = { 0,0,0 };

		switch (saveslot)
		{
		case 0:
			LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
			break;
		case 1:
			LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot("Slot1", LoadGameInstance->UserIndex));
			break;
		case 2:
			LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot("Slot2", LoadGameInstance->UserIndex));
			break;
		case 3:
			LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot("Slot3", LoadGameInstance->UserIndex));
			break;
		}
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