// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"



float USideOpCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const ASideOpCharacter* CharOwner = Cast<ASideOpCharacter>(PawnOwner);
	if (CharOwner)
	{
		if (CharOwner->IsSprinting())
		{
			MaxSpeed *= CharOwner->GetSprintingModifier();
		}
	}
	return MaxSpeed;
}
