// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockBase.h"
#include "LiquidBlockBase.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_OF_TIME_API ALiquidBlockBase : public ABlockBase
{
	GENERATED_BODY()
protected:
	ALiquidBlockBase();
	UPROPERTY(EditAnywhere)
	float m_height = 1.0f;

	ALiquidBlockBase* ParentBlock;

	UFUNCTION(BlueprintCallable, Category = "Placing")
	void SetHeight(float height);
	UFUNCTION(BlueprintCallable, Category = "Placing")
	float GetHeight();

	void SetParant(ALiquidBlockBase* Parent);

	void PlaceDummy(FVector location);

public:
	UPROPERTY(EditAnywhere)
	UClass* Place;

	

	float Elapsed_Time = 0.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
