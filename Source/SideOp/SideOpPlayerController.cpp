// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpPlayerController.h"
#include "SideOpGameMode.h"
#include "SideOpCharacter.h"


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
	CoinsCollected = 0;
	PlayerHealth = 1.0f;
	PlayerLives = 3;
	PlayerLevel = 1;
	CurrentXP = 0.0f;
	XPToLevel = 500.0f;
	XPPercent = 0.0f;
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
	CalculateXPPercent();
	
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
	DOREPLIFETIME(ASideOpPlayerController, PlayerHealth);
	DOREPLIFETIME(ASideOpPlayerController, CoinsCollected);
}

// What to do when we die. This currently just brings up our message, and does respawn logic
// Will most likely be split into multiple step functions
void ASideOpPlayerController::Die_Implementation()
{

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
}

// The RPC to set our lives on the server
void ASideOpPlayerController::ServerRPCSetLives_Implementation(int32 Lives)
{
	PlayerLives = Lives;
}

// Validation for the above  RPC
bool ASideOpPlayerController::ServerRPCSetLives_Validate(int32 Lives)
{
	return true;
}

// Functionality when adding a coin. This will mostly likely be morphed into adding xp
void ASideOpPlayerController::AddCoin()
{
	if (Role == ROLE_Authority)
	{
		CoinsCollected++;
	}
	else
	{
		ServerRPCAddCoin();
	}
	AddXP(50);
}

// Validation for the below RPC
bool ASideOpPlayerController::ServerRPCAddCoin_Validate()
{
	return true;
}

// Server RPC on what to do when adding a coin
void ASideOpPlayerController::ServerRPCAddCoin_Implementation()
{
		CoinsCollected++;
}

void ASideOpPlayerController::SubtractCoin()
{
	if (Role == ROLE_Authority)
	{
		CoinsCollected--;
	}
	else
	{
		ServerRPCSubtractCoin();
	}
}

bool ASideOpPlayerController::ServerRPCSubtractCoin_Validate()
{
	return true;
}

void ASideOpPlayerController::ServerRPCSubtractCoin_Implementation()
{
		CoinsCollected--;
}

void ASideOpPlayerController::AddXP(int32 Amount)
{
	CurrentXP = CurrentXP + Amount;
	CalculateXPPercent();
	if (Role != ROLE_Authority)
	{
		ServerRPCAddXP(Amount);
	}
	if (CheckForLevelUp())
	{
		LevelUp();
	}
	
}

void ASideOpPlayerController::ServerRPCAddXP_Implementation(int32 Amount)
{
	CurrentXP = CurrentXP + Amount;
	if (CheckForLevelUp())
	{
		LevelUp();
	}
}

bool ASideOpPlayerController::ServerRPCAddXP_Validate(int32 Amount)
{
	return true;
}

void ASideOpPlayerController::SubtractXP(int32 Amount)
{

	CurrentXP = CurrentXP - Amount;
	CalculateXPPercent();
	if (Role != ROLE_Authority)
	{
		ServerRPCSubtractXP(Amount);
	}
}
void ASideOpPlayerController::ServerRPCSubtractXP_Implementation(int32 Amount)
{
	CurrentXP = CurrentXP - Amount;
}

bool ASideOpPlayerController::ServerRPCSubtractXP_Validate(int32 Amount)
{
	return true;
}

bool ASideOpPlayerController::CheckForLevelUp()
{
	if (CurrentXP >= XPToLevel)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ASideOpPlayerController::LevelUp()
{
	PlayerLevel++;
	CurrentXP = 0;
	XPToLevel = 500 * PlayerLevel;
	CalculateXPPercent();
	if (Role != ROLE_Authority)
	{
		ServerRPCAddLevel();
	}
}

void ASideOpPlayerController::ServerRPCAddLevel_Implementation()
{
	PlayerLevel++;
	CurrentXP = 0;
	XPToLevel = 500 * PlayerLevel;
}

bool ASideOpPlayerController::ServerRPCAddLevel_Validate()
{
	return true;
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

void ASideOpPlayerController::CalculateXPPercent()
{
	XPPercent = CurrentXP / XPToLevel;
}