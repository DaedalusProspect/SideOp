// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperCharacter.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// allow updates every tick
	virtual void Tick(float DeltaSeconds) override;

	// Functionality for when the character is spawned
	virtual void BeginPlay() override;

protected:
	// The animation to play while running around  - Blue Character / Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)  - Blue Character / Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while jumping - Blue Character / Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	// The animation to play while ducking  - Blue Character / Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* DuckAnimation;

	// The animation to play while swimming - Blue Character / Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* SwimAnimation;

	///////////////////////////////////////////////////////////////////////////

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

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

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	// Current Animation Frame
	class UPaperFlipbook* CurrentAnimation;





public:
	ASideOpCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Networking functions
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSetCrouch();
	bool ServerRPCSetCrouch_Validate();
	void ServerRPCSetCrouch_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCSetCrouch();
	void MulticastRPCSetCrouch_Implementation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	int32 PlayerLives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsSwimming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool bIsRunning;

	bool bCanMove;
	bool bCanJump;

};
