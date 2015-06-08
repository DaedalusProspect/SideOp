// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SideOpFinishZone.generated.h"

UCLASS()
class SIDEOP_API ASideOpFinishZone : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SideOpZone")
	class UBoxComponent* CollisionComponent;
	
public:	
	// Sets default values for this actor's properties
	ASideOpFinishZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
