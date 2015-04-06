// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SideOpPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API ASideOpPlayerController : public APlayerController
{
	GENERATED_BODY()
	


protected:

	virtual void SetupInputComponent() override;

public:

	
};
