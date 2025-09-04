// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHUD.h"
#include "Widgets/MiniMapWidget.h"
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

					StatsComp->OnFlasksChanged.AddDynamic(this, &APlayerHUD::UpdateFlasksAmount);
					UpdateFlasksAmount(StatsComp->GetFlasksAmount());
				}
			}
		}
	}

	if (MapWidgetClass)
	{
		MapWidget = CreateWidget<UMiniMapWidget>(GetWorld(), MapWidgetClass);
		if (MapWidget)
		{
			MapWidget->AddToViewport();
			MapWidget->SetVisibility(ESlateVisibility::Collapsed);
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

void APlayerHUD::UpdateFlasksAmount(int32 NewFlasks)
{
	if (PlayerWidget)
	{
		PlayerWidget->FlasksTextUpdate(NewFlasks);
	}
}

void APlayerHUD::ToggleMap()
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (!MapWidget && MapWidgetClass)
	{
		MapWidget = CreateWidget<UMiniMapWidget>(PC, MapWidgetClass);
		if (MapWidget)
		{
			MapWidget->AddToViewport();
			MapWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (MapWidget)
	{
		bIsMapOpen = !bIsMapOpen;

		if (bIsMapOpen)
		{
			MapWidget->SetVisibility(ESlateVisibility::Visible);
			PC->SetInputMode(FInputModeGameAndUI());
			PC->bShowMouseCursor = true;
			
		}
		else
		{
			MapWidget->SetVisibility(ESlateVisibility::Collapsed);
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}
	}
}
