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
	DeterminePawnClass();
	
}

// This determines what class our player gets to use based on the player color set in ASideOpGameMode
void ASideOpPlayerController::DeterminePawnClass_Implementation()
{
	if (IsLocalController()) // Only do this on the local controller
	{
		// Now check if we already have a color, and if we do, set our player pawn
		if (PlayerColor == EPlayerColor::Unset)
		{
			// Our player color will be determined in ASideOpGameMode::PostLogin()
		}
		else
		{
			switch (PlayerColor)
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
		return;
	}
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

// What to do when we die. This currently just brings up our message, and does respawn logic
// Will most likely be split into multiple step functions
void ASideOpPlayerController::Die_Implementation()
{
	//##########################################
	// This should be done in the player/gamestate
	//##########################################
		if (PlayerLives > 0) // This is very simple right now. Just checks if we have lives and if we do, respawns us at start
		{
			PlayerLives--;

			MessageText = TEXT("You have died!");
			GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASideOpPlayerController::ClearMessage, 2.0f);
			
			return;
		}
		else
		{
			// Game Over
			MessageText = TEXT("GameOver");
			return;
		}
	//#############################################
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

void ASideOpPlayerController::ClearMessage()
{
	MessageText = TEXT("");
	// For testing purposes, just clear the timer
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);

	ASideOpCharacter* OurChar = Cast<ASideOpCharacter>(GetCharacter());
	if (OurChar)
	{
		OurChar->Destroy();
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