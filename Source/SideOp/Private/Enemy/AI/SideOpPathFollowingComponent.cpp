// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"


USideOpPathFollowingComponent::USideOpPathFollowingComponent(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

}
/*
void USideOpPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	if (CharacterMoveComp != NULL)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

		if (FNavAreaHelper::HasJumpFlag(SegmentStart))
		{
			// TODO: Implement better jump logic
			CharacterMoveComp->SetMovementMode(MOVE_Flying);
		}
		else
		{
			// regular move
			CharacterMoveComp->SetMovementMode(MOVE_Walking);
		}
	}
}

void USideOpPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);

	CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
}*/