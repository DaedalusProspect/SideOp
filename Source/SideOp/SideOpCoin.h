// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "SideOpCharacter.h"
#include "GameFramework/Actor.h"
#include "SideOpCoin.generated.h"


UCLASS()
class SIDEOP_API ASideOpCoin : public AActor
{
	GENERATED_BODY()

	// Our Collision comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComp;

	// Our sprite component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* SpriteComp;

protected:
	// How much XP will this coin give?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	int32 XPToGive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite)
	class UPaperFlipbook* CoinFlipbook;
	
public:	
	// Sets default values for this actor's properties
	ASideOpCoin(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE int32 GetXPToGive(){ return XPToGive; }

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	


};
