// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpPlayerController.h"
#include "SideOpGameMode.h"


ASideOpPlayerController::ASideOpPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> BluePlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Blue.BP_Character_Blue_C'"));
	static ConstructorHelpers::FClassFinder<APawn> BeigePlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Beige.BP_Character_Beige_C'"));
	static ConstructorHelpers::FClassFinder<APawn> GreenPlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Green.BP_Character_Green_C'"));
	static ConstructorHelpers::FClassFinder<APawn> YellowPlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Yellow.BP_Character_Yellow_C'"));
	static ConstructorHelpers::FClassFinder<APawn> PinkPlayerObject(TEXT("Pawn'/Game/Players/BP_Character_Pink.BP_Character_Pink_C'"));

	// Set the rest of the characters
	if (BluePlayerObject.Class != NULL)
	{
		BluePlayer = BluePlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Blue Pawn Set from control"));
	}

	if (BeigePlayerObject.Class != NULL)
	{
		BeigePlayer = BeigePlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Beige Pawn Set from control"));
	}

	if (GreenPlayerObject.Class != NULL)
	{
		GreenPlayer = GreenPlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Green Pawn Set from control"));
	}

	if (YellowPlayerObject.Class != NULL)
	{
		YellowPlayer = YellowPlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Yellow Pawn Set from control"));
	}

	if (PinkPlayerObject.Class != NULL)
	{
		PinkPlayer = PinkPlayerObject.Class;
		UE_LOG(LogTemp, Warning, TEXT("Pink Pawn Set from control"));
	}

	PlayerPawn = BluePlayer; // For whatever reason, this always sets the servers pawn. The rest of the set pawn code does nothing for it.
	// Set to replicate
	bReplicates = true;
	bPlayerSet = false;

}
void ASideOpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


}


void ASideOpPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetPCPawn();
}


bool ASideOpPlayerController::ServerRPCSetPawn_Validate(TSubclassOf<APawn> InPawnClass, EPlayerColor InColor, bool IsSet)
{
	return true;
}

void ASideOpPlayerController::ServerRPCSetPawn_Implementation(TSubclassOf<APawn> InPawnClass, EPlayerColor InColor, bool IsSet)
{

		PlayerPawn = InPawnClass;
		PlayerColor = InColor;
		// Only restart the player once for the beginning of the map
		GetWorld()->GetAuthGameMode()->RestartPlayer(this);

}

void ASideOpPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASideOpPlayerController, PlayerPawn);
	DOREPLIFETIME(ASideOpPlayerController, PlayerColor);
	DOREPLIFETIME(ASideOpPlayerController, bPlayerSet);
}

void ASideOpPlayerController::SetPCPawn_Implementation()
{
	// This check is to make sure for whatever reason it doesnt try and reset our pawn color
	if (bPlayerSet)
	{
		ServerRPCSetPawn(PlayerPawn, PlayerColor);
		return;
	}
	else
	{
		int32 PlayColor = FMath::RandRange(1, 4);
		switch (PlayColor)
		{
		case 1:
			PlayerPawn = BluePlayer;
			PlayerColor = EPlayerColor::Blue;

		case 2:
			PlayerPawn = BeigePlayer;
			PlayerColor = EPlayerColor::Beige;

		case 3:
			PlayerPawn = GreenPlayer;
			PlayerColor = EPlayerColor::Green;

		case 4:
			PlayerPawn = PinkPlayer;
			PlayerColor = EPlayerColor::Pink;

		default:
			PlayerPawn = BluePlayer;
			PlayerColor = EPlayerColor::Blue;

		}
		ServerRPCSetPawn(PlayerPawn, PlayerColor);
		bPlayerSet = true;
		return;
	}
	return;
}

