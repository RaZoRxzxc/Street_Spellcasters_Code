// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MyGameInstance.h"
#include "Engine/DataTable.h"
#include "Characters/BaseCharacter.h"

void UMyGameInstance::Init()
{
	Super::Init();

	LoadCharacterSelection();

	if (!SelectedCharacter.Character && CharacterDataTable)
	{
		TArray<FCharacterStruct*> AllCharacters;
		CharacterDataTable->GetAllRows<FCharacterStruct>(TEXT(""), AllCharacters);

		if (AllCharacters.Num() > 0)
		{
			// Sort by CharacterID for consistency
			AllCharacters.Sort([](const FCharacterStruct& A, const FCharacterStruct& B) {
				return A.CharacterID.LexicalLess(B.CharacterID);
			});

			SetSelectedCharacter(*AllCharacters[0]);
			UE_LOG(LogTemp, Log, TEXT("Initialized first character in GameInstance: %s"), *AllCharacters[0]->CharacterName.ToString());
		}
	}
}

void UMyGameInstance::SetSelectedCharacter(const FCharacterStruct& CharacterStruct)
{
	SelectedCharacter = CharacterStruct;
	LastSelectedCharacterID = CharacterStruct.CharacterID;
	SaveCharacterSelection();
	UE_LOG(LogTemp, Log, TEXT("Character selected and saved: %s"), *CharacterStruct.CharacterName.ToString());
}

void UMyGameInstance::SaveCharacterSelection()
{
	if (GConfig && !LastSelectedCharacterID.IsNone())
	{
		GConfig->SetString(
			TEXT("CharacterSelection"),
			TEXT("LastSelectedCharacterID"),
			*LastSelectedCharacterID.ToString(),
			GGameIni
		);
		GConfig->Flush(false, GGameIni);
	}
}

void UMyGameInstance::LoadCharacterSelection()
{
	if (!CharacterDataTable || !GConfig) return;

	FString SavedCharacterID;
	if (GConfig->GetString(
		TEXT("CharacterSelection"),
		TEXT("LastSelectedCharacterID"),
		SavedCharacterID,
		GGameIni
	))
	{
		TArray<FCharacterStruct*> AllCharacters;
		CharacterDataTable->GetAllRows<FCharacterStruct>(TEXT(""), AllCharacters);

		for (auto* Character : AllCharacters)
		{
			if (Character->CharacterID.ToString() == SavedCharacterID)
			{
				SelectedCharacter = *Character;
				LastSelectedCharacterID = Character->CharacterID;
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No saved character selection found"));
	}
}
