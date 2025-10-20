// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ZoneActor.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ZoneInterface.h"
#include "BasePlayerController.generated.h"

class AZoneActor;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class STREET_SPELLCASTERS_API ABasePlayerController : public APlayerController, public IZoneInterface
{
	GENERATED_BODY()

public:

	ABasePlayerController();

	void OnZoneBeginPlay_Implementation(AZoneActor* ZoneRef) override;

	virtual void BeginPlay() override;

protected:

	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Sprint Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	
	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Attack Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	// Block Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	// Evade Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* EvadeAction;

	// Interact Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	// Heal Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* HealAction;

	// Map Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MapAction;
	
	// Mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;
	
private:
	/** Setup input function */
	virtual void SetupInputComponent() override;
	
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

	UFUNCTION()
    void HandleMove(const FInputActionValue& Value);

    UFUNCTION()
    void HandleLook(const FInputActionValue& Value);

    UFUNCTION()
    void HandleJump();

    UFUNCTION()
    void HandleStopJumping();

    UFUNCTION()
    void HandleSprintStart();

    UFUNCTION()
    void HandleSprintStop();

    UFUNCTION()
    void HandleAttack();

    UFUNCTION()
    void HandleBlockStart();

    UFUNCTION()
    void HandleBlockEnd();

    UFUNCTION()
    void HandleEvade(const FInputActionValue& Value);

    UFUNCTION()
    void HandleInteract();

    UFUNCTION()
    void HandleHeal();

    UFUNCTION()
    void HandleToggleMap();
};
