// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SideOpCharacter.h"
#include "GameFramework/Actor.h"
#include "SideOpCoin.generated.h"


UCLASS()
class SIDEOP_API ASideOpCoin : public AActor
{
	GENERATED_BODY()


	// To see who picked this up
	class ASideOpCharacter* OtherActor;



	
public:	
	// Sets default values for this actor's properties
	ASideOpCoin(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Our Collision comp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	class UCapsuleComponent* CollisionComp;
	


};
