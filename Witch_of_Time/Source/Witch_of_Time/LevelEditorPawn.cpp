// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEditorPawn.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "math.h"

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
	FVector CLocation = this->GetActorLocation();
	FVector CForwardVector = this->GetActorForwardVector();

	FHitResult hitResult;

	FCollisionQueryParams collisionParams;

	collisionParams.AddIgnoredActor(this);
	collisionParams.AddIgnoredActor(DummyBlock);

	// 디버깅용 라인
	// DrawDebugLine(GetWorld(), CLocation, CLocation + CForwardVector * 2000, FColor::Red, false, 1, 0, 1.0f);

	FRotator Rotator = { 0,0,0 };
	if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Camera, collisionParams))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector Location = hitResult.GetComponent()->GetComponentLocation();


		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Location.ToString());

		Location -= (FVector)hitResult.Location;

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Location.ToString());
		
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

		GetWorld()->SpawnActor<AActor>(PlaceActor,(FVector)hitResult.Location, Rotator, SpawnParams);
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


		DummyBlock->SetActorLocation(hitResult.Location);
	}
}


void ALevelEditorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ADefaultPawn::SetupPlayerInputComponent(PlayerInputComponent);

	// 우클릭
	PlayerInputComponent->BindAction("PickTarget", IE_Pressed, this, &ALevelEditorPawn::PlaceBlock);

	// 좌클릭
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ALevelEditorPawn::DestroyBlock);



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