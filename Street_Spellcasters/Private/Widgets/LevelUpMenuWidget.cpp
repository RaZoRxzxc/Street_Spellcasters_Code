// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/LevelUpMenuWidget.h"
#include "Components/Button.h"
#include "Components/StatsComponent.h"
#include "Components/TextBlock.h"
#include "Weapon/BaseWeapon.h"

void ULevelUpMenuWidget::UpdateStatsMenu()
{
	if (!StatsComponent) return;

	// Set current player stats to upgrade menu
	CurrentHealthText->SetText(FText::FromString(FString::Printf(TEXT("Health: %.0f"), StatsComponent->MaxHealth)));
	CurrentStaminaText->SetText(FText::FromString(FString::Printf(TEXT("Stamina: %.0f"), StatsComponent->MaxStamina)));
	CurrentDamageText->SetText(FText::FromString(FString::Printf(TEXT("Damage: %.0f"), StatsComponent->Weapon->Damage)));
	CurrentLevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %.0d"), StatsComponent->Level)));

	// Next stats
	const float NextHealth = StatsComponent->MaxHealth + 50.0f;
	const float NextStamina = StatsComponent->MaxStamina + 25.0f;
	const float NextDamage = StatsComponent->Weapon->Damage + 10.0f;
	const float NextLevel = StatsComponent->Level + 1.0f;

	// Set next stats after level up
	NextHealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NextHealth)));
	NextStaminaText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NextStamina)));
	NextDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NextDamage)));
	NextLevelText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NextLevel)));

	// setting the required number of souls to level up
	SoulsToUpgradeText->SetText(FText::FromString(FString::Printf(TEXT("Souls needed: %d"), StatsComponent->SoulsToNextLevel)));
}

void ULevelUpMenuWidget::Upgrade()
{
	if (!StatsComponent) return;
	
	if (StatsComponent->GetSoulsPoints() >= StatsComponent->SoulsToNextLevel)
	{
		// Decrease souls for level up
		StatsComponent->AddPoints(-StatsComponent->SoulsToNextLevel);

		// level up
		StatsComponent->LevelUp();

		// Update stats menu
		UpdateStatsMenu();
	}
	else
	{
		SoulsToUpgradeText->SetText(FText::FromString(FString::Printf(TEXT("Not enough souls! Need %d"), StatsComponent->SoulsToNextLevel)));
	}
}

void ULevelUpMenuWidget::UpdateDamage(float Damage)
{
	CurrentDamageText->SetText(FText::FromString(FString::Printf(TEXT("Damage: %.0f"), Damage)));
	NextDamageText->SetText(FText::FromString(FString::Printf(TEXT("-> %.0f"), Damage + 10.0f)));
}

void ULevelUpMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Install on click function to button
	if (UpgradeButton)
	{
		UpgradeButton->OnClicked.AddDynamic(this, &ULevelUpMenuWidget::Upgrade);
	}

	if (StatsComponent)
	{
		// Update weapon damage 
		StatsComponent->OnDamageChanged.AddDynamic(this, &ULevelUpMenuWidget::UpdateDamage);

	}
	// Update stats menu
	UpdateStatsMenu();
}
