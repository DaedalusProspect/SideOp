// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"



// Our constructor
ASideOpPlayerState::ASideOpPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Setup the defaults
	PlayerColor = EPlayerColor::Unset;
	PlayerLives = 3;
	PlayerLevel = 1;
	Stamina = 1.0f;
	CurrentXP = 0;
	LevelCompletion = 100.0f;
	bPlayerQuit = false;
	XPPercent = 0.0f;
	bHaveZones = false;

}

// The function to call when resetting our players level data (EG. Lives, level, level completion
// this is usually called between levels
void ASideOpPlayerState::Reset()
{
	Super::Reset();


	// Since the player states persist, dont change character color
	// just reset the rest of the player data
	PlayerLives = 3;
	PlayerLevel = 1;
	CurrentXP = 0;
	LevelCompletion = 0.0f;
	bPlayerQuit = false;
}

// On client initialize, set our playername for easy finding in blueprints and elsewhere
void ASideOpPlayerState::ClientInitialize(class AController* InController)
{
	// This is where code will go to get our player color
}

// Add lives to our player
void ASideOpPlayerState::AddLives(int32 Lives)
{
	PlayerLives += Lives;
}

// Add XP to our player
void ASideOpPlayerState::AddXP(int32 XP)
{
	CurrentXP += XP;
	int32 XPNeeded = PlayerLevel * 118;
	XPPercent = CurrentXP / XPNeeded;
	if (CheckForLevelUp())
	{
		OnLevelUp();
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3, 1.0f, FColor::Blue, FString::FromInt(CurrentXP));
	}
}

// Check to see if we have level'd up
bool ASideOpPlayerState::CheckForLevelUp()
{
	// Add logic here for the level up
	int32 XPNeeded = PlayerLevel * 118;
	if (CurrentXP >= XPNeeded)
	{
		return true;
	}
	return false;
}

// What to do on level up
void ASideOpPlayerState::OnLevelUp()
{
	
	// Check if we are max level
	if (PlayerLevel > 6)
	{
		return;
	}

	PlayerLevel++;
	int32 XPNeeded = PlayerLevel * 118;
	//int32 RemainingXP = CurrentXP - XPNeeded;
	CurrentXP = 0;
	XPPercent = CurrentXP / XPNeeded;

}

// For replication purposes
void ASideOpPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASideOpPlayerState, PlayerColor);
	DOREPLIFETIME(ASideOpPlayerState, PlayerLives);
	DOREPLIFETIME(ASideOpPlayerState, PlayerLevel);
	DOREPLIFETIME(ASideOpPlayerState, LevelCompletion);
	DOREPLIFETIME(ASideOpPlayerState, bIsSprinting);
}

// If we need to copy information to another player state
void ASideOpPlayerState::CopyProperties(class APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	ASideOpPlayerState* SideOpState = Cast<ASideOpPlayerState>(PlayerState);
	if (SideOpState)
	{
		SideOpState->PlayerColor = PlayerColor;
	}

}

void ASideOpPlayerState::UpdatePosition(FVector Position)
{
	if (!bHaveZones)
	{
		for (TActorIterator<ASideOpFinishZone> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr)
			{
				if (ActorItr->GetActorLocation() != FVector::ZeroVector)
				{
					FinishLocation = ActorItr->GetActorLocation();
					break;
				}

			}
		}

		for (TActorIterator<ASideOpStartZone> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr)
			{
				if (ActorItr->GetActorLocation() != FVector::ZeroVector)
				{
					StartLocation = ActorItr->GetActorLocation();
					break;
				}

			}
		}

		if (StartLocation != FVector::ZeroVector  && FinishLocation != FVector::ZeroVector)
		{
			bHaveZones = true;
		}


	}
	if (Position != FVector::ZeroVector) // Make sure we got a good position
	{
		float CourseLength = FVector::Dist(StartLocation,FinishLocation);
		float PlayerDist = FVector::Dist(Position, FinishLocation);

		LevelCompletion = PlayerDist / CourseLength;
	}
}
