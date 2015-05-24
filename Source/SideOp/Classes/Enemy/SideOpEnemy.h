// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "SideOpEnemy.generated.h"

UCLASS()
class SIDEOP_API ASideOpEnemy : public APaperCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Behavior")
	class UBehaviorTree* BotBehavior;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* MoveAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* DeathAnimation;

	//
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Transient)
	class UPaperFlipbook* CurrentAnimation;

	//
	// Function to call to update the animation of our pawn
	void UpdateAnimation();

	//

	bool bCanJump;
	bool bCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 XPToGive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Stats")
	int32 HitsToKill;

	UPROPERTY(VisibleAnywhere, Transient, Replicated)
	bool bIsDying;

	UPROPERTY(VisibleAnywhere, Transient, Replicated)
	bool bIsHit;

	// AI Extras






public:
	// AI Extra property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SideOp AI")
	bool bDriveAway;

	// Sets default values for this pawn's properties
	ASideOpEnemy(const FObjectInitializer& ObjectInitializer);


	FORCEINLINE int32 GetXPToGive(){ return XPToGive; }
	FORCEINLINE int32 GetHitsToKill(){ return HitsToKill; }
	FORCEINLINE void SetHitsToKill(int32 Hits){ HitsToKill = Hits; }

	UFUNCTION(Server, WithValidation, Reliable)
	void ServerSetEnemyHit(bool Hit);
	void ServerSetEnemyHit_Implementation(bool Hit);
	bool ServerSetEnemyHit_Validate(bool Hit);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	// For setting a patrol location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", Meta = (MakeEditWidget = true))
	FVector PatrolLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float ChaseRadius;
	
};
