// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class STREET_SPELLCASTERS_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Widget)
	UUserWidget* MenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Widget)
	TSubclassOf<UUserWidget> MenuWidgetClass;
};
