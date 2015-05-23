// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Navigation/PathFollowingComponent.h"
#include "SideOpPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API USideOpPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
	
protected:
	/** cached UCharacterMovementComponent */
	UPROPERTY(transient)
		UCharacterMovementComponent* CharacterMoveComp;

public:
	USideOpPathFollowingComponent(const FObjectInitializer& ObjectInitializer);
	// used to detect properties of a path's segment a character is about to follow
	//virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	// used to cache UCharacterMovementComponent we're using in SetMoveSegment implementation
	//virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;
};
