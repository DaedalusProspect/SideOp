// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"




FVector ASideOpGameState::GetPlayerPos(EPlayerColor::Color PlayerColor)
{
	
	// THis gets the player with the matching player color and returns their current position
	// This is used for updating the UI
	for (auto Itr(PlayerArray.CreateIterator()); Itr; Itr++)
	{
		ASideOpPlayerState* OurState = Cast<ASideOpPlayerState>(*Itr);
		if (OurState)
		{
			if (OurState->GetPlayerColor() == PlayerColor)
			{
				ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(OurState->GetOwner());
				if (PC)
				{
					return PC->GetCharacter()->GetActorLocation();
				}
			}
		}
	}

	return FVector::ZeroVector;
}