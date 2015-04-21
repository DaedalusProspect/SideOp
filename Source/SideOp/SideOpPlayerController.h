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

	// We need to keep track of our player color for the HUD, as well as for respawns
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Category = Player)
		EPlayerColor PlayerColor;

	// How many lives we have left
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
		int32 PlayerLives;

	// The health we have left
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
		float PlayerHealth;

	// How many coins we've collected
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Transient, Category = Gameplay)
		int32 CoinsCollected;


	virtual void SetupInputComponent() override;

	UFUNCTION(Reliable, Client)
	void DeterminePawnClass();
	virtual void DeterminePawnClass_Implementation();

	virtual void BeginPlay() override;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCSetPawn(TSubclassOf<APawn> InPawnClass);
	virtual void ServerRPCSetPawn_Implementation(TSubclassOf<APawn> InPawnClass);
	virtual bool ServerRPCSetPawn_Validate(TSubclassOf<APawn> InPawnClass);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCAddCoin();
	virtual void ServerRPCAddCoin_Implementation();
	virtual bool ServerRPCAddCoin_Validate();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCSubtractCoin();
	virtual void ServerRPCSubtractCoin_Implementation();
	virtual bool ServerRPCSubtractCoin_Validate();
	///////////////////////////////////////////////////////////////////////////
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

	//////////////////////////////////////////////////////////////////////////////////

public:

	ASideOpPlayerController(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE UClass* GetPlayerPawnClass(){ return PlayerPawn; }
	FORCEINLINE EPlayerColor GetPlayerColor(){ return PlayerColor; }
	FORCEINLINE int32 GetPlayerLives(){ return PlayerLives; }
	FORCEINLINE float GetPlayerHealth(){ return PlayerHealth; }

	FORCEINLINE void SetPlayerColor(EPlayerColor Color){ PlayerColor = Color; }
	FORCEINLINE void SetPlayerLives(int32 Lives){ PlayerLives = Lives; }
	FORCEINLINE void SetPlayerHealth(float Health){ PlayerHealth = Health; }

	// Called to add a coin to our characters score
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void AddCoin();

	// Called to subtract a coin from our character
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SubtractCoin();

	void Die();

};
