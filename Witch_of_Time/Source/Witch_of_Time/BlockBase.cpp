// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABlockBase::ABlockBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	Types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	Ignores.Add(this);

	OriginLocation = this->GetActorLocation();
}

void ABlockBase::ApplyHeatEnergy(float heat)
{
	temperature += heat / mess;
	if (temperature > MeltingPoint && m_MeltingClass != NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto spawned = GetWorld()->SpawnActor<AActor>(m_MeltingClass, this->GetActorLocation(), this->GetActorRotation(), SpawnParams);

		

		Cast<ABlockBase>(spawned)->temperature = temperature;
		this->Destroy();
	}
	else if (temperature < FreezingPoint && m_FreezingClass != NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		auto spawned = GetWorld()->SpawnActor<AActor>(m_FreezingClass, this->GetActorLocation(), this->GetActorRotation(), SpawnParams);

		Cast<ABlockBase>(spawned)->temperature = temperature;
		this->Destroy();
	}
}

void ABlockBase::ApplyMove()
{
	this->SetActorTickEnabled(true);
	Move_MaxCount += 1;
	Move_Speed += 50;
}

void ABlockBase::ApplyMoves(int Count)
{
	this->SetActorTickEnabled(true);
	Move_MaxCount += Count;
	Move_Speed += 50 * Count;
}

FVector ABlockBase::GetOrigin()
{
	return OriginLocation;
}

bool ABlockBase::GetIsMovable()
{
	return IsMovable;
}

void ABlockBase::ResetBlock()
{
	this->SetActorLocation(OriginLocation);
	CommandBlockInitailized = false;
	CurrentCommandBlock = 0;
	this->SetActorTickEnabled(true);
}

void ABlockBase::GetMovement(int& maxcount, float& speed)
{
	maxcount = Move_MaxCount;
	speed = Move_Speed;

}

void ABlockBase::ExecuteCommandBlock(FCommandBlockInfo block, float DeltaTime)
{
	switch (block.index)
	{
	case 0: // MoveBlock
	{
		FVector location = this->GetActorLocation();
		if (!CommandBlockInitailized)
		{
			TargetLocation = FVector(location.X + block.data[0] * 200, location.Y + block.data[1] * 200, location.Z + block.data[2] * 200);
			DirectionVector = (FVector(block.data[0] * 200, block.data[1] * 200, block.data[2] * 200));
			TargetDistance = DirectionVector.Size();
			DirectionVector.Normalize();
			CommandBlockInitailized = true;
		}

		float temp = 100 * DeltaTime;
		if (temp >= TargetDistance)
		{
			this->SetActorLocation(TargetLocation);
			if (CommandBlockArray.IsValidIndex(CurrentCommandBlock + 1))
			{
				CommandBlockInitailized = false;
				CurrentCommandBlock++;
			}
		}
		else
		{
			this->SetActorLocation(location + DirectionVector * temp);
			TargetDistance -= temp;
		}

	}
		break;
	case 1:
		CurrentCommandBlock = 0;
		break;
	case 2:
		ResetBlock();
		break;
	}
}

bool ABlockBase::GetApplyCommandBlocks()
{
	return ApplyCommandBlocks;
}

// Called when the game starts or when spawned
void ABlockBase::BeginPlay()
{
	Super::BeginPlay();
	if (ApplyHeatNearBlocks)
	{
		this->SetActorTickEnabled(true);
	}
	OriginLocation = this->GetActorLocation();

}

// Called every frame
void ABlockBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ApplyHeatNearBlocks)
	{
		Actors.Empty();
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), this->GetActorLocation(), 1000.f, Types, NULL, Ignores, Actors);

		for (AActor* actor : Actors)
		{
			auto picked = Cast<ABlockBase>(actor);
			if (picked != NULL)
			{
				if (picked->ApplyStateChange)
				{
					float distance = (this->GetActorLocation() - actor->GetActorLocation()).Size();
					float tempdiff = this->temperature - picked->temperature;
					picked->ApplyHeatEnergy(tempdiff / (distance + 1.f));
				}
					
			}
			
		}
		
	}

	if (CommandBlockArray.Num() != 0)
	{
		ExecuteCommandBlock(CommandBlockArray[CurrentCommandBlock], DeltaTime);
	}


	/*
	* move ������
	if (IsMovable && Move_MaxCount != 0)
	{
		if (MoveChange)
		{
			FVector location = this->GetActorLocation();
			location.X -= Move_Speed * DeltaTime;

			
			Actors.Empty();
			FVector temp = location;

			temp.Z += 100.f;

			UKismetSystemLibrary::BoxOverlapActors(GetWorld(), temp, FVector(95.f, 95.f, 95.f), Types, NULL, Ignores, Actors);

			if (Actors.Num() != 0)
			{
				MoveChange = !MoveChange;
				for (auto target : Actors)
				{
					if (target->ActorHasTag("Dummy"))
					{
						SetActorLocation(location);
						target->Destroy();
						MoveChange = !MoveChange;

						break;
					}
				}
				
			}
			else
			{
				SetActorLocation(location);
			}

			if (fabs(location.X - OriginLocation.X) > Move_MaxCount * 200.f)
			{
				location.X = OriginLocation.X - 200.f * Move_MaxCount;

				SetActorLocation(location);

				MoveChange = !MoveChange;
			}
		}
		else
		{
			FVector location = this->GetActorLocation();
			location.X += Move_Speed * DeltaTime;

			Actors.Empty();
			FVector temp = location;

			temp.Z += 100.f;

			UKismetSystemLibrary::BoxOverlapActors(GetWorld(), temp, FVector(95.f, 95.f, 95.f), Types, NULL, Ignores, Actors);


			if (Actors.Num() != 0)
			{
				MoveChange = !MoveChange;
				for (auto target : Actors)
				{
					if (target->ActorHasTag("Dummy"))
					{
						SetActorLocation(location);
						target->Destroy();
						MoveChange = !MoveChange;

						break;
					}
				}
			}
			else
			{
				SetActorLocation(location);
			}

			if (fabs(location.X - OriginLocation.X) > Move_MaxCount * 200.f)
			{
				location.X = OriginLocation.X + 200.f * Move_MaxCount;

				SetActorLocation(location);

				MoveChange = !MoveChange;
			}
		}

		
	}
	*/
}

