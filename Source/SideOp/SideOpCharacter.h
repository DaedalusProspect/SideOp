// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperCharacter.h"
#include "SideOpPlayerController.h"
#include "SideOpCharacter.generated.h"

// This class is the default character for SideOp, and it is responsible for all
// physical interaction between the player and the world.
//
//   The capsule component (inherited from ACharacter) handles collision with the world
//   The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
//   The Sprite component (inherited from APaperCharacter) handles the visuals

UCLASS(config=Game)
class ASideOpCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// allow updates every tick
	virtual void Tick(float DeltaSeconds) override;

	// Functionality for when the character is spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	class UTextRenderComponent* PlayerNameComponent;

	// The animation to play while running around  - Blue Character / Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	// The animation to play while ducking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* DuckAnimation;

	// The animation to play while swimming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* SwimAnimation;

	// The animation to play when the character is hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* HitAnimation;

	// The animation to play while climbing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* ClimbAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* FrontAnimation;

	///////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterClass)
	float SprintingModifier;

	////////////////////////////////////////////////////////////////////////////////

	// Was the character hit?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsHit;

	// Are we sprinting
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsSprinting;

	// Is the character dying?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsDying;

	// Can we move?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bCanMove;

	// Can we jump?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bCanJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keys)
	bool bHasBlueKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keys)
	bool bHasRedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keys)
	bool bHasGreenKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Keys)
	bool bHasYellowKey;

	//////////////////////////////////////////////////////////////////////////

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	// Called when the character is possessed
	virtual void PossessedBy(class AController* InController) override;

	//////////////////////////////////////////////////////////////////////////

	/** Called for side to side input */
	void MoveRight(float Value);

	// Called when swimming up or down, or climbing up/down
	void MoveUp(float Value);

	// Called when crouching
	void Crouching();

	// Stop crouching
	void StopCrouching();

	// Handle Jumping so we can add extra utilities.
	void OnJump();

	void StopJump();

	// Overrides Jump so we can have variable jump height
	void Jump() override;

	// Start our sprint logic
	void Sprint();
	void StopSprint();

	void SetSprint(bool Sprint);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetSprint(bool Sprint);
	void ServerSetSprint_Implementation(bool Sprint);
	bool ServerSetSprint_Validate(bool Sprint);

	/////////////////////////////////////////////////////////////////////////////

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	/////////////////////////////////////////////////////////////////////////////

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	//////////////////////////////////////////////////////////////////////////////

	// Current Animation Frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Play")
	class UPaperFlipbook* CurrentAnimation;

	//////////////////////////////////////////////////////////////////////////////
	// Networking functions
	/////////////////////////////////////////////////////////////////////////////
public:
	ASideOpCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }

	// Get our PlayerName component for rendering the player name above their head
	FORCEINLINE class UTextRenderComponent* GetPlayerNameComponent() const { return PlayerNameComponent; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE float GetSprintingModifier() const { return SprintingModifier; }

	FORCEINLINE bool IsSprinting() const { return bIsSprinting; }

	FORCEINLINE bool IsDead() const { return bIsDying; }

	FORCEINLINE void SetMovement(bool CanMove){ bCanMove = CanMove; }

	////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsPossessed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysics)
	float PlayerGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysics)
	float PlayerAirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysics)
	float PlayerJumpZVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysics)
	float PlayerGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysics)
	float PlayerMaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysics)
	float PlayerMaxFlySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerPhysics)
	float PlayerBouyancy;
	//////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// Called to add a coin to our characters score
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void AddCoin();

	UFUNCTION(BlueprintCallable, Category = Character)
	void OnDeath();



};
