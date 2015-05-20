// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SIdeOpEnemy.h"
#include "SideOpAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

ASideOpAIController::ASideOpAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Setup the blackboard component
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, "BlackboardComp");

	// Setup the behavior tree component
	BrainComponent = BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, "BehaviorComp");


}

// Start all of our AI when we possess our pawn
void ASideOpAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	ASideOpEnemy* Bot = Cast<ASideOpEnemy>(InPawn);

	// Check that we actually have a bot and can start its AI
	if (Bot && Bot->BotBehavior)
	{
		if (Bot->BotBehavior->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Bot->BotBehavior->BlackboardAsset);
		}

		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		HomeKeyID = BlackboardComp->GetKeyID("Home");
	}

	if (InPawn)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(HomeKeyID, InPawn->GetActorLocation());
	}

	if (Bot && Bot->BotBehavior)
	{
		BehaviorComp->StartTree(*(Bot->BotBehavior));
		GEngine->AddOnScreenDebugMessage(8, 1.0f, FColor::Magenta, TEXT("YAY!"));
	}
}

// Set our enemy in the blackboard so the AI can process
void ASideOpAIController::SetEnemy(class APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
	}
}

// Get the value of the enemy key from the blackboard
class ASideOpCharacter* ASideOpAIController::GetEnemy() const
{
	if (BlackboardComp)
	{
		return Cast<ASideOpCharacter>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}

	return NULL;
}

// Find the closest enemy and set the blackboard
void ASideOpAIController::FindClosestEnemy()
{
	// Get our bot, if not fail out
	APawn* MyBot = GetPawn();
	if (MyBot == NULL)
	{
		return;
	}

	const FVector MyLoc = MyBot->GetActorLocation();
	float BestDistSq = MAX_FLT;
	ASideOpCharacter* BestPawn = NULL;

	// This will get all of our pawns and iterate through them, 
	// then check if its one of our characters and that its alive. If so, it will get the sqr distance
	// if its within the best sqdist then it will set that as the enemy
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		ASideOpCharacter* TestPawn = Cast<ASideOpCharacter>(*It);
		if (TestPawn && !TestPawn->IsDead())
		{
			const float DistSq = (TestPawn->GetActorLocation() - MyLoc).SizeSquared();
			if (DistSq < BestDistSq)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
			}
		}
	}

	// if we got a pawn, set our blackboard
	if (BestPawn)
	{
		SetEnemy(BestPawn);
	}
}