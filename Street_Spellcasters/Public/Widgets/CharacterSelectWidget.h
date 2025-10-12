// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CharacterStruct.h"
#include "CharacterSelectWidget.generated.h"

class UDataTable;
class UButton;
class UTextBlock;
class UImage;
class UMenuWidget;
class UUniformGridPanel;
class UCharacterButtonWidget;

UCLASS()
class STREET_SPELLCASTERS_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	UDataTable* CharacterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UCharacterButtonWidget> CharacterButtonClass;

	FCharacterStruct CharacterStruct;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterClassText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterDescriptionText;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* CharacterGridPanel;

private:
	UFUNCTION()
	void OnCharacterSelected(const FCharacterStruct& SelectedCharacter);

	UFUNCTION()
	void SpawnAndPossessCharacter(const FCharacterStruct& Character);

	TArray<FCharacterStruct> AvailableCharacters;
	FCharacterStruct CurrentCharacter;

	void LoadCharacterData();
	void CreateCharacterButtons();
	void UpdateCharacterDisplay(const FCharacterStruct& Character);
	
	//void SwitchToLobbyCharacterCamera(const FCharacterStruct& Character);

public:
	UFUNCTION()
	void ReturnToMenu();
};
