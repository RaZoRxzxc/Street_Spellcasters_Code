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

UCLASS()
class STREET_SPELLCASTERS_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	UDataTable* CharacterDataTable;

	FCharacterStruct CharacterStruct;

	UPROPERTY(meta = (BindWidget))
	UButton* PreviousButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NextButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectButton;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMenuWidget* MenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMenuWidget> MenuWidgetClass;
	
private:
	UFUNCTION()
	void OnPreviousClicked();

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnSelectClicked();

	TArray<FCharacterStruct> AvailableCharacters;
	int32 CurrentCharacterIndex;

	void LoadCharacterData();
	void UpdateCharacterDisplay();
	
	void SwitchToLobbyCharacterCamera();

public:
	UFUNCTION()
	void ReturnToMenuCamera();
};
