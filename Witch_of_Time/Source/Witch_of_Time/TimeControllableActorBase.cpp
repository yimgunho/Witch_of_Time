// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeControllableActorBase.h"

// Sets default values
ATimeControllableActorBase::ATimeControllableActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Returnable"));
	Tags.Add(FName("Jumpable"));

	m_CurrentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Current Mesh"));

	RootComponent = m_CurrentMesh;

	m_PastMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Past Mesh"));

	m_PastMesh->SetVisibility(false);

	m_PastMesh->SetupAttachment(RootComponent);

	m_FutureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Future Mesh"));

	m_FutureMesh->SetVisibility(false);

	m_FutureMesh->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ATimeControllableActorBase::BeginPlay()
{
	TimeBlock_id = -1;
	Super::BeginPlay();
	
}

void ATimeControllableActorBase::ResetBlock_Implementation()
{
	Super::ResetBlock_Implementation();
	CurrentMesh = InitMesh;
	InitScale = ResetScale;
	ChangeMesh();
	SetActorScale3D(FVector(ResetScale));
	this->SetActorTickEnabled(true);
}

void ATimeControllableActorBase::ReturnTime()
{
	if (CurrentMesh != Past && !Changing)
	{
		client->send_time_packet(block_id, 0);
	}

}

void ATimeControllableActorBase::ReturnTimeNiagara()
{
	if (CurrentMesh != Past && !Changing)
	{
		if (CurrentMesh == Current)
		{
			if (m_Current_to_Past != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_Current_to_Past, this->GetActorLocation());
				m_CurrentMesh->SetVisibility(false);
			}
		}
		if (CurrentMesh == Future)
		{
			if (m_Future_to_Current != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_Future_to_Current, this->GetActorLocation());
				m_FutureMesh->SetVisibility(false);
			}
		}

		CurrentMesh--;
		Changing = true;
		IsFuture = false;
		Elapsed_Time = 0.f;
	}

}

void ATimeControllableActorBase::JumpTime()
{
	if (CurrentMesh != Future && !Changing)
	{
		JumpTimeBlock_id = TimeBlock_id;
		client->send_time_packet(block_id, 1);
	}
}

void ATimeControllableActorBase::JumpTimeNiagara()
{
	if (CurrentMesh != Future && !Changing)
	{
		if (CurrentMesh == Current)
		{
			if (m_Current_to_Future != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_Current_to_Future, this->GetActorLocation());
				m_CurrentMesh->SetVisibility(false);
			}
		}
		if (CurrentMesh == Past)
		{
			if (m_Past_to_Current != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_Past_to_Current, this->GetActorLocation());
				m_PastMesh->SetVisibility(false);
			}
		}

		CurrentMesh++;
		Changing = true;
		IsFuture = true;
		Elapsed_Time = 0.f;
	}
}

void ATimeControllableActorBase::ChangeMesh()
{
	switch (CurrentMesh)
	{
	case Past:
		m_PastMesh->SetVisibility(true);
		m_CurrentMesh->SetVisibility(false);
		m_FutureMesh->SetVisibility(false);
		m_PastMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		m_CurrentMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_FutureMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case Current:
		m_PastMesh->SetVisibility(false);
		m_CurrentMesh->SetVisibility(true);
		m_FutureMesh->SetVisibility(false);
		m_PastMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_CurrentMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		m_FutureMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case Future:
		m_PastMesh->SetVisibility(false);
		m_CurrentMesh->SetVisibility(false);
		m_FutureMesh->SetVisibility(true);
		m_PastMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_CurrentMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		m_FutureMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	}
}

void ATimeControllableActorBase::InitalizeMesh()
{
	CurrentMesh = InitMesh;
	ResetScale = InitScale;
}

// Called every frame
void ATimeControllableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Changing)
	{
		Elapsed_Time += DeltaTime;

		if (Elapsed_Time > 2.0f)
		{
			/*
			if (IsFuture)
			{
				CurrScale = FMath::Lerp<float>(InitScale, 5 * InitScale, 1.f);
			}
			else
			{
				CurrScale = FMath::Lerp<float>(0.2 * InitScale, InitScale, 0.f);
			}
			SetActorScale3D(FVector(CurrScale));
			*/
			Elapsed_Time = 0.f;
			ChangeMesh();
			Changing = false;
			InitScale = CurrScale;
		}
		/*
		else
		{

			if (IsFuture)
			{
				CurrScale = FMath::Lerp<float>(InitScale, 5 * InitScale, Elapsed_Time);
			}
			else
			{
				CurrScale = FMath::Lerp<float>(0.2 * InitScale, InitScale, 1.f - Elapsed_Time);
			}
			SetActorScale3D(FVector(CurrScale));
		}
		*/
	}

}

