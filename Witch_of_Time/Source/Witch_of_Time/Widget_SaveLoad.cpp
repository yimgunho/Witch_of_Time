// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SaveLoad.h"
#include "BlockBase.h"
#include "Misc/DateTime.h"



void UWidget_SaveLoad::NativeConstruct()
{
	Super::NativeConstruct();

	MainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Title")));

	SaveList = Cast<UScrollBox>(GetWidgetFromName(TEXT("SaveList")));

}

TArray<FString> UWidget_SaveLoad::GetAllSaveGameSlotNames()
{
	class FFindSavesVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		FFindSavesVisitor() {}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
		{
			if (!bIsDirectory)
			{
				FString FullFilePath(FilenameOrDirectory);
				if (FPaths::GetExtension(FullFilePath) == TEXT("sav"))
				{
					FString CleanFilename = FPaths::GetBaseFilename(FullFilePath);
					CleanFilename = CleanFilename.Replace(TEXT(".sav"), TEXT(""));
					SavesFound.Add(CleanFilename);
				}
			}
			return true;
		}
		TArray<FString> SavesFound;
	};

	TArray<FString> Saves;
	const FString SavesFolder = FPaths::ProjectSavedDir() + TEXT("SaveGames");

	if (!SavesFolder.IsEmpty())
	{
		FFindSavesVisitor Visitor;
		FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*SavesFolder, Visitor);
		Saves = Visitor.SavesFound;
	}

	return Saves;
}

void UWidget_SaveLoad::SetMode(bool Mode)
{
	MainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Title")));

	SaveList = Cast<UScrollBox>(GetWidgetFromName(TEXT("SaveList")));
	IsSaveMode = Mode;
	if (Mode)
	{
		MainText->SetText(FText::FromString("Save"));
	}
	else
	{
		MainText->SetText(FText::FromString("Load"));
	}
	RefreshSaveList();
}


