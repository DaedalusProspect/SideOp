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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated, Transient, Category = Player)
	int32 CoinsCollected;

	// Our players level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	int32 PlayerLevel;

	// Amount of experience
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	float CurrentXP;

	// Amount of XP needed to level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Player)
	float XPToLevel;

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

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCSetLives(int32 Lives);
	virtual void ServerRPCSetLives_Implementation(int32 Lives);
	virtual bool ServerRPCSetLives_Validate(int32 Lives);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCAddXP(int32 Amount);
	virtual void ServerRPCAddXP_Implementation(int32 Amount);
	virtual bool ServerRPCAddXP_Validate(int32 Amount);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCSubtractXP(int32 Amount);
	virtual void ServerRPCSubtractXP_Implementation(int32 Amount);
	virtual bool ServerRPCSubtractXP_Validate(int32 Amount);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRPCAddLevel();
	void ServerRPCAddLevel_Implementation();
	bool ServerRPCAddLevel_Validate();
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
	FORCEINLINE int32 GetCoinsCollected(){ return CoinsCollected; }
	FORCEINLINE int32 GetPlayerLevel(){ return PlayerLevel; }
	FORCEINLINE int32 GetCurrentXP(){ return CurrentXP; }
	FORCEINLINE int32 GetXPToLevel(){ return XPToLevel; }

	FORCEINLINE void SetPlayerColor(EPlayerColor Color){ PlayerColor = Color; }
	FORCEINLINE void SetPlayerLives(int32 Lives){ PlayerLives = Lives; }
	FORCEINLINE void SetPlayerHealth(float Health){ PlayerHealth = Health; }
	FORCEINLINE void SetPlayerLevel(int32 Level){ PlayerLevel = Level; }

	// Called to add a coin to our characters score
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void AddCoin();

	// Called to subtract a coin from our character
	UFUNCTION(BlueprintCallable, Category = "Coins")
	void SubtractCoin();

	UFUNCTION(BlueprintCallable, Category = Player)
	void AddXP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = Player)
	void SubtractXP(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = Player)
	bool CheckForLevelUp();

	UFUNCTION(BlueprintCallable, Category = Player)
	void LevelUp();

	UFUNCTION(BlueprintCallable, Category = Player)
	void CalculateXPPercent();

	UFUNCTION(BlueprintNativeEvent)
	void Die();
	void Die_Implementation();

	UFUNCTION(BlueprintCallable, Category = Message)
	void ClearMessage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Message)
	FString MessageText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float XPPercent;

	// Some simple functions for movement disabling
	void EnableMovement();
	void DisableMovement();

	// Handle our timers
	FTimerHandle TimerHandler;

};
