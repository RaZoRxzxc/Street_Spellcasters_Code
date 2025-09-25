// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MenuCameraActor.generated.h"

class UCameraComponent;

UCLASS()
class STREET_SPELLCASTERS_API AMenuCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMenuCameraActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* MenuCamera;

public:	
	virtual void Tick(float DeltaTime) override;

};
