// Fill out your copyright notice in the Description page of Project Settings.


#include "LiquidBlockBase.h"
#include "Kismet/KismetSystemLibrary.h"

ALiquidBlockBase::ALiquidBlockBase()
{
	Types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	Types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	Ignores.Add(this);
}

void ALiquidBlockBase::SetHeight(float height)
{
	m_height = height;
	SetActorScale3D(FVector(0.99, 0.99, m_height));
}

float ALiquidBlockBase::GetHeight()
{
	return m_height;
}

void ALiquidBlockBase::SetParant(ALiquidBlockBase* Parent)
{
	ParentBlock = Parent;
}

void ALiquidBlockBase::PlaceDummy(FVector location)
{
	FRotator Rotator = { 0,0,0 };

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	
	auto PlacedActor = GetWorld()->SpawnActor<AActor>(Place, location, Rotator, SpawnParams);
	auto casted = Cast<ALiquidBlockBase>(PlacedActor);
	if (casted != NULL)
	{
		casted->SetHeight(m_height - 0.2f);
		casted->SetParant(this);
	}
	else
	{
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), location, 100.f, Types, NULL, Ignores, Actors);
		for (AActor* Actor : Actors)
		{
			casted = Cast< ALiquidBlockBase>(Actor);
			if (casted != NULL)
			{
				if (casted->m_height < m_height - 0.2f)
				{
					casted->SetHeight(m_height - 0.2f);
					casted->SetParant(this);
				}
			}

		}
	}
}

void ALiquidBlockBase::BeginPlay()
{
	ABlockBase::BeginPlay();

	
}



void ALiquidBlockBase::Tick(float DeltaTime)
{
	ABlockBase::Tick(DeltaTime);

	Elapsed_Time += DeltaTime;
	if (Elapsed_Time > 1.f)
	{
		if (m_height > 0.3f)
		{
			FVector Location = this->GetActorLocation();

			PlaceDummy(Location - FVector(200, 0, -1));
			PlaceDummy(Location + FVector(200, 0, 1));
			PlaceDummy(Location - FVector(0, 200, -1));
			PlaceDummy(Location + FVector(0, 200, 1));

		}

		if (m_height != 1.f && !IsValid(ParentBlock))
		{
			this->Destroy();
		}
		Elapsed_Time = 0.f;
	}
	
}
