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


	
public:

	// This start zones color
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Replicated)
	TEnumAsByte<EPlayerColor::Color>  StartColor;


	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
};
