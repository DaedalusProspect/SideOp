// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"
#include "PaperFlipbookComponent.h"


//////////////////////////////////////////////////////////////////////////
// ASideOpCharacter
// This constructor first sets our custom movement component
// then sets up the rest of the character.

ASideOpCharacter::ASideOpCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USideOpCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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
	GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;

	// Setup the head collision box (Only players collide with this)
	//HeadBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadCollisionBox"));
	//HeadBoxComp->AttachTo(RootComponent);
	//HeadBoxComp->SetBoxExtent(FVector(32.0f, 32.0f, 55.0f));
	//HeadBoxComp->BodyInstance.SetCollisionProfileName("BlockPawnOnly");
	//HeadBoxComp->CanCharacterStepUpOn = ECB_Yes;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 45.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// The text for displaying our players name
	PlayerNameComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerName"));
	PlayerNameComponent->AttachTo(RootComponent);
	PlayerNameComponent->SetText(FString("Player Name"));
	PlayerNameComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	PlayerNameComponent->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	PlayerNameComponent->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
	PlayerNameComponent->RelativeLocation = FVector(0.0f, 0.0f, 80.0f);
	PlayerNameComponent->XScale = 1.5f;
	PlayerNameComponent->YScale = 1.5f;
	PlayerNameComponent->SetTextRenderColor(FColor::White);
	PlayerNameComponent->SetIsReplicated(true);
	PlayerNameComponent->bOwnerNoSee = true;

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 1860.0f;
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
	bIsSprinting = false;
	SprintingModifier = 1.5f;
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
	GetCharacterMovement()->GravityScale = PlayerGravityScale;
	GetCharacterMovement()->AirControl = PlayerAirControl;
	GetCharacterMovement()->JumpZVelocity = PlayerJumpZVelocity;
	GetCharacterMovement()->GroundFriction = PlayerGroundFriction;
	GetCharacterMovement()->MaxWalkSpeed = PlayerMaxWalkSpeed;
	GetCharacterMovement()->MaxFlySpeed = PlayerMaxFlySpeed;
	GetCharacterMovement()->Buoyancy = PlayerBouyancy;

	PlayerNameComponent->SetTextRenderColor(FColor::White);

}

//////////////////////////////////////////////////////////////////////////
// Animation

void ASideOpCharacter::UpdateAnimation()
{
	// Some Data Holders
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeed = PlayerVelocity.Size();


	if (bIsHit || bIsDying) // Check if we were damaged or dying
	{
		CurrentAnimation = HitAnimation;
	}
	else if (GetCharacterMovement()->IsInWater())// Check if we're swimming
	{
		CurrentAnimation = SwimAnimation;
	}
	else if (bIsCrouched)// Check if were crouching
	{
		CurrentAnimation = DuckAnimation;
	}
	else if (PlayerVelocity.Z != 0 && !(GetCharacterMovement()->IsInWater())) // Check if were falling
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

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ASideOpCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ASideOpCharacter::StopSprint);
	InputComponent->BindAxis("MoveRight", this, &ASideOpCharacter::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ASideOpCharacter::MoveUp);

	InputComponent->BindTouch(IE_Pressed, this, &ASideOpCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ASideOpCharacter::TouchStopped);
} 

void ASideOpCharacter::MoveRight(float Value)
{
	// Update animation to match the motion
	UpdateAnimation();

	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (Value < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			PlayerNameComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		}
		else if (Value > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			PlayerNameComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		}
	}
	// Make sure we can move
	if (bCanMove)
	{
		// Apply the input to the character motion
		// But apply more if we want to sprint
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);

	}
}

void ASideOpCharacter::MoveUp(float Value)
{
	// Check if were swimming
	if (GetCharacterMovement()->IsInWater())
	{
		AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Value);
	}
}

// This is an override so we can add functionality to our jump
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

void ASideOpCharacter::OnJump()
{

		// Then if we can jump (Cant while crouching)
		if (bCanJump)
		{
			Jump(); // And finally jump
		}
}

void ASideOpCharacter::StopJump()
{

		StopJumping();
}

void ASideOpCharacter::Crouching()
{
	// See if were swimming first
	if (GetCharacterMovement()->IsInWater())
	{
		// add code here to make us move down
	}
	else
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			// Not falling, not in water, so crouch
			Crouch();
			UpdateAnimation();
			bCanMove = false;
			bCanJump = false;
		}	
	}

}

void ASideOpCharacter::StopCrouching()
{
	UnCrouch();
	bCanMove = true;
	bCanJump = true;
}

void ASideOpCharacter::Sprint()
{
	SetSprint(true);
}

void ASideOpCharacter::StopSprint()
{
	SetSprint(false);
}

void ASideOpCharacter::SetSprint(bool Sprint)
{
	bIsSprinting = Sprint;

	if (Role < ROLE_Authority)
	{
		ServerSetSprint(Sprint);
	}
	GEngine->AddOnScreenDebugMessage(8, 1.0f, FColor::Magenta, TEXT("Called Sprint"));

}

void ASideOpCharacter::ServerSetSprint_Implementation(bool Sprint)
{
	SetSprint(Sprint);
}

bool ASideOpCharacter::ServerSetSprint_Validate(bool Sprint)
{
	return true;
}

void ASideOpCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASideOpCharacter, bIsSprinting, COND_SkipOwner);
}

void ASideOpCharacter::AddCoin()
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(GetController());
	if (PC)
	{
		PC->AddCoin();
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Blue, TEXT("From Character"));
	}
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

////////////////////////////////////////////////

void ASideOpCharacter::PossessedBy(class AController* InController)
{
	Super::PossessedBy(InController);

}


