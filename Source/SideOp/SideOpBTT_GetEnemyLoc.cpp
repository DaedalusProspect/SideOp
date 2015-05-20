// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"
#include "SideOpAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "SideOpBTT_GetEnemyLoc.h"


USideOpBTT_GetEnemyLoc::USideOpBTT_GetEnemyLoc(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

EBTNodeResult::Type USideOpBTT_GetEnemyLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASideOpAIController* MyController = Cast<ASideOpAIController>(OwnerComp.GetAIOwner());
	if (MyController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	ASideOpCharacter* Enemy = MyController->GetEnemy();
	if (Enemy)
	{
		const FVector Loc = Enemy->GetActorLocation();
		if (Loc != FVector::ZeroVector)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}