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
	class UPaperFlipbook* BaseAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Animations)
	class UPaperFlipbook* DeathAnimation;

	//
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Transient)
	class UPaperFlipbook* CurrentAnimation;

	//



	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 XPToGive;

	UPROPERTY(VisibleAnywhere, Transient, Replicated)
	bool bIsDying;

	UPROPERTY(VisibleAnywhere, Transient, Replicated)
	bool bIsHit;
public:
	// Sets default values for this pawn's properties
	ASideOpEnemy(const FObjectInitializer& ObjectInitializer);


	FORCEINLINE int32 GetXPToGive(){ return XPToGive; }

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
};
