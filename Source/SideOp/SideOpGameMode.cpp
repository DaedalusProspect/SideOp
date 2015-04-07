// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SideOp.h"
#include "SideOpGameMode.h"
#include "SideOpCharacter.h"
#include "SideOpPlayerController.h"

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

	LastUsedPawn = 0;
	
}

void ASideOpGameMode::BeginPlay()
{
	Super::BeginPlay();
}



UClass* ASideOpGameMode::GetDefaultPawnClassForController(AController* InController)
{
	Super::GetDefaultPawnClassForController(InController);
	ASideOpPlayerController* PC = Cast<ASideOpPlayerController>(InController);
	TSubclassOf<APawn> ClassToUse = nullptr;
	if (PC != nullptr)
	{
		ClassToUse = PC->GetPlayerPawnClass();
		return ClassToUse;
	}
	else
	{
		return DefaultPawnClass;
	}
}