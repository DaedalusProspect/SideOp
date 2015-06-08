// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"


// Sets default values
ASideOpStartZone::ASideOpStartZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

}

// Called when the game starts or when spawned
void ASideOpStartZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASideOpStartZone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

