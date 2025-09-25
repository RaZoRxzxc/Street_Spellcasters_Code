// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStruct.generated.h"

USTRUCT(BlueprintType)
struct STREET_SPELLCASTERS_API FCharacterStruct: public FTableRowBase
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CharacterID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABaseCharacter> Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ALobbyCharacter> LobbyCharacter;
};
