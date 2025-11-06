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
class UProgressBar;

UCLASS()
class STREET_SPELLCASTERS_API UCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	UDataTable* CharacterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UCharacterButtonWidget> CharacterButtonClass;

	FCharacterStruct CharacterStruct;

	// Character info
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterClassText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterDescriptionText;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIcon;

	// Character stats progress bars
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* DamageProgressBar;

	// Character stats texts
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthStatText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaStatText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageStatText;

	// Max value progress bars
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealthValue = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxStaminaValue = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxDamageValue = 100.0f;
	
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* CharacterGridPanel;

private:
	UFUNCTION()
	void OnCharacterSelected(const FCharacterStruct& SelectedCharacter);

	UFUNCTION()
	void OnCharacterHovered(const FCharacterStruct& HoveredCharacter);

	UFUNCTION()
	void OnCharacterUnhovered(const FCharacterStruct& UnhoveredCharacter);

	UFUNCTION()
	void SpawnAndPossessCharacter(const FCharacterStruct& Character);

	TArray<FCharacterStruct> AvailableCharacters;
	FCharacterStruct CurrentCharacter;
	FCharacterStruct CurrentlyHoveredCharacter;
	bool bIsHovering;

	void LoadCharacterData();
	void CreateCharacterButtons();
	void UpdateCharacterDisplay(const FCharacterStruct& Character);
	void UpdateCharacterStats(const FCharacterStruct& Character);
	float NormalizeStatValue(float Value, float MaxValue) const { return FMath::Clamp(Value / MaxValue, 0.0f, 1.0f);}
    
	//void SwitchToLobbyCharacterCamera(const FCharacterStruct& Character);
	
};
