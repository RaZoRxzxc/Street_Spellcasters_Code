// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputHandlerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STREET_SPELLCASTERS_API IInputHandlerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleMove(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleLook(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleJump();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleStopJumping();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleSprintStart();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleSprintStop();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleBlockStart();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleBlockEnd();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleEvade(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleInteract();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleHeal();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Input")
	void HandleToggleMap();
};
