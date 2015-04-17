// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/GameMode.h"
#include "SideOpGameMode.generated.h"

// The GameMode defines the game being played. It governs the game rules, scoring, what actors
// are allowed to exist in this game type, and who may enter the game.
//
// This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of SideOpCharacter

UCLASS(minimalapi)
class ASideOpGameMode : public AGameMode
{
	GENERATED_BODY()

	UClass* GetDefaultPawnClassForController(AController* InController) override;


public:
	ASideOpGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* InController) override;

};
