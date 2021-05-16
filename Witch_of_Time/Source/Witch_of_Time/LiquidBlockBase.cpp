// Fill out your copyright notice in the Description page of Project Settings.


#include "LiquidBlockBase.h"
#include "Kismet/KismetSystemLibrary.h"

ALiquidBlockBase::ALiquidBlockBase()
{
	Types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	Types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	Ignores.Add(this);
	OriginLocation = this->GetActorLocation();
}

void ALiquidBlockBase::SetHeight(float height, bool IsFlow)
{
	if (!IsFlow)
	{
		m_height = height;
		SetActorScale3D(FVector(0.99, 0.99, m_height));
	}
	else
	{
		m_height = std::min(m_height, 0.99f);
		SetActorScale3D(FVector(0.99, 0.99, 0.99));
	}
}

float ALiquidBlockBase::GetHeight()
{
	return m_height;
}

void ALiquidBlockBase::SetParant(ALiquidBlockBase* Parent)
{
	ParentBlock = Parent;
}

void ALiquidBlockBase::SetGrandParant(ALiquidBlockBase* Parent)
{
	GrandParentBlock = Parent;
}

bool ALiquidBlockBase::PlaceDummy(FVector location, bool IsFlow)
{
	if (location.X < 0.f || location.Y < 0.f)
		return false;
	FRotator Rotator = { 0,0,0 };
	int spawncount = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	auto PlacedActor = GetWorld()->SpawnActor<AActor>(Place, location, Rotator, SpawnParams);
	auto casted = Cast<ALiquidBlockBase>(PlacedActor);
	if (IsFlow == false)
	{
		if (casted != NULL)
		{
			casted->SetHeight(m_height - 0.2f,false);
			casted->SetParant(this);
			casted->SetGrandParant(GrandParentBlock);
			ChildBlocks.Add(casted);
			spawncount++;
		}
		else
		{
			UKismetSystemLibrary::SphereOverlapActors(GetWorld(), location, 100.f, Types, NULL, Ignores, Actors);
			for (AActor* Actor : Actors)
			{
				casted = Cast<ALiquidBlockBase>(Actor);
				if (casted != NULL)
				{
					if (casted->m_height < m_height - 0.2f)
					{
						casted->SetHeight(m_height - 0.2f, false);
						casted->SetParant(this);
						casted->SetGrandParant(GrandParentBlock);
						ChildBlocks.Add(casted);
						spawncount++;
					}
				}

			}
		}
	}
	else
	{
		

		if (casted != NULL)
		{
			casted->SetHeight(m_height,true);
			casted->SetParant(this);
			casted->SetGrandParant(GrandParentBlock);
			spawncount++;

			for (auto actor : ChildBlocks)
			{
				actor->SetParant(NULL);
				actor->SetGrandParant(NULL);
			}
		}
		else
		{
			UKismetSystemLibrary::SphereOverlapActors(GetWorld(), location, 100.f, Types, NULL, Ignores, Actors);
			for (AActor* Actor : Actors)
			{
				casted = Cast<ALiquidBlockBase>(Actor);
				if (casted != NULL && (casted != casted->GrandParentBlock))
				{
					casted->SetHeight(m_height ,true);
					casted->SetParant(this);
					spawncount++;
					
				}
			}
		}
	}
	

	if (spawncount > 0)
		return true;
	else
		return false;
}

void ALiquidBlockBase::BeginPlay()
{
	ABlockBase::BeginPlay();

	
}
void ALiquidBlockBase::ResetBlock()
{
	Super::ResetBlock();
	this->SetActorTickEnabled(true);
}



void ALiquidBlockBase::Tick(float DeltaTime)
{
	ABlockBase::Tick(DeltaTime);

	Elapsed_Time += DeltaTime;
	if (Elapsed_Time > 0.6f)
	{
		if (m_height != 1.f && !IsValid(ParentBlock))
		{
			this->Destroy();
		}
		if (!IsValid(GrandParentBlock))
		{
			DestroyCount++;
			if (DestroyCount > 5)
			{
				this->Destroy();
			}
		}

		FVector Location = this->GetActorLocation();
		auto flow = PlaceDummy(Location - FVector(0, 0, 199), true);

		if (!flow)
		{
			if (m_height > 0.3f)
			{
				PlaceDummy(Location - FVector(200, 0, -1), false);
				PlaceDummy(Location + FVector(200, 0, 1), false);
				PlaceDummy(Location - FVector(0, 200, -1), false);
				PlaceDummy(Location + FVector(0, 200, 1), false);

			}
		}

		Elapsed_Time = 0.f;
		this->SetActorTickEnabled(false);
	}
	
}
