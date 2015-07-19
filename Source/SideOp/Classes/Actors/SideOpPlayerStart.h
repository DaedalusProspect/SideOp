// Copyright 2015 Aaron Mares.

#pragma once

#include "Classes/Player/SideOpPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "SideOpPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class SIDEOP_API ASideOpPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
protected:
	// This start zones color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Replicated)
	TEnumAsByte<EPlayerColor::Color>  StartColor;
	
public:


	FORCEINLINE TEnumAsByte<EPlayerColor::Color> const GetStartColor(){ return StartColor; }

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
};
