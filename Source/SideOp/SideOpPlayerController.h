// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SideOpPlayerController.generated.h"


/**
 *
 */
UENUM(BlueprintType)
enum class EPlayerColor : uint8
{
	Blue,
	Beige,
	Green,
	Pink,
	Yellow,
	Unset
};

/**
 * 
 */
UCLASS()
class SIDEOP_API ASideOpPlayerController : public APlayerController
{
	GENERATED_BODY()



protected:

	virtual void SetupInputComponent() override;

	UFUNCTION(Reliable, Client)
	void DeterminePawnClass();
	virtual void DeterminePawnClass_Implementation();

	virtual void BeginPlay() override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCSetPawn(TSubclassOf<APawn> InPawnClass);
	virtual void ServerRPCSetPawn_Implementation(TSubclassOf<APawn> InPawnClass);
	virtual bool ServerRPCSetPawn_Validate(TSubclassOf<APawn> InPawnClass);

	UPROPERTY(Replicated)
	TSubclassOf<APawn> PlayerPawn;

	// Pawn for the BluePlayer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Players)
	TSubclassOf<APawn> BluePlayer;

	// Pawn for the BeigePlayer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Players)
	TSubclassOf<APawn> BeigePlayer;

	// Pawn for the GreenPlayer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Players)
	TSubclassOf<APawn> GreenPlayer;

	// Pawn for the PinkPlayer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Players)
	TSubclassOf<APawn> PinkPlayer;

	// Pawn for the YellowPlayer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Players)
	TSubclassOf<APawn> YellowPlayer;



public:

	ASideOpPlayerController(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE UClass* GetPlayerPawnClass(){ return PlayerPawn; }

	// We need to keep track of our player color for the HUD, as well as for respawns
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	EPlayerColor PlayerColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	int32 PlayerLives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	float PlayerHealth;

};
