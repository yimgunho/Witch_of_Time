// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Settings.h"

void UWidget_Settings::NativeConstruct()
{
	Super::NativeConstruct();

	SaveButton = Cast<UButton>(GetWidgetFromName(TEXT("Save")));

	LoadButton = Cast<UButton>(GetWidgetFromName(TEXT("Load")));

	SaveButton->OnClicked.AddDynamic(this, &UWidget_Settings::OnSaveButtonClicked);
	LoadButton->OnClicked.AddDynamic(this, &UWidget_Settings::OnLoadButtonClicked);
}

void UWidget_Settings::SetWidget(UWidget_SaveLoad* Widget)
{
	SaveLoadWidget = Widget;
}

UWidget_SaveLoad* UWidget_Settings::GetWidget()
{
	return SaveLoadWidget;
}

void UWidget_Settings::OnSaveButtonClicked()
{
	SaveLoadWidget->SetMode(true);

	RemoveFromParent();
}

void UWidget_Settings::OnLoadButtonClicked()
{
	SaveLoadWidget->SetMode(false);

	RemoveFromParent();
}

