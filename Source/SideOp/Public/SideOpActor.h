// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SideOpActor.generated.h"

UCLASS()
class SIDEOP_API ASideOpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASideOpActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideOpGameplay")
	int32 XPtoGive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideOpGameplay")
	bool bDestroyable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideOpGameplay")
	int32 HitsToDestroy;
	
	
};
