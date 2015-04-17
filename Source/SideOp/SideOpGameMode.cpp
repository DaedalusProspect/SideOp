// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"
#include "SideOpGameMode.h"
#include "SideOpCharacter.h"
#include "SideOpPlayerController.h"

ASideOpGameMode::ASideOpGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerDefaultPawnObject(TEXT("Pawn'/Game/Blueprints/BP_BlankCharacter.BP_BlankCharacter_C'"));
	// set default pawn class to our default character
	if (PlayerDefaultPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerDefaultPawnObject.Class;

	}

	// Dont have our players spawn automatically, do it manually
	bStartPlayersAsSpectators = true;
	
}

void ASideOpGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASideOpGameMode::PostLogin(APlayerController* InController)
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(InController);
	// Set our player color based on this controllers player num
	switch (NumPlayers)
	{
	case 0:
		PC->PlayerColor = EPlayerColor::Blue;
		break;
	case 1:
		PC->PlayerColor = EPlayerColor::Beige;
		break;
	case 2:
		PC->PlayerColor = EPlayerColor::Green;
		break;
	case 3:
		PC->PlayerColor = EPlayerColor::Pink;
		break;
	case 4:
		PC->PlayerColor = EPlayerColor::Yellow;
		break;
	default:
		PC->PlayerColor = EPlayerColor::Blue;
		break;
	}
	Super::PostLogin(InController);
}


UClass* ASideOpGameMode::GetDefaultPawnClassForController(AController* InController)
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(InController);
	if (PC)
	{
		// return our pawn class from the controller
		return PC->GetPlayerPawnClass();
	}

	// return the default if we dont get our own
	return DefaultPawnClass;
}