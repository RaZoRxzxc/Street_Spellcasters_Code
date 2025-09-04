// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZoneInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZoneInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STREET_SPELLCASTERS_API IZoneInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zone")
	void OnZoneBeginPlay(AZoneActor* ZoneRef);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zone")
	void OnZoneDestroyed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zone")
	void OnZoneInside();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zone")
	void OnZoneOutSide();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zone")
	void OnZoneDamageTaken(float Damage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zone")
	void OnZoneNearZone(bool isNear);
};
