// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SideOpBTT_GetEnemyLoc.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API USideOpBTT_GetEnemyLoc : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	USideOpBTT_GetEnemyLoc(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
