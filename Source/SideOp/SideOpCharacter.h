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

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UBoxComponent* HeadBoxComp;

	// allow updates every tick
	virtual void Tick(float DeltaSeconds) override;

	// Functionality for when the character is spawned
	virtual void BeginPlay() override;

protected:
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

	///////////////////////////////////////////////////////////////////////////

	// The modifier for this class's run speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterClass)
	float RunSpeedModifier;

	// The modifier for this class's gravity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterClass)
	float GravityModifier;

	////////////////////////////////////////////////////////////////////////////////

	// Is the character swimming?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsSwimming;

	// Is the character falling?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsFalling;

	// Is the character running?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsRunning;

	// Is the character climbing?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsClimbing;

	// Was the character hit?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsHit;

	// Is the character dying?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsDying;

	////////////////////////////////////////////////////////////////////////
	// Is this just a blank character?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsBlank;

	// Can we move?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bCanMove;

	// Can we jump?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bCanJump;

	//////////////////////////////////////////////////////////////////////////

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	/** Called for side to side input */
	UFUNCTION(BlueprintNativeEvent)
	void MoveRight(float Value);
	void MoveRight_Implementation(float Value);

	// Called when swimming up or down, or climbing up/down
	UFUNCTION(BlueprintNativeEvent)
	void MoveUp(float Value);
	void MoveUp_Implementation(float Value);

	// Called when crouching
	UFUNCTION(BlueprintNativeEvent)
	void Crouching();
	void Crouching_Implementation();

	// Stop crouching
	UFUNCTION(BlueprintNativeEvent)
	void StopCrouching();
	void StopCrouching_Implementation();


	// Handle Jumping so we can add extra utilities.
	UFUNCTION(BlueprintNativeEvent)
	void OnJump();
	void OnJump_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void StopJump();
	void StopJump_Implementation();

	// Overrides Jump so we can have variable jump height
	void Jump() override;

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
	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPCUpdateCoins(int32 Coins);
	//virtual bool ServerRPCUpdateCoins_Validate(int32 Coins);
	//virtual void ServerRPCUpdateCoins_Implementation(int32 Coins);

	/////////////////////////////////////////////////////////////////////////////
public:
	ASideOpCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//FORCEINLINE int32 GetCoinsCollected(){ return CoinsCollected; }
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

	// What to do when the character is dying
	//UFUNCTION(BlueprintCallable, Category = Character)
	//void Dying();

	UFUNCTION(BlueprintCallable, Category = Character)
	void OnDeath();



};
