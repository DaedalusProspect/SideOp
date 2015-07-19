// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SideOp.h"
#include "GameFramework/PlayerState.h"
#include "SideOpPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API ASideOpPlayerState : public APlayerState
{
	GENERATED_BODY()

	// This is so we can reset our players records
	virtual void Reset() override;

	// Let's set our player color in the client initialize
	// This means we will have to override it
	// @param	InController	is the controller to initialize us with
	//
	virtual void ClientInitialize(class AController* InController) override;

	//virtual void UnregisterPlayerWithSession() override;
	//

	// This determines what color to use for our pawn eventually will have character select
	// @param PlayerChoice will eventually be what color the player selects.
	void DeterminePawnClass(EPlayerColor::Color PlayerChoice);
	
protected:

	// Our player's color. For character purposes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient, Replicated)
	TEnumAsByte<EPlayerColor::Color>  PlayerColor;

	// How many lives he currently has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Replicated)
	int32 PlayerLives;

	// The players current level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Replicated)
	int32 PlayerLevel;

	// The players current amount of XP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	int32 CurrentXP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	float XPPercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Replicated)
	float LevelCompletion;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	float Stamina;

	// Did the user quit the match?
	bool bPlayerQuit;

	// Can we recharge
	bool bCanRecharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsSprinting;

	bool bHaveZones;
	FVector FinishLocation;
	FVector StartLocation;
	/////////////////////

public:

	ASideOpPlayerState(const FObjectInitializer& ObjectInitializer);


	// Getters for all of our player data
	FORCEINLINE int32 GetPlayerLives() const { return PlayerLives; }
	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
	FORCEINLINE TEnumAsByte<EPlayerColor::Color> GetPlayerColor() const { return PlayerColor; }
	FORCEINLINE void SetPlayerColor(TEnumAsByte<EPlayerColor::Color> NewColor) { PlayerColor = NewColor; }
	FORCEINLINE int32 GetCurrentXP() const { return CurrentXP; }
	FORCEINLINE float GetCompletionPercent() const { return LevelCompletion; }
	FORCEINLINE bool DidPlayerQuit() const { return bPlayerQuit; }
	FORCEINLINE bool IsSprinting() const { return bIsSprinting; }
	FORCEINLINE void SetSprinting(bool Sprint) { bIsSprinting = Sprint; }
	
	// Add Lives to our player
	void AddLives(int32 Lives);

	// Add experience to our player
	UFUNCTION(BlueprintCallable, Category = "SideOp Gameplay")
	void AddXP(int32 XP);

	// Let our stamina recharge over time
	void RechargeStamina();

	// Check for a level up
	bool CheckForLevelUp();

	// Level up the player
	void OnLevelUp();

	// Handle a death
	void OnDeath();

	// Functions to call when getting a kill or being killed
	void ScoreKill(class ASideOpPlayerState* Victim, int32 XP);
	void ScoreDeath(class ASideOpPlayerState* KilledBy, int32 XP);

	/*
	UFUNCTION(BlueprintNativeEvent)
	void Die();
	void Die_Implementation();
	*/

	void UpdatePosition(FVector Position);

	// Returns the short name of our player so it fits in the right places
	FString GetShortName() const;

	// Tell the other clients about the kill
	//UFUNCTION(Reliable, Client)
	//void BroadcastKill(class ASideOpPlayerState* KillerPlayerState, class ASideOpPlayerState* KilledPlayerState);
	//void BroadcastKill_Implementation(class ASideOpPlayerState* KillerPlayerState, class ASideOpPlayerState* KilledPlayerState);

	// Tell everyone about the death locally
	//UFUNCTION(Reliable, NetMulticast)
	//void BroadcastDeath(class ASideOpPlayerState* KillerPlayerState, class ASideOpPlayerState* KilledPlayerState);
	//void BroadcastDeath_Implementation(class ASideOpPlayerState* KillerPlayerState, class ASideOpPlayerState* KilledPlayerState);



	// For copying our player state
	virtual void CopyProperties(class APlayerState* PlayerState) override;
};
