// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpAIController.h"
#include "SideOpEnemy.h"


// Sets default values
ASideOpEnemy::ASideOpEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup to use our custom AI controller
	AIControllerClass = ASideOpAIController::StaticClass();

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	XPToGive = 0;

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetSprite()->SetFlipbook(BaseAnimation);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

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

void ASideOpEnemy::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASideOpEnemy, bIsDying, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ASideOpEnemy, bIsHit, COND_SkipOwner);

}