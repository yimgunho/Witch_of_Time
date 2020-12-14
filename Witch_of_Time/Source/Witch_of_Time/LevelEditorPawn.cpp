// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEditorPawn.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

ALevelEditorPawn::ALevelEditorPawn()
{
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
}


void ALevelEditorPawn::PlaceBlock()
{
	FVector CLocation = this->GetActorLocation();
	FVector CForwardVector = this->GetActorForwardVector();

	FHitResult hitResult;

	FCollisionQueryParams collisionParams;

	collisionParams.AddIgnoredActor(this);

	// 디버깅용 라인
	DrawDebugLine(GetWorld(), CLocation, CLocation + CForwardVector * 2000, FColor::Red, false, 1, 0, 1.0f);

	FRotator Rotator;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
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

	// 디버깅용 라인
	DrawDebugLine(GetWorld(), CLocation, CLocation + CForwardVector * 2000, FColor::Red, false, 1, 0, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * 2000, ECC_Visibility, collisionParams) && hitResult.GetActor()->ActorHasTag("Destroyable"))
	{
		hitResult.GetActor()->Destroy();
	}
}

void ALevelEditorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ADefaultPawn::SetupPlayerInputComponent(PlayerInputComponent);

	// 우클릭
	PlayerInputComponent->BindAction("PickTarget", IE_Pressed, this, &ALevelEditorPawn::PlaceBlock);

	// 좌클릭
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ALevelEditorPawn::DestroyBlock);

}
