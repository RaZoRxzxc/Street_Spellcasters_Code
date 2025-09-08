// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ZoneActor.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ZoneInterface.h"
#include "BasePlayerController.generated.h"

class AZoneActor;

UCLASS()
class STREET_SPELLCASTERS_API ABasePlayerController : public APlayerController, public IZoneInterface
{
	GENERATED_BODY()

public:

	ABasePlayerController();

	void OnZoneBeginPlay_Implementation(AZoneActor* ZoneRef) override;

	virtual void BeginPlay() override;

private:

	void DelayedZoneSetup();

	AZoneActor* ZoneActor;

	UFUNCTION()
	void OnZoneInit(float ZoneBeginsIn);

	UFUNCTION()
	void OnZoneShrinking(FCurrentStep CurrentConfig);

	UFUNCTION()
	void OnZonePaused(float NextPointIn);
	
	UFUNCTION()
	void OnZoneEnded();
	
};
