// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

class UDataTable;

UCLASS()
class STREET_SPELLCASTERS_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SpawnSelectedCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	UDataTable* CharacterDataTable;
	
};
