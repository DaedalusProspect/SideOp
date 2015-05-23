// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpActor.h"


// Sets default values
ASideOpActor::ASideOpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASideOpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASideOpActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

