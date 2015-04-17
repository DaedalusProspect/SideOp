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
}


