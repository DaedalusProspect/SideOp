// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpEnemy.h"
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
		EnemyLocKeyID = BlackboardComp->GetKeyID("EnemyLoc");
		HomeKeyID = BlackboardComp->GetKeyID("Home");
		PatrolKeyID = BlackboardComp->GetKeyID("Patrol");
	}

	if (InPawn)
	{
		ASideOpEnemy* MyBot = Cast<ASideOpEnemy>(InPawn);
		FVector PatrolLocationNormal = InPawn->GetActorLocation() + MyBot->PatrolLocation;
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(HomeKeyID, InPawn->GetActorLocation());
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(PatrolKeyID, PatrolLocationNormal);
	}

	if (Bot && Bot->BotBehavior)
	{
		BehaviorComp->StartTree(*(Bot->BotBehavior));
	}
}

// Set our enemy in the blackboard so the AI can process
void ASideOpAIController::SetEnemy(class APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		BlackboardComp->SetValue<UBlackboardKeyType_Vector>(EnemyKeyID, InPawn->GetActorLocation());
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

FVector ASideOpAIController::GetHome() const
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValue<UBlackboardKeyType_Vector>(HomeKeyID);
	}

	return FVector::ZeroVector;
}

FVector ASideOpAIController::GetPatrol() const
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValue<UBlackboardKeyType_Vector>(PatrolKeyID);
	}

	return FVector::ZeroVector;
}

void ASideOpAIController::ClearEnemy()
{
	BlackboardComp->ClearValue(EnemyKeyID);
}

void ASideOpAIController::SetPatrolLocation(FVector Location)
{
	// Check the location
	if (Location != FVector::ZeroVector)
	{
		if (BlackboardComp)
		{
			BlackboardComp->SetValue<UBlackboardKeyType_Vector>(PatrolKeyID, Location);
		}
	}
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
	float NestRadius = Cast<ASideOpEnemy>(MyBot)->NestRadius;
	ASideOpCharacter* BestPawn = NULL;

	const FVector PatrolAmount = Cast<ASideOpEnemy>(MyBot)->PatrolLocation;

	const FVector NestLoc = GetHome() + (PatrolAmount / 2);
	// This will get all of our pawns and iterate through them, 
	// then check if its one of our characters and that its alive. If so, it will get the sqr distance
	// if its within the best sqdist then it will set that as the enemy
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		// This gets the enemy based on a certain distance from the enemy's "Nest"
		// Which is basically the middle of its patrol location and its home location
		//TODO: Clean this and make more dynamic
		GEngine->AddOnScreenDebugMessage(126, 5.0, FColor::Red, NestLoc.ToString());
		GEngine->AddOnScreenDebugMessage(12, 5.0, FColor::Red, TEXT("Looking For Enemy"));
		ASideOpCharacter* TestPawn = Cast<ASideOpCharacter>(*It);
		if (TestPawn && !TestPawn->IsDead())
		{
			GEngine->AddOnScreenDebugMessage(123, 5.0, FColor::Red, TEXT("Got a Pawn"));
			const float DistSq = (TestPawn->GetActorLocation() - NestLoc).SizeSquared();
			if (DistSq < BestDistSq && DistSq < NestRadius)
			{
				BestDistSq = DistSq;
				BestPawn = TestPawn;
				FString Distance = FString::SanitizeFloat(BestDistSq);
				GEngine->AddOnScreenDebugMessage(1233, 5.0, FColor::Red, *Distance);
			}

		}

	}

	// if we got a pawn, set our blackboard
	if (BestPawn)
	{
		SetEnemy(BestPawn);
		GEngine->AddOnScreenDebugMessage(13, 5.0, FColor::Red, TEXT("FoundEnemy"));
	}
	else
	{
		// Unset the BlackBoard
		ClearEnemy();
		GEngine->AddOnScreenDebugMessage(14, 5.0, FColor::Red, TEXT("NoEnemy"));
	}
}