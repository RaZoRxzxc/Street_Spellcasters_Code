// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHUD.h"
#include "Widgets/MiniMapWidget.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/LevelUpMenuWidget.h"
#include "Components/SizeBox.h"
#include "Components/StatsComponent.h"
#include "Components/TextBlock.h"
#include "Widgets/CharacterSelectWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/InteractionWidget.h"
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

// Update souls amount when player killed an enemy
void APlayerHUD::UpdateSoulsPoints(int32 CurrentPoints)
{
	if (PlayerWidget)
	{
		PlayerWidget->SoulsPointsUpdate(CurrentPoints);
	}
}

// Hide health, stamina progress bars, level text and amount flasks
void APlayerHUD::HideStatsBox()
{
	if (PlayerWidget)
	{
		PlayerWidget->HideStats();
	}
}

// Show interact box 
void APlayerHUD::ShowInteractBox(FText NewUpgradeText)
{
	if (PlayerWidget)
	{
		PlayerWidget->InteractionWidget->OnInteractableOverlap(NewUpgradeText);
	}
}

// Hide interact box
void APlayerHUD::HideInteractBox()
{
	if (PlayerWidget)
	{
		PlayerWidget->InteractionWidget->Hide();
	}
}

// Show level up menu 
void APlayerHUD::ShowLevelUpMenu()
{
	if (PlayerWidget)
	{
		HideInteractBox();
		PlayerWidget->LevelUpMenu->SetVisibility(ESlateVisibility::Visible);

		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
		}
	}
}

void APlayerHUD::HideLevelUpMenu()
{
	//LevelUpWidget->RemoveFromParent();
	
	if (PlayerWidget)
	{
		PlayerWidget->LevelUpMenu->SetVisibility(ESlateVisibility::Collapsed);

		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

bool APlayerHUD::IsLevelMenuOpen() const
{
	return PlayerWidget && PlayerWidget->LevelUpMenu->IsVisible();
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
	
	if (MapWidget)
	{
		bIsMapOpen = !bIsMapOpen;

		if (bIsMapOpen)
		{
			MapWidget->Show();
			PC->SetInputMode(FInputModeGameAndUI());
			PC->bShowMouseCursor = true;
			
		}
		else
		{
			MapWidget->Hide();
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}
	}
}

void APlayerHUD::ShowCharacterSelectMenu()
{
	if (CharSelectWidgetClass)
	{
		CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), CharSelectWidgetClass);
		if (CharacterSelectWidget)
		{
			HideInteractBox();
			CharacterSelectWidget->AddToViewport();

			//HideCharacterSelectMenu();
			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void APlayerHUD::HideCharacterSelectMenu()
{
	if (IsCharSelectMenuOpen())
	{
		CharacterSelectWidget->RemoveFromParent();
		
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}

void APlayerHUD::ShowSelectMapWidget()
{
	if (SelectMapWidgetClass)
	{
		SelectMapWidget = CreateWidget<USelectMapWidget>(GetWorld(), SelectMapWidgetClass);
		if (SelectMapWidget)
		{
			HideInteractBox();
			SelectMapWidget->AddToViewport();

			APlayerController* PC = GetOwningPlayerController();
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
}

void APlayerHUD::HideSelectMapWidget()
{
	if (IsMapSelectMenuOpen())
	{
		SelectMapWidget->RemoveFromParent();
		
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}
