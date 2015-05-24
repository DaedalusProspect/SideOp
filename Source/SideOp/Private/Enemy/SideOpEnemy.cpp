// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"


// Sets default values
ASideOpEnemy::ASideOpEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup to use our custom AI controller
	AIControllerClass = ASideOpAIController::StaticClass();

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetSprite()->SetFlipbook(IdleAnimation);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Set our first animation
	CurrentAnimation = IdleAnimation;

}

// Called when the game starts or when spawned
void ASideOpEnemy::BeginPlay()
{
	Super::BeginPlay();
	UpdateAnimation();
	
}

// Called every frame
void ASideOpEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UpdateAnimation();
}

// Called to bind functionality to input
void ASideOpEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void ASideOpEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASideOpEnemy, bIsDying, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ASideOpEnemy, bIsHit, COND_SkipOwner);

}

void ASideOpEnemy::UpdateAnimation()
{
	const FVector CurrentVelocity = GetVelocity();
	const float BotSpeed = CurrentVelocity.Size();

	if (bIsHit || bIsDying)
	{
		CurrentAnimation = HitAnimation;
	}
	else if (CurrentVelocity.Z != 0)
	{
		CurrentAnimation = JumpAnimation;
	}
	else if (BotSpeed > 0)
	{
		CurrentAnimation = MoveAnimation;
	}
	else
	{
		CurrentAnimation = IdleAnimation;
	}

	GetSprite()->SetFlipbook(CurrentAnimation);

}

void ASideOpEnemy::ServerSetEnemyHit_Implementation(bool Hit)
{
	bIsHit = Hit;
}

bool ASideOpEnemy::ServerSetEnemyHit_Validate(bool Hit)
{
	return true;
}