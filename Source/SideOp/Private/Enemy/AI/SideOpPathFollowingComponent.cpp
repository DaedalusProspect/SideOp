// Fill out your copyright notice in the Description page of Project Settings.

#include "SideOp.h"


USideOpPathFollowingComponent::USideOpPathFollowingComponent(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{

}

void USideOpPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	//Pointer Safety Checks
	if (MovementComp == NULL || !Path.IsValid())
	{
		return;
	}


	//Use Jump/Fall Pathing?
	if (Path->IsPartial()) //AI could not reach player, so we have to jump or fall
	{
		//I send out instructions to my custom character class here
		//JoyChar->ReceiveJumpFallPathingRequest();

		return;
		//Dont do normal pathing, jump
	}


	//Proceed normally (no jump pathing)
	Super::FollowPathSegment(DeltaTime);
}

/*
void USideOpPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	if (CharacterMoveComp != NULL)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

		if (FNavAreaHelper::HasJumpFlag(SegmentStart))
		{
			// TODO: Implement better jump logic
			CharacterMoveComp->SetMovementMode(MOVE_Flying);
			FVector JumpHeight = SegmentStart.
		}
		else
		{
			// regular move
			CharacterMoveComp->SetMovementMode(MOVE_Walking);
		}
	}
}*/

void USideOpPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);

	CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
}

/*//Rama's UE4 Nav code to get all the nav polys!
bool UJoyPathFollowComp::NavPoly_GetAllPolys(TArray<NavNodeRef>& Polys)
{
	if (!MovementComp) return false;
	//~~~~~~~~~~~~~~~~~~

	//Get Nav Data
	const ANavigationData* NavData = JoyGetNavData();

	const ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
	if (!NavMesh)
	{
		return false;
	}

	TArray<FNavPoly> EachPolys;
	for (int32 v = 0; v < NavMesh->GetNavMeshTilesCount(); v++)
	{

		//CHECK IS VALID FIRST OR WILL CRASH!!! 
		//     256 entries but only few are valid!
		// use continue in case the valid polys are not stored sequentially
		if (!TileIsValid(NavMesh, v))
		{
			continue;
		}

		NavMesh->GetPolysInTile(v, EachPolys);
	}


	//Add them all!
	for (int32 v = 0; v < EachPolys.Num(); v++)
	{
		Polys.Add(EachPolys[v].Ref);
	} 

}*/