// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SideOpStartZone.generated.h"

UCLASS()
class SIDEOP_API ASideOpStartZone : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SideOpZone")
	class UBoxComponent* CollisionComponent;
	
public:	
	// Sets default values for this actor's properties
	ASideOpStartZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
