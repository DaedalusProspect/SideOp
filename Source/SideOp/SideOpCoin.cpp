// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpCoin.h"


// Sets default values
ASideOpCoin::ASideOpCoin(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup our Collision Capsule
	UCapsuleComponent* CollisionComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CollisionCapsule"));
	CollisionComp->InitCapsuleSize(28.0f, 28.0f);
	RootComponent = CollisionComp;

}

// Called when the game starts or when spawned
void ASideOpCoin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASideOpCoin::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


