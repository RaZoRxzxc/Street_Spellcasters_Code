// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STREET_SPELLCASTERS_API IHealthInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health")
	float GetHealthPercent() const;
};
