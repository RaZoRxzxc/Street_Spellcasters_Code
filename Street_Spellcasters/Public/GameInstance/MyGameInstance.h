// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Structs/CharacterStruct.h"
#include "MyGameInstance.generated.h"

UCLASS()
class STREET_SPELLCASTERS_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Selection")
	FCharacterStruct SelectedCharacter;
	
	UFUNCTION(BlueprintCallable, Category = "Character Selection")
	void SetSelectedCharacter(const FCharacterStruct& CharacterStruct) { SelectedCharacter = CharacterStruct; }
	
	UFUNCTION(BlueprintCallable, Category = "Character Selection")
	FCharacterStruct GetSelecterdCharacter() const { return SelectedCharacter; }
};
