// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"

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
	bStartPlayersAsSpectators = false;

	
	
}

void ASideOpGameMode::BeginPlay()
{
	Super::BeginPlay();
	SeedXPTable(XPSeed);
}

void ASideOpGameMode::PostLogin(APlayerController* InController)
{
	ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(InController->PlayerState);
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(InController);
	// Set our player color based on this controllers player num
	if (PS)
	{
		if (PC)
		{
			switch (NumPlayers)
			{
			case 0:
				PS->SetPlayerColor(EPlayerColor::Blue);
				break;
			case 1:
				PS->SetPlayerColor(EPlayerColor::Beige);
				break;
			case 2:
				PS->SetPlayerColor(EPlayerColor::Green);
				break;
			case 3:
				PS->SetPlayerColor(EPlayerColor::Pink);
				break;
			case 4:
				PS->SetPlayerColor(EPlayerColor::Yellow);
				break;
			default:
				PS->SetPlayerColor(EPlayerColor::Blue);
				break;
			}
		}
	}
	Super::PostLogin(InController);
}


UClass* ASideOpGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(InController);
	ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(InController->PlayerState);
	if (PC)
	{
		return PC->GetPlayerPawnClass(PS->GetPlayerColor());
	}

	// return the default if we dont get our own
	return DefaultPawnClass;
}


AActor* ASideOpGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(Player);

	if (!PC)
		return Super::ChoosePlayerStart_Implementation(Player);

	ASideOpPlayerState* PState = Cast<ASideOpPlayerState>(PC->PlayerState);

	if (PState)
	{
		// Iterate through the starts and find the one with the tag that matches the player
		for (TActorIterator<ASideOpPlayerStart> It(GetWorld()); It; ++It)
		{
			ASideOpPlayerStart* PS = *It;
			if (PS)
			{
				if (PState->GetPlayerColor() == PS->GetStartColor())
				{
					return PS;
				}
			}
		}
	}
	else
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}
	 return Super::ChoosePlayerStart_Implementation(Player);
} 

bool ASideOpGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return true;
}

class AActor* ASideOpGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	AActor* BestStart = ChoosePlayerStart(Player);
	return BestStart;
	/* // THIS IS CURRENTLY BROKEN
	// See if we even got a player at all
	if (Player)
	{
		// If there is already a start name being passed in, just run normal code
		if (!IncomingName.IsEmpty())
		{
			return Super::FindPlayerStart_Implementation(Player, IncomingName);
		}
		else
		{
			// First cast our PC to our PC to get its player color
			ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(Player);
			if (PC)
			{
				FString Color = TEXT("NONE");

				switch (PC->GetPlayerColor())
				{
				case EPlayerColor::Blue:
					Color = TEXT("BLUE");
					break;
				case EPlayerColor::Beige:
					Color = TEXT("BEIGE");
					break;
				case EPlayerColor::Green:
					Color = TEXT("GREEN");
					break;
				case EPlayerColor::Pink:
					Color = TEXT("PINK");
					break;
				case EPlayerColor::Yellow:
					Color = TEXT("YELLOW");
					break;
				default:
					Color = TEXT("NONE");
					break;
				}

				// If we didnt get a color run default
				if (Color == TEXT("NONE"))
				{
					return Super::FindPlayerStart_Implementation(Player, IncomingName);
				}
				else
				{
					// We got everything we need, so run with a color for the spawn
					return Super::FindPlayerStart_Implementation(Player, Color);
				}
			}
		}
	}
	else
	{
		return Super::FindPlayerStart_Implementation(Player, IncomingName);
	}

	// We got nothing, so just do the default
	return Super::FindPlayerStart_Implementation(Player, IncomingName);
	*/
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

void ASideOpGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}