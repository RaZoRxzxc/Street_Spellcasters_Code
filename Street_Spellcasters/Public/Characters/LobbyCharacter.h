// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LobbyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class STREET_SPELLCASTERS_API ALobbyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALobbyCharacter();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

public:	
	virtual void Tick(float DeltaTime) override;

};
