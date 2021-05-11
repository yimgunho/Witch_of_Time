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
	Super::BeginPlay();
	
}

void ATimeControllableActorBase::ReturnTime()
{
	if (CurrentMesh != Past)
	{
		CurrentMesh--;
		UpdateMesh();
	}
}

void ATimeControllableActorBase::JumpTime()
{
	if (CurrentMesh != Future)
	{
		CurrentMesh++;
		UpdateMesh();
	}
}

void ATimeControllableActorBase::UpdateMesh()
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

// Called every frame
void ATimeControllableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

