// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEditorPawn.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "math.h"
#include "BlockBase.h"
#include "CommandBlockBase.h"

ALevelEditorPawn::ALevelEditorPawn()
{
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

}

void ALevelEditorPawn::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator Rotator = { 0,0,0 };

	DummyBlock = GetWorld()->SpawnActor<AActor>(DummyActor, (FVector)(0,0,0) , Rotator, SpawnParams);
}


void ALevelEditorPawn::PlaceBlock()
{
	auto BlockClass = TSubclassOf<ABlockBase>(PlaceActor);

	if (BlockClass != NULL)
	{
		FVector CLocation = this->GetActorLocation();
		FVector CForwardVector = this->GetActorForwardVector();

		FHitResult hitResult;

		FCollisionQueryParams collisionParams;

		collisionParams.AddIgnoredActor(this);
		collisionParams.AddIgnoredActor(DummyBlock);

		FRotator Rotator = { 0,0,0 };
		if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Camera, collisionParams))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
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

			auto spawned = GetWorld()->SpawnActor<AActor>(PlaceActor, (FVector)hitResult.Location, Rotator, SpawnParams);
			TArray<AActor*> overlapped;
			spawned->GetOverlappingActors(overlapped);
			for (AActor* actor : overlapped)
			{
				actor->Destroy();
			}
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
		if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Camera, collisionParams))
		{
			FVector Location = hitResult.GetActor()->GetActorLocation();
			auto target = hitResult.GetActor();
			auto casted = Cast<ABlockBase>(target);
			if (casted != NULL)
			{
				if (casted->GetApplyCommandBlocks())
				{
					casted->ApplyMove();
				}

			}

		}
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

	// 디버깅용 라인
	//DrawDebugLine(GetWorld(), CLocation, CLocation + CForwardVector * 2000, FColor::Red, false, 1, 0, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams) && hitResult.GetActor()->ActorHasTag("Destroyable"))
	{
		hitResult.GetActor()->Destroy();
	}
}

void ALevelEditorPawn::DrawDummyBlock(float value)
{
	auto BlockClass = TSubclassOf<ABlockBase>(PlaceActor);

	if (BlockClass != NULL)
	{
		FVector CLocation = this->GetActorLocation();
		FVector CForwardVector = this->GetActorForwardVector();

		FHitResult hitResult;

		FCollisionQueryParams collisionParams;

		collisionParams.AddIgnoredActor(this);
		collisionParams.AddIgnoredActor(DummyBlock);

		FRotator Rotator = { 0,0,0 };
		if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Camera, collisionParams))
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
		if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Camera, collisionParams))
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
			casted->GetMovement(temp.Move_MaxCount, temp.Move_Speed);
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
			auto spawned = GetWorld()->SpawnActor<AActor>(block.blockclass, block.location, Rotator, SpawnParams);

			auto casted = Cast<ABlockBase>(spawned);

			if (casted)
			{
				casted->ApplyMoves(block.Move_MaxCount);
			}
		}
		

	}
	
}

void ALevelEditorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ADefaultPawn::SetupPlayerInputComponent(PlayerInputComponent);

	// 우클릭
	PlayerInputComponent->BindAction("PickTarget", IE_Pressed, this, &ALevelEditorPawn::PlaceBlock);

	// 좌클릭
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ALevelEditorPawn::DestroyBlock);

	PlayerInputComponent->BindAction("SaveGame", IE_Pressed, this, &ALevelEditorPawn::SaveGame);

	PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &ALevelEditorPawn::LoadGame);


	PlayerInputComponent->BindAxis("Turn", this, &ALevelEditorPawn::DrawDummyBlock);

	PlayerInputComponent->BindAxis("LookUp", this, &ALevelEditorPawn::DrawDummyBlock);

}



void ALevelEditorPawn::SetPlaceActor(UClass* Actor)
{
	PlaceActor = Actor;
}

UClass* ALevelEditorPawn::GetPlaceActor()
{
	return PlaceActor;
}



