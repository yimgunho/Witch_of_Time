// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "client.h"
#include "GameFramework/Character.h"
#include "monster.generated.h"

UCLASS()
class WITCH_OF_TIME_API Amonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Amonster();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		Aclient* client;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
