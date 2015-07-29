// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GameFramework/GameMode.h"
#include "SideOpGameMode.generated.h"

// The GameMode defines the game being played. It governs the game rules, scoring, what actors
// are allowed to exist in this game type, and who may enter the game.
//
// 

UCLASS(minimalapi)
class ASideOpGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual class AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;

protected:
	// The Max Level for our players
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 MaxLevel;

	// What amount of XP is required for the first level,
	// This also decides amount of xp per level (XPSeed * Level = Amount needed)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int32 XPSeed;

	// How much XP is needed to get to the index level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<int32> XPToLevel;

	// This takes an argument so we can call it again from blueprints
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void SeedXPTable(int32 Seed);

public:
	ASideOpGameMode(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE int32 GetMaxLevel() { return MaxLevel; }
	int32 GetXPToLevel(int32 CurrentLevel);

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* InController) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
};
