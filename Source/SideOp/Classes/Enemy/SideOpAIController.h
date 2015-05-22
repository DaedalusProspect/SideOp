// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "SideOpAIController.generated.h"

/**
 * The SideOpAIController is a base controller for our different bots
 * Each enemy will have its own behavior, which will be decided by the BehaviorTree and BlackBoard
 */
UCLASS()
class SIDEOP_API ASideOpAIController : public AAIController
{
	GENERATED_BODY()
	
private:

	// Our AI Components
	UPROPERTY(Transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorComp;
	
	
public:

	//  Setup our constructor
	ASideOpAIController(const FObjectInitializer& ObjectInitializer);

	// Find our enemy
	UFUNCTION(BlueprintCallable, Category = Behavior)
	void FindClosestEnemy(); 

	// Override Possess so we can start our BehaviorTree
	virtual void Possess(class APawn* InPawn) override;

	// Set the blackboard key and our AI to this enemy
	void SetEnemy(class APawn* InPawn); 
	void ClearEnemy();

	// Get the enemy
	class ASideOpCharacter* GetEnemy() const;
	FVector GetHome() const;
	FVector GetPatrol() const;

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetPatrolLocation(FVector Location);

protected:

	// Holds the id to the blackboard key we need
	int32 EnemyKeyID; 
	int32 EnemyLocKeyID;
	int32 HomeKeyID;
	int32 PatrolKeyID;

public:

	// Returns BlackboardComp subobject 
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	// Returns BehaviorComp subobject
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
};
