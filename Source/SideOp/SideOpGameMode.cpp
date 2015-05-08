// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"
#include "SideOpPlayerState.h"
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
	SeedXPTable(XPSeed);
}

void ASideOpGameMode::PostLogin(APlayerController* InController)
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(InController);
	//ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(PC->PlayerState);
	// Set our player color based on this controllers player num
	switch (NumPlayers)
	{
	case 0:
		PC->SetPlayerColor(EPlayerColor::Blue);
		break;
	case 1:
		PC->SetPlayerColor(EPlayerColor::Beige);
		break;
	case 2:
		PC->SetPlayerColor(EPlayerColor::Green);
		break;
	case 3:
		PC->SetPlayerColor(EPlayerColor::Pink);
		break;
	case 4:
		PC->SetPlayerColor(EPlayerColor::Yellow);
		break;
	default:
		PC->SetPlayerColor(EPlayerColor::Blue);
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

void ASideOpGameMode::SeedXPTable(int32 Seed)
{
	for (int i = 0; i < MaxLevel; i++)
	{
		int32 XPAmount = 0;
		XPAmount = i * Seed;
		XPToLevel.Add(XPAmount);
	}
}

int32 ASideOpGameMode::GetXPToLevel(int32 CurrentLevel)
{
	int32 NextLevel = CurrentLevel + 1;
	return XPToLevel[NextLevel];
}