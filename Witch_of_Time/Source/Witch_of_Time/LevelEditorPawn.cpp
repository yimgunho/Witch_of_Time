// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEditorPawn.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "math.h"
#include "CommandBlockBase.h"
#include "../../../Server/Server/TCPServer.h"
float old_location_x;
float old_location_y;
float old_location_z;
FVector location_to_FVector;

ALevelEditorPawn::ALevelEditorPawn()
{
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

}

void ALevelEditorPawn::BeginPlay()
{
	Super::BeginPlay();

	blockindex_arr_level_from_CL.Init(0, MAXLOADBLOCK);
	X_arr_level_from_CL.Init(0, MAXLOADBLOCK);
	Y_arr_level_from_CL.Init(0, MAXLOADBLOCK);
	Z_arr_level_from_CL.Init(0, MAXLOADBLOCK);
	//blockindex_to_uclass.Reserve(MAXLOADBLOCK);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator Rotator = { 0,0,0 };

	DummyBlock = GetWorld()->SpawnActor<AActor>(DummyActor, (FVector)(0,0,0) , Rotator, SpawnParams);

	old_location_x = 0;
	old_location_y = 0;
	old_location_z = 0;
	blockindex = 1;
	ToDestroyBlockName = "none";

}

void ALevelEditorPawn::LeftMouseFunc(bool flag)
{
	LeftMousePressed = flag;
	if (flag)
	{
		DestroyBlock();
		GetWorld()->GetTimerManager().SetTimer(LeftButtonTimer, this, &ALevelEditorPawn::DestroyBlock, MinPlaceTime, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(LeftButtonTimer);
	}
}

void ALevelEditorPawn::RightMouseFunc(bool flag)
{
	RightMousePressed = flag;
	if (flag)
	{
		PlaceBlock();
		GetWorld()->GetTimerManager().SetTimer(RightButtonTimer, this, &ALevelEditorPawn::PlaceBlock, MinPlaceTime, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RightButtonTimer);
	}
}

void ALevelEditorPawn::SwitchPlaceMode()
{
	CommandBlockMode = !CommandBlockMode;
}

void ALevelEditorPawn::Location_Clear()
{
	location_x = 0;
	location_y = 0;
	location_z = 0;
	temp_location_x = 0;
	temp_location_y = 0;
	temp_location_z = 0;
}
void ALevelEditorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//if (((X_arr_level_from_CL[0] > 0) && (Y_arr_level_from_CL[0] > 0) && (Z_arr_level_from_CL[0] > 0)) == true)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Entered Loading");
	//	FRotator Rotator = { 0,0,0 };

	//	FActorSpawnParameters SpawnParams;

	//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//	for (int i = 0; i < id_arr_level_from_CL.Num(); ++i)
	//	{
	//		if ((X_arr_level_from_CL[i] > 0 && Y_arr_level_from_CL[i] > 0 && Z_arr_level_from_CL[i] > 0) == true) 
	//		{
	//			//SetClassOfPlacedBlock(blockindex_to_uclass[0]);
	//			auto spawned_Load = GetWorld()->SpawnActor<AActor>(ClassOfPlacedBlock, { X_arr_level_from_CL[i],Y_arr_level_from_CL[i],Z_arr_level_from_CL[i] }, Rotator, SpawnParams);
	//		}
	//	}
	//	blockindex_arr_level_from_CL.Empty();
	//	X_arr_level_from_CL.Empty();
	//	Y_arr_level_from_CL.Empty();
	//	Z_arr_level_from_CL.Empty();
	//	blockindex_to_uclass.Empty();

	//}
	//location_to_FVector = { temp_location_x, temp_location_y, temp_location_z };

	//if (!(temp_location_x == 0 && temp_location_y == 0 && temp_location_z == 0) && (temp_location_x > 0 && temp_location_y > 0 && temp_location_z > 0))
	//{
	//	FRotator Rotator = { 0,0,0 };

	//	FActorSpawnParameters SpawnParams;

	//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	//	auto spawned_by_server = GetWorld()->SpawnActor<AActor>(ClassOfPlacedBlock, location_to_FVector, Rotator, SpawnParams);


	//	old_location_x = temp_location_x;
	//	old_location_y = temp_location_y;
	//	old_location_z = temp_location_z;

	//	location_x = 0;
	//	location_y = 0;
	//	location_z = 0;
	//	temp_location_x = 0;
	//	temp_location_y = 0;
	//	temp_location_z = 0;
	//}

	

	//ToDestroyBlock->Destroy();
}

void ALevelEditorPawn::PlaceBlock()
{
	if (!CommandBlockMode)
	{
		FVector CLocation = this->GetActorLocation();
		FVector CForwardVector = this->GetActorForwardVector();

		FHitResult hitResult;

		FCollisionQueryParams collisionParams;

		collisionParams.AddIgnoredActor(this);
		collisionParams.AddIgnoredActor(DummyBlock);

		FRotator Rotator = { 0,0,0 };
		if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector Location = hitResult.GetComponent()->GetComponentLocation();

			Location -= (FVector)hitResult.Location;

			SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

			if (fabsf(Location.X - 100.f) < 0.01f || fabsf(Location.X + 100.f) < 0.01f)
			{
				hitResult.Location.X -= Location.X;
			}
			else
			{
				hitResult.Location.X = hitResult.GetComponent()->GetComponentLocation().X;
			}

			if (fabsf(Location.Y - 100.f) < 0.01f || fabsf(Location.Y + 100.f) < 0.01f)
			{
				hitResult.Location.Y -= Location.Y;
			}
			else
			{
				hitResult.Location.Y = hitResult.GetComponent()->GetComponentLocation().Y;
			}

			if (fabsf(Location.Z + 200.f) < 0.01f || fabsf(Location.Z) < 0.01f)
			{
				if (fabsf(Location.Z + 200.f) < 0.01f)
					hitResult.Location.Z = hitResult.GetComponent()->GetComponentLocation().Z + 200.f;
				else
					hitResult.Location.Z = hitResult.GetComponent()->GetComponentLocation().Z - 200.f;
			}
			else
			{
				hitResult.Location.Z = hitResult.GetComponent()->GetComponentLocation().Z;
			}

			location_x = hitResult.Location.X;
			location_y = hitResult.Location.Y;
			location_z = hitResult.Location.Z;

			//auto spawned = GetWorld()->SpawnActor<AActor>(PlaceActor, (FVector)hitResult.Location, Rotator, SpawnParams);
			client->send_block_packet(blockindex, hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z);

			TArray<AActor*> overlapped;
			//spawned->GetOverlappingActors(overlapped);

			for (AActor* actor : overlapped)
			{
				actor->Destroy();
			}

			//BlockName = spawned->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Block Spawned");
		}
	}
	else
	{
		FVector CLocation = this->GetActorLocation();
		FVector CForwardVector = this->GetActorForwardVector();

		FHitResult hitResult;

		FCollisionQueryParams collisionParams;

		collisionParams.AddIgnoredActor(this);
		collisionParams.AddIgnoredActor(DummyBlock);

		FRotator Rotator = { 0,0,0 };
		if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams))
		{
			FVector Location = hitResult.GetActor()->GetActorLocation();
			auto target = hitResult.GetActor();
			auto casted = Cast<ABlockBase>(target);
			if (casted != NULL)
			{
				if (casted->GetApplyCommandBlocks())
				{
					OpenCommandList(casted);
				}

			}

		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Tried Block Spawning but not proper MODE");
	}

	
	
}

void ALevelEditorPawn::DestroyBlock()
{
	FVector CLocation = this->GetActorLocation();
	FVector CForwardVector = this->GetActorForwardVector();

	FHitResult hitResult;

	FCollisionQueryParams collisionParams;

	collisionParams.AddIgnoredActor(this);
	collisionParams.AddIgnoredActor(DummyBlock);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams) && hitResult.GetActor()->ActorHasTag("Destroyable"))
	{
		ToDestroyBlock = hitResult.GetActor();
		
		ToDestroyBlockName = hitResult.GetActor()->GetName();
		//todestroyblockid = 0;
		//hitResult.GetActor()->Destroy();
		client->send_destroy_packet(Cast<ABlockBase>(ToDestroyBlock)->block_id);
	}
}

void ALevelEditorPawn::DrawDummyBlock(float value)
{
	if (VisibleFlag)
	{
		if (!CommandBlockMode)
		{
			FVector CLocation = this->GetActorLocation();
			FVector CForwardVector = this->GetActorForwardVector();

			FHitResult hitResult;

			FCollisionQueryParams collisionParams;

			collisionParams.AddIgnoredActor(this);
			collisionParams.AddIgnoredActor(DummyBlock);

			FRotator Rotator = { 0,0,0 };
			if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams))
			{
				FVector Location = hitResult.GetComponent()->GetComponentLocation();

				Location -= (FVector)hitResult.Location;

				if (fabsf(Location.X - 100.f) < 0.01f || fabsf(Location.X + 100.f) < 0.01f)
				{
					hitResult.Location.X -= Location.X;
				}
				else
				{
					hitResult.Location.X = hitResult.GetComponent()->GetComponentLocation().X;
				}

				if (fabsf(Location.Y - 100.f) < 0.01f || fabsf(Location.Y + 100.f) < 0.01f)
				{
					hitResult.Location.Y -= Location.Y;
				}
				else
				{
					hitResult.Location.Y = hitResult.GetComponent()->GetComponentLocation().Y;
				}

				if (fabsf(Location.Z + 200.f) < 0.01f || fabsf(Location.Z) < 0.01f)
				{
					if (fabsf(Location.Z + 200.f) < 0.01f)
						hitResult.Location.Z = hitResult.GetComponent()->GetComponentLocation().Z + 200.f;
					else
						hitResult.Location.Z = hitResult.GetComponent()->GetComponentLocation().Z - 200.f;
				}
				else
				{
					hitResult.Location.Z = hitResult.GetComponent()->GetComponentLocation().Z;
				}


				DummyBlock->SetActorLocation(hitResult.Location);
				DummyBlock->SetActorHiddenInGame(false);

			}
			else
			{
				DummyBlock->SetActorHiddenInGame(true);
			}
		}
		else
		{
			FVector CLocation = this->GetActorLocation();
			FVector CForwardVector = this->GetActorForwardVector();

			FHitResult hitResult;

			FCollisionQueryParams collisionParams;

			collisionParams.AddIgnoredActor(this);
			collisionParams.AddIgnoredActor(DummyBlock);

			FRotator Rotator = { 0,0,0 };
			if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams))
			{
				FVector Location = hitResult.GetActor()->GetActorLocation();
				auto target = hitResult.GetActor();
				auto casted = Cast<ABlockBase>(target);
				if (casted != NULL)
				{
					if (casted->GetApplyCommandBlocks())
					{
						DummyBlock->SetActorLocation(Location);
						DummyBlock->SetActorHiddenInGame(false);
					}

				}
				else
				{
					DummyBlock->SetActorHiddenInGame(true);
				}
			}
			else
			{
				DummyBlock->SetActorHiddenInGame(true);
			}
		}
	}

	
}

void ALevelEditorPawn::SaveGame()
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

	UGameplayStatics::SaveGameToSlot(Instance, Instance->SaveSlotName, Instance->UserIndex);
}

void ALevelEditorPawn::LoadGame()
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

		LoadGameInstance = Cast<USaveEditorLevel>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
		if (!IsValid(LoadGameInstance))
		{
			return;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		for (auto block : LoadGameInstance->blockarray)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "LoadPlace");
			auto spawned = GetWorld()->SpawnActor<AActor>(block.blockclass, block.location, Rotator, SpawnParams);

			auto casted = Cast<ABlockBase>(spawned);

			if (casted)
			{
				casted->CommandBlockArray = block.CommandArray;
			}
		}
		

	}
	
}

int ALevelEditorPawn::GetBlockIndex()
{
	return blockindex;
}

void ALevelEditorPawn::SetClassOfPlacedBlock(UClass* blockclass)
{
	ClassOfPlacedBlock = blockclass;
}

AActor* ALevelEditorPawn::SpawnDummyActor(UClass* blockclass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Name = FName("DummyActor");
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	auto spawned = GetWorld()->SpawnActor<AActor>(blockclass, (FVector)(-10000, -10000, -10000), (FRotator)(0, 0, 0), SpawnParams);
	return spawned;
}

void ALevelEditorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ADefaultPawn::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SaveGame", IE_Pressed, this, &ALevelEditorPawn::SaveGame);

	PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &ALevelEditorPawn::LoadGame);

	
	// 우클릭
	PlayerInputComponent->BindAction<OneParamBool>("PickTarget", IE_Pressed, this,&ALevelEditorPawn::RightMouseFunc, true);
	// 좌클릭
	PlayerInputComponent->BindAction<OneParamBool>("Attack", IE_Pressed, this, &ALevelEditorPawn::LeftMouseFunc, true);

	// 우클릭
	PlayerInputComponent->BindAction<OneParamBool>("PickTarget", IE_Released, this, &ALevelEditorPawn::RightMouseFunc, false);
	// 좌클릭
	PlayerInputComponent->BindAction<OneParamBool>("Attack", IE_Released, this, &ALevelEditorPawn::LeftMouseFunc, false);

	// R키
	PlayerInputComponent->BindAction("SwitchPlaceMode", IE_Pressed, this, &ALevelEditorPawn::SwitchPlaceMode);

	PlayerInputComponent->BindAxis("Turn", this, &ALevelEditorPawn::DrawDummyBlock);

	PlayerInputComponent->BindAxis("LookUp", this, &ALevelEditorPawn::DrawDummyBlock);

}

void ALevelEditorPawn::SetVisibleFlag(bool flag)
{
	VisibleFlag = flag;
}



void ALevelEditorPawn::SetPlaceActor(UClass* Actor)
{
	PlaceActor = Actor;
}

UClass* ALevelEditorPawn::GetPlaceActor()
{
	return PlaceActor;
}

AActor* ALevelEditorPawn::GetDummyBlock()
{
	return DummyBlock;
}



