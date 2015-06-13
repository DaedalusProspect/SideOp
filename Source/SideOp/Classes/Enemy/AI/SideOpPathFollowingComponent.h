// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Navigation/NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "SideOpPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API USideOpPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
	
protected:
	/** cached UCharacterMovementComponent */
	UPROPERTY(transient)
		UCharacterMovementComponent* CharacterMoveComp;

public:
	USideOpPathFollowingComponent(const FObjectInitializer& ObjectInitializer);
	// this dictates the behavior for following the move segment
	virtual void FollowPathSegment(float DeltaTime) override;

	// used to detect properties of a path's segment a character is about to follow
	//virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	// used to cache UCharacterMovementComponent we're using in SetMoveSegment implementation
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;

	// Rama UE4 C++ AI Code for you!
	//     add this to your custom path follow component!

	/*//Nav Data Main
	FORCEINLINE const ANavigationData* GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound) const
	{
		UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();
		if (!NavSys) return NULL;
		return NavSys->GetMainNavData(CreateNewIfNoneFound);
	}

	//Choose Which Nav Data To Use
	FORCEINLINE const ANavigationData* SideGetNavData() const
	{
		if (!MovementComp)
		{
			return GetMainNavData();
		}

		const FNavAgentProperties& AgentProperties = MovementComp->GetNavAgentPropertiesRef();
		const ANavigationData* NavData = GetNavDataForProps(AgentProperties);
		if (NavData == NULL)
		{
			NavData = GetMainNavData();
		}

		return NavData;
	}

	//VERY IMPORTANT FOR CRASH PROTECTION !!!!!
	FORCEINLINE bool TileIsValid(const ARecastNavMesh* NavMesh, int32 TileIndex) const
	{
		if (!NavMesh) return false;
		//~~~~~~~~~~~~~~
		const FBox TileBounds = NavMesh->GetNavMeshTileBounds(TileIndex);

		return TileBounds.IsValid != 0;
	}

	//add this to your custom path follow component!
	bool NavPoly_GetAllPolys(TArray<NavNodeRef>& Polys);
	*/
};
