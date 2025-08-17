// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpMenuWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UButton;
class UStatsComponent;

UCLASS()
class STREET_SPELLCASTERS_API ULevelUpMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//  Level up Vertical box
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UVerticalBox* LevelUpMenu;

	// Current player health text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UTextBlock* CurrentHealthText;

	// Next player health text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UTextBlock* NextHealthText;

	// Current player stamina text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UTextBlock* CurrentStaminaText;

	// Next player stamina text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UTextBlock* NextStaminaText;

	// Souls to upgrade text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UTextBlock* SoulsToUpgradeText;

	// Current weapon damage text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UTextBlock* CurrentDamageText;

	// Next weapon damage text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UTextBlock* NextDamageText;

	// Upgrade button
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "LevelUpMenu", meta = (BindWidget))
	UButton* UpgradeButton;

	// Update stats on level up menu
	UFUNCTION()
	void UpdateStatsMenu();

	// Upgrade player stats
	UFUNCTION()
	void Upgrade();

	// Set Stats component
	void SetStatsComponent(UStatsComponent* StatsComp) { StatsComponent = StatsComp; }

	// Update damage 
	UFUNCTION()
	void UpdateDamage(float Damage);
	
protected:

	// Native construct
	virtual void NativeConstruct() override;

private:
	// Player stats component 
	UPROPERTY()
	UStatsComponent* StatsComponent;
};
