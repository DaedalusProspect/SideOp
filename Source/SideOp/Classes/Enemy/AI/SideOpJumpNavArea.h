// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Navigation/NavAreas/NavArea.h"
#include "SideOpJumpNavArea.generated.h"

// Create some flags for our nav area
UENUM()
namespace ENavAreaFlag
{
	// TODO: Add more states as needed. 15
	enum Type
	{
		Default,
		Jump,
		Crouch,
		
	};
}

/* Just some helper stuff to get flags and whatnot
namespace FNavAreaHelper
{
	FORCEINLINE bool IsSet(uint16 Flags, ENavAreaFlag::Type Bit) { return (Flags & (1 << Bit)) != 0; }
	FORCEINLINE void Set(uint16& Flags, ENavAreaFlag::Type Bit) { Flags |= (1 << Bit); }

	FORCEINLINE bool IsNavLink(const FNavPathPoint& PathVert) { return (FNavMeshNodeFlags(PathVert.Flags).PathFlags & RECAST_STRAIGHTPATH_OFFMESH_CONNECTION) != 0; }
	FORCEINLINE bool HasJumpFlag(const FNavPathPoint& PathVert) { return IsSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, ENavAreaFlag::Jump); }
	FORCEINLINE bool HasCrouchFlag(const FNavPathPoint& PathVert) { return IsSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, ENavAreaFlag::Crouch); }
}
*/
/**
 * The class for our special nav area that lets our AI jump between platforms.
 */
UCLASS()
class SIDEOP_API USideOpJumpNavArea : public UNavArea
{
	GENERATED_BODY()


	USideOpJumpNavArea(const FObjectInitializer& ObjectInitializer);
	
};
