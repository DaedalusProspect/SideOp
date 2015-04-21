// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpPlayerController.h"
#include "SideOpGameMode.h"


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
}
void ASideOpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


}


void ASideOpPlayerController::BeginPlay()
{
	Super::BeginPlay();
	DeterminePawnClass();
}

void ASideOpPlayerController::DeterminePawnClass_Implementation()
{
	if (IsLocalController()) // Only do this on the local controller
	{
		// Now check if we already have a color, and if we do, set our player pawn
		if (PlayerColor == EPlayerColor::Unset)
		{
			// Our player color will be determined in Post Login
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

bool ASideOpPlayerController::ServerRPCSetPawn_Validate(TSubclassOf<APawn> InPawnClass)
{
	return true;
}

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


void ASideOpPlayerController::Die()
{

		if (PlayerLives > 0) // This is very simple right now. Just checks if we have lives and if we do, respawns us at start
		{
			PlayerLives--;

			GetWorld()->GetAuthGameMode()->RestartPlayer(this);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1, 5, FColor::Yellow, FString::Printf(TEXT("You are le dead.")));
			}
			return;
		}
		else
		{
			// Game Over
			return;
		}
}

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
}

bool ASideOpPlayerController::ServerRPCAddCoin_Validate()
{
	return true;
}

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