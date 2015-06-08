// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"


// Sets default values
ASideOpFinishZone::ASideOpFinishZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);

}

// Called when the game starts or when spawned
void ASideOpFinishZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASideOpFinishZone::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

