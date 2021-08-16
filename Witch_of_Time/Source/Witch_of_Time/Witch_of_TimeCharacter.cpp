// Copyright Epic Games, Inc. All Rights Reserved.

#include "Witch_of_TimeCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "TimeControllableActorBase.h"

//////////////////////////////////////////////////////////////////////////
// AWitch_of_TimeCharacter

AWitch_of_TimeCharacter::AWitch_of_TimeCharacter()
{
	Player_name = TEXT("Player");

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

}

//////////////////////////////////////////////////////////////////////////
// Input

void AWitch_of_TimeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWitch_of_TimeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWitch_of_TimeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWitch_of_TimeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWitch_of_TimeCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AWitch_of_TimeCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AWitch_of_TimeCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AWitch_of_TimeCharacter::OnResetVR);

	PlayerInputComponent->BindAction("PickTarget", IE_Pressed, this, &AWitch_of_TimeCharacter::PickTarget);

	PlayerInputComponent->BindAction("SlowTime", IE_Pressed, this, &AWitch_of_TimeCharacter::SlowTime);

	PlayerInputComponent->BindAction("FastTime", IE_Pressed, this, &AWitch_of_TimeCharacter::FastTime);
}


void AWitch_of_TimeCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AWitch_of_TimeCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AWitch_of_TimeCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AWitch_of_TimeCharacter::PickTarget()
{
	FVector CLocation = FollowCamera->GetComponentLocation();
	FVector CForwardVector = FollowCamera->GetForwardVector();

	FHitResult hitResult;

	FCollisionQueryParams collisionParams;

	collisionParams.AddIgnoredActor(this);

	// 디버깅용 라인
	// DrawDebugLine(GetWorld(), CLocation, CLocation + CForwardVector * 2000, FColor::Red, false, 1, 0, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, CLocation, CLocation + CForwardVector * TIMEMAGIC_RANGE, ECC_WorldStatic, collisionParams))
	{
		PickedActor = hitResult.GetActor();
		if ((PickedActor->ActorHasTag("Returnable")) || (PickedActor->ActorHasTag("Jumpable")) || (PickedActor->ActorHasTag("Destroyable")) || (PickedActor->ActorHasTag("Restorable")))
		{
			DrawBillboard(PickedActor);

		}
		else
		{
			PickedActor = nullptr;
			DrawBillboard(PickedActor);
		}
	}

	
}

void AWitch_of_TimeCharacter::SlowTime()
{
	if (PickedActor!= NULL)
	{
		if (PickedActor->ActorHasTag("Returnable"))
		{
			dynamic_cast<ATimeControllableActorBase*>(PickedActor)->ReturnTime();
		}

		else if (PickedActor->ActorHasTag("Restorable") == false)
		{
			PickedActor->SetActorEnableCollision(false);
			PickedActor->Tags.Add(FName("Restorable"));
		}
	}
}

void AWitch_of_TimeCharacter::FastTime()
{
	if (PickedActor != NULL)
	{
		if (PickedActor->ActorHasTag("Jumpable"))
		{
			dynamic_cast<ATimeControllableActorBase*>(PickedActor)->JumpTime();
		}

		else if (PickedActor->ActorHasTag("Restorable") == true)
		{
			PickedActor->SetActorEnableCollision(true);
			PickedActor->Tags.Remove("Restorable");
		}
	}
}

void AWitch_of_TimeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWitch_of_TimeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AWitch_of_TimeCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWitch_of_TimeCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
