// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"
#include "SideOpCharacter.h"
#include "SideOpPlayerController.h"
#include "PaperFlipbookComponent.h"

//////////////////////////////////////////////////////////////////////////
// ASideOpCharacter


ASideOpCharacter::ASideOpCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	GetSprite()->SetFlipbook(IdleAnimation); // Sets default idle animation
	CurrentAnimation = IdleAnimation;

	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(56.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Let us be able to crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Set our bool so the controllers know its not possessed
	bIsPossessed = false;
	
	// Other
	bCanMove = true;
	bCanJump = true;
	bIsDying = false;
}

void ASideOpCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateAnimation(); // Called to make sure animation is updated over the network

}

void ASideOpCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Configure character movement and physics based on the characters play class
	// Normally, this is done in the constructor. However, we need special functionality
	// To allow for physics changes on a per blutprint basis for the different player colors
	GetCharacterMovement()->GravityScale = PlayerGravityScale * GravityModifier;
	GetCharacterMovement()->AirControl = PlayerAirControl;
	GetCharacterMovement()->JumpZVelocity = PlayerJumpZVelocity;
	GetCharacterMovement()->GroundFriction = PlayerGroundFriction;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMaxWalkSpeed * RunSpeedModifier;
	GetCharacterMovement()->MaxFlySpeed = PlayerMaxFlySpeed;
	GetCharacterMovement()->Buoyancy = PlayerBouyancy;
	
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASideOpCharacter::UpdateAnimation()
{
	// Some Data Holders
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();
	//const float HalfHeight = GetCapsuleComponent()->

	if (bIsHit || bIsDying) // Check if we were damaged or dying
	{
		CurrentAnimation = HitAnimation;
	}
	else if (GetCharacterMovement()->IsInWater())// Check if we're swimming
	{
		CurrentAnimation = SwimAnimation;
	}
	else if (bIsCrouching)// Check if were crouching
	{
		CurrentAnimation = DuckAnimation;
	}
	else if (GetVelocity().Z != 0 && !(GetCharacterMovement()->IsInWater())) // Check if were falling
	{
		CurrentAnimation = JumpAnimation;
	}
	else if (PlayerSpeed > 0 && !(GetVelocity().Z != 0) && !(GetCharacterMovement()->IsInWater())) // Check if were running
	{
		CurrentAnimation = RunningAnimation;
	}
	else // Otherwise, just set ourselves to idle
	{
		CurrentAnimation = IdleAnimation;
	}

	GetSprite()->SetFlipbook(CurrentAnimation);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASideOpCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASideOpCharacter::OnJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ASideOpCharacter::StopJump);
	InputComponent->BindAction("Duck", IE_Pressed, this, &ASideOpCharacter::Crouching);
	InputComponent->BindAction("Duck", IE_Released, this, &ASideOpCharacter::StopCrouching);
	InputComponent->BindAxis("MoveRight", this, &ASideOpCharacter::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ASideOpCharacter::MoveUp);

	InputComponent->BindTouch(IE_Pressed, this, &ASideOpCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ASideOpCharacter::TouchStopped);
} 

void ASideOpCharacter::MoveRight_Implementation(float Value)
{
	// Update animation to match the motion
	UpdateAnimation();

	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (Value < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (Value > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
	// Make sure we can move
	if (bCanMove && !bIsCrouching)
	{
		// Apply the input to the character motion
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	}
}

void ASideOpCharacter::MoveUp_Implementation(float Value)
{
	// Check if were swimming
	if (GetCharacterMovement()->IsInWater())
	{
		AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Value);
	}
}

void ASideOpCharacter::Jump()
{
	if (CanJump())
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void ASideOpCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{

		// Then if we can jump (Cant while crouching)
		if (bCanJump)
		{
			Jump(); // And finally jump
		}

}

void ASideOpCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{

		StopJumping();

}

void ASideOpCharacter::OnJump_Implementation()
{

		// Then if we can jump (Cant while crouching)
		if (bCanJump)
		{
			Jump(); // And finally jump
		}
}

void ASideOpCharacter::StopJump_Implementation()
{

		StopJumping();
}

void ASideOpCharacter::Crouching_Implementation()
{
	// See if were swimming first
	if (GetCharacterMovement()->IsInWater())
	{
	}
	else
	{
		if (!bIsCrouching)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(48.0f);
			bCanMove = false;
			bCanJump = false;
			ServerRPCSetCrouch(); // Set the var on client and server so it replicates
			UpdateAnimation();
		}
	}

}

void ASideOpCharacter::StopCrouching_Implementation()
{
	// See if were swimming first
	if (GetCharacterMovement()->IsInWater())
	{
		bIsCrouching = false;
		bCanMove = true;
		bCanJump = true;
	}	
	else
	{
		if (bIsCrouching)
		{
			GetCapsuleComponent()->SetCapsuleHalfHeight(56.0f);
			UpdateAnimation();
			bCanMove = true;
			bCanJump = true;
			ServerRPCSetCrouch();
		}
	}
}


void ASideOpCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASideOpCharacter, bIsCrouching);
}

bool ASideOpCharacter::ServerRPCSetCrouch_Validate()
{
	return true;
}

void ASideOpCharacter::ServerRPCSetCrouch_Implementation()
{
	if (!GetCharacterMovement()->IsInWater())
	{
		if (bIsCrouching == false)
		{
			bIsCrouching = true;
		}
		else
		{
			bIsCrouching = false;
		}
	}
	else
	{
		bIsCrouching = false;
	}
}

void ASideOpCharacter::AddCoin()
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(GetController());
	if (PC)
	{
		PC->AddCoin();
	}
}

void ASideOpCharacter::SubtractCoin()
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(GetController());
	if (PC)
	{
		PC->SubtractCoin();
	}
}

bool ASideOpCharacter::ServerRPCUpdateCoins_Validate(int32 Coins)
{
	return true;
}

void ASideOpCharacter::ServerRPCUpdateCoins_Implementation(int32 Coins)
{
	CoinsCollected = Coins;
}

void ASideOpCharacter::OnDeath()
{
	// For now, just call die on the owning controller and set ourselves to dying
	bIsDying = true;
	UpdateAnimation();
	DisableInput(Cast<ASideOpPlayerController>(GetController()));
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(GetController());
	if (PC)
	{
		PC->Die();
	}
}