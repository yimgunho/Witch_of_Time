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

// Called when the game starts or when spawned
void ABlockBase::BeginPlay()
{
	Super::BeginPlay();
	if (ApplyHeatNearBlocks)
	{
		this->SetActorTickEnabled(true);
	}
	
}

// Called every frame
void ABlockBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ApplyHeatNearBlocks)
	{
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

}

