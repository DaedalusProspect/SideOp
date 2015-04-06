// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpEnemy.h"


// Sets default values
ASideOpEnemy::ASideOpEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASideOpEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASideOpEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASideOpEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

