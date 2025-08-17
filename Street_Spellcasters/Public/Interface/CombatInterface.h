// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STREET_SPELLCASTERS_API ICombatInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void CallToggleWeaponTrace(bool bIsWeaponTraceOn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void MeleeAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void IsPlayerAttack(bool& bIsAttack);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void isPlayerDead(bool& IsDead);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void StartBlock();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void EndBlock();
};
