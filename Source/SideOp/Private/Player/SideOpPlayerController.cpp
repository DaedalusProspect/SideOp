// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"



ASideOpPlayerController::ASideOpPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set our color for this controller to default, or unset.
	PlayerColor = EPlayerColor::Unset;

	// Set our pawn to NULL to initialize
	PlayerPawn = NULL;

	// Set to replicate
	bReplicates = true;

	// Set our defaults
	PlayerLives = 3;
	MessageText = TEXT("");

}

// This sets up our input component
// Current just uses default
void ASideOpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


}

// What to do when the player controller is first spawned
void ASideOpPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//DeterminePawnClass();
	
}

void ASideOpPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsLocalController())
	{
		FString Color;
		ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(PlayerState);
		if (PS)
		{
			switch (PS->GetPlayerColor())
			{
			case EPlayerColor::Blue:
				Color = TEXT("Blue Pawn");
				break;
			case EPlayerColor::Beige:
				Color = TEXT("Beige Pawn");
				break;
			case EPlayerColor::Green:
				Color = TEXT("Green Pawn");
				break;
			case EPlayerColor::Pink:
				Color = TEXT("Pink Pawn");
				break;
			case EPlayerColor::Yellow:
				Color = TEXT("Yellow Pawn");
				break;
			default:
				Color = TEXT("Unset");
				break;
			}
		}


		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::White, Color);
	}
}


// This determines what class our player gets to use based on the player color set in ASideOpGameMode
void ASideOpPlayerController::DeterminePawnClass_Implementation()
{
		ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(PlayerState);
		if (PS)
		{
			// Now check if we already have a color, and if we do, set our player pawn
			if (PS->GetPlayerColor() == EPlayerColor::Unset)
			{
				// Our player color will be determined in ASideOpGameMode::PostLogin()
			}
			else
			{
				switch (PS->GetPlayerColor())
				{
				case EPlayerColor::Blue:
					PlayerPawn = BluePlayer;
					break;
				case EPlayerColor::Beige:
					PlayerPawn = BeigePlayer;
					break;
				case EPlayerColor::Green:
					PlayerPawn = GreenPlayer;
					break;
				case EPlayerColor::Pink:
					PlayerPawn = PinkPlayer;
					break;
				case EPlayerColor::Yellow:
					PlayerPawn = YellowPlayer;
					break;
				default:
					PlayerPawn = BluePlayer;
					break;
				}
			}
			ServerRPCSetPawn(PlayerPawn);
		}
		return;
}

// Server validation to set our pawn
bool ASideOpPlayerController::ServerRPCSetPawn_Validate(TSubclassOf<APawn> InPawnClass)
{
	return true;
}

// This sets our player color on the server copy of the PC
void ASideOpPlayerController::ServerRPCSetPawn_Implementation(TSubclassOf<APawn> InPawnClass)
{
	PlayerPawn = InPawnClass;

	// Now restart our player if we didnt get our class in time
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}


void ASideOpPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASideOpPlayerController, PlayerPawn);
	DOREPLIFETIME(ASideOpPlayerController, PlayerColor);
	DOREPLIFETIME(ASideOpPlayerController, PlayerLives);
}

UClass* ASideOpPlayerController::GetPlayerPawnClass(TEnumAsByte<EPlayerColor::Color> Color)
{
	switch (Color)
	{
	case EPlayerColor::Blue:
		return BluePlayer;

	case EPlayerColor::Beige:
		return BeigePlayer;

	case EPlayerColor::Green:
		return GreenPlayer;

	case EPlayerColor::Pink:
		return PinkPlayer;

	case EPlayerColor::Yellow:
		return YellowPlayer;

	default:
		return BluePlayer;
	}

}

// What to do when we die. This currently just brings up our message, and does respawn logic
// Will most likely be split into multiple step functions
void ASideOpPlayerController::Die_Implementation()
{
	ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(PlayerState);
	if (PS)
	{
		if (PS->GetPlayerLives() > 0) // This is very simple right now. Just checks if we have lives and if we do, respawns us at start
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASideOpPlayerController::DeathTimer, 2.0f);
			return;
		}
		else
		{
			// Game Over
			return;
		}
	}
}

// Functionality when adding a coin. This will mostly likely be morphed into adding xp
void ASideOpPlayerController::AddCoin()
{
	ASideOpPlayerState* SideState = Cast<ASideOpPlayerState>(PlayerState);
	if (SideState)
	{
		SideState->AddXP(50);
		GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Blue, TEXT("From Controller"));
	}
}

void ASideOpPlayerController::DeathTimer()
{
	//MessageText = TEXT("");
	// For testing purposes, just clear the timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);

	ASideOpCharacter* OurChar = Cast<ASideOpCharacter>(GetCharacter());
	if (OurChar)
	{
		OurChar->Destroy();
	}

	ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(PlayerState);
	if (PS)
	{
		PS->OnDeath();
	}

	ServerRestartPlayer();

}

void ASideOpPlayerController::EnableMovement()
{
	ASideOpCharacter* OurChar = Cast<ASideOpCharacter>(GetCharacter());
	if (OurChar)
	{
		OurChar->EnableInput(this);
	}
}

void ASideOpPlayerController::DisableMovement()
{
	ASideOpCharacter* OurChar = Cast<ASideOpCharacter>(GetCharacter());
	if (OurChar)
	{
		OurChar->DisableInput(this);
	}
}

TEnumAsByte<EPlayerColor::Color> ASideOpPlayerController::GetPlayerColor()
{
	ASideOpPlayerState* PS = Cast<ASideOpPlayerState>(PlayerState);
	if (PS)
	{
		return PS->GetPlayerColor();
	}
	return EPlayerColor::Unset;
}