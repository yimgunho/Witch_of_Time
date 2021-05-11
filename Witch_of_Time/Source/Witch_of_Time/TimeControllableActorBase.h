// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockBase.h"
#include "TimeControllableActorBase.generated.h"

enum MeshIndex {
	Past, Current, Future
};

UCLASS()
class WITCH_OF_TIME_API ATimeControllableActorBase : public ABlockBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimeControllableActorBase();

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Mesh)
		UStaticMeshComponent* m_CurrentMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Mesh)
		UStaticMeshComponent* m_PastMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Mesh)
		UStaticMeshComponent* m_FutureMesh;

	void ReturnTime();

	void JumpTime();

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	void UpdateMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentMesh = Current;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
