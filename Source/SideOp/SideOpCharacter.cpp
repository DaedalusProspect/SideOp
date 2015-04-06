// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"
#include "SideOpCharacter.h"
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

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 1.8f;
	GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	GetCharacterMovement()->MaxFlySpeed = 900.0f;
	GetCharacterMovement()->Buoyancy = 1.4f;

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
	
	// Other
	bCanMove = true;
	bCanJump = true;

	// Set Health and Lives
	PlayerHealth = 0.6f;
	PlayerLives = 3;
}

void ASideOpCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateAnimation(); // Called to make sure animation is updated over the network

}

void ASideOpCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASideOpCharacter::UpdateAnimation()
{
	// Some Data Holders
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();
	//const float HalfHeight = GetCapsuleComponent()->

	// Check if we're swimming
	if (GetCharacterMovement()->IsInWater())
	{
		CurrentAnimation = SwimAnimation;
	}
	else
	{
		// Check if were jumping or falling or crouching
		if (bIsCrouching)
		{
			CurrentAnimation = DuckAnimation;
		}
		else if (GetVelocity().Z != 0 && !(GetCharacterMovement()->IsInWater()))
		{
			CurrentAnimation = JumpAnimation;
		}
		else
		{
			if (PlayerSpeed > 0 && !(GetVelocity().Z != 0) && !(GetCharacterMovement()->IsInWater()))
			{
				CurrentAnimation = RunningAnimation;
			}
			else
			{
				CurrentAnimation = IdleAnimation;
			}
		}
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
	if (!bIsCrouching)
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(48.0f);
		bCanMove = false;
		bCanJump = false;
	}
	ServerRPCSetCrouch(); // Set the var on client and server so it replicates
	UpdateAnimation();
}

void ASideOpCharacter::StopCrouching_Implementation()
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
	if (bIsCrouching == false)
	{
		bIsCrouching = true;
	}
	else
	{
		bIsCrouching = false;
	}
}

void ASideOpCharacter::MulticastRPCSetCrouch_Implementation()
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
