// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/Player/SideOpPlayerController.h"
#include "GameFramework/GameState.h"
#include "SideOpGameState.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API ASideOpGameState : public AGameState
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable, Category=SideOpHUDData)
	FVector GetPlayerPos(EPlayerColor::Color PlayerColor);
	
};
