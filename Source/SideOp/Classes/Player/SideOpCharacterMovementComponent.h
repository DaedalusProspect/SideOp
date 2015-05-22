// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "SideOpCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API USideOpCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


		virtual float GetMaxSpeed() const override;
	
};
