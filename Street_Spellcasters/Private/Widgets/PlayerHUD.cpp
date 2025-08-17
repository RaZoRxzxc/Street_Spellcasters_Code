// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHUD.h"
#include "Weapon/BaseWeapon.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/LevelUpMenuWidget.h"
#include "Components/HealthComponent.h"
#include "Components/HorizontalBox.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/PlayerWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerWidgetClass)
	{
		PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
		if (PlayerWidget)
		{
			PlayerWidget->AddToViewport();
			
			if (const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
			{
				if (auto* StatsComp = PlayerPawn->FindComponentByClass<UStatsComponent>())
				{

					StatsComp->OnStatsChanged.AddDynamic(this, &APlayerHUD::UpdateStats);
					UpdateStats(StatsComp->GetHealthPercent(), StatsComp->GetStaminaPercent());
					
					StatsComp->OnSoulsPointsChanged.AddDynamic(this, &APlayerHUD::UpdateSoulsPoints);
					UpdateSoulsPoints(StatsComp->GetSoulsPoints());

					StatsComp->OnLevelChanged.AddDynamic(this, &APlayerHUD::UpdateLevel);
					UpdateLevel(StatsComp->Level);
				}
			}
		}
	}
}

void APlayerHUD::UpdateStats(float CurrentHealth, float CurrentStamina)
{
	if (PlayerWidget)
	{
		PlayerWidget->HealthUpdate(CurrentHealth);
		PlayerWidget->StaminaUpdate(CurrentStamina);
	}
}

void APlayerHUD::UpdateStamina(float CurrentStamina)
{
	if (PlayerWidget)
	{
		PlayerWidget->StaminaUpdate(CurrentStamina);
	}
}

void APlayerHUD::UpdateSoulsPoints(int32 CurrentPoints)
{
	if (PlayerWidget)
	{
		PlayerWidget->SoulsPointsUpdate(CurrentPoints);
	}
}

void APlayerHUD::ShowUpgradeBox()
{
	if (PlayerWidget)
	{
		PlayerWidget->UpgradeHorizontalBox->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideUpgradeBox()
{
	if (PlayerWidget)
	{
		PlayerWidget->UpgradeHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::ShowLevelUpMenu()
{
	if (WidgetClass)
	{
		LevelUpWidget = CreateWidget<ULevelUpMenuWidget>(GetWorld(), WidgetClass);
		if (LevelUpWidget)
		{
			if (const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
			{
				auto* StatsComp = PlayerPawn->FindComponentByClass<UStatsComponent>();
				LevelUpWidget->SetStatsComponent(StatsComp);
				LevelUpWidget->AddToViewport();
				HideUpgradeBox();
			}

			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void APlayerHUD::HideLevelUpMenu()
{
	LevelUpWidget->RemoveFromParent();
	ShowUpgradeBox();

	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

bool APlayerHUD::IsLevelMenuOpen() const
{
	return LevelUpWidget && LevelUpWidget->IsInViewport();
}

void APlayerHUD::UpdateLevel(int32 NewLevel)
{
	if (PlayerWidget)
	{
		PlayerWidget->LevelTextUpdate(NewLevel);
	}
}
