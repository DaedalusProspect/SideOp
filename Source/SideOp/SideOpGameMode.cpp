// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"
#include "SideOpGameMode.h"
#include "SideOpCharacter.h"

ASideOpGameMode::ASideOpGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerDefaultPawnObject(TEXT("Pawn'/Game/Blueprints/BP_Character.BP_Character_C'"));
	static ConstructorHelpers::FClassFinder<APawn> BluePlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Blue.BP_Character_Blue_C'"));
	static ConstructorHelpers::FClassFinder<APawn> BeigePlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Beige.BP_Character_Beige_C'"));
	static ConstructorHelpers::FClassFinder<APawn> GreenPlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Green.BP_Character_Green_C'"));
	static ConstructorHelpers::FClassFinder<APawn> YellowPlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Yellow.BP_Character_Yellow_C'"));
	static ConstructorHelpers::FClassFinder<APawn> PinkPlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Pink.BP_Character_Pink_C'"));

	// set default pawn class to our default character
	if (PlayerDefaultPawnObject.Class != NULL /*&& HUDObject.Class != NULL*/)
	{
		DefaultPawnClass = PlayerDefaultPawnObject.Class; 

	}

	// Set the rest of the characters
	if (BluePlayerObject.Class != NULL)
	{
		BluePlayer = BluePlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Blue Pawn Set"));
	}

	if (BeigePlayerObject.Class != NULL)
	{
		BeigePlayer = BeigePlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Beige Pawn Set"));
	}

	if (GreenPlayerObject.Class != NULL)
	{
		GreenPlayer = GreenPlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Green Pawn Set"));
	}

	if (YellowPlayerObject.Class != NULL)
	{
		YellowPlayer = YellowPlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Yellow Pawn Set"));
	}

	if (PinkPlayerObject.Class != NULL)
	{
		PinkPlayer = PinkPlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Pink Pawn Set"));
	}

	

}

void ASideOpGameMode::BeginPlay()
{
	Super::BeginPlay();
	InitialID = GetWorld()->GetGameState()->PlayerArray[0]->PlayerId;
}



UClass* ASideOpGameMode::GetDefaultPawnClassForController(AController* InController)
{
	Super::GetDefaultPawnClassForController(InController);
	APlayerController* PC = InController->CastToPlayerController();
	int32 PlayerID = PC->PlayerState->PlayerId;
	
	UE_LOG(LogTemp, Warning, TEXT("Player ID is: %d"), PlayerID);


	if (PlayerID == InitialID)
	{
		if (BluePlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Blue Player"));
			return BluePlayer;

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Using the default Player"));
			return DefaultPawnClass;
		}
	}
	else if (PlayerID == (InitialID + 1))
	{
		if (BeigePlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Beige Player"));
			return BeigePlayer;

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Using the default Player"));
			return DefaultPawnClass;
		}
	}
	else if (PlayerID == (InitialID + 2))
	{
		if (GreenPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Green Player"));
			return GreenPlayer;

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Using the default Player"));
			return DefaultPawnClass;
		}
	}
	else if (PlayerID == (InitialID + 3))
	{
		if (YellowPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Yellow Player"));
			return YellowPlayer;

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Using the default Player"));
			return DefaultPawnClass;
		}
	}
	else if (PlayerID == (InitialID + 4))
	{
		if (PinkPlayer)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Pink Player"));
			return PinkPlayer;

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Using the default Player"));
			return DefaultPawnClass;
		}
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("We're using the default :/"));
		return DefaultPawnClass;
	}
}