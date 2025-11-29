// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerWidget.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Widgets/LevelUpMenuWidget.h"
#include "Components/TextBlock.h"

void UPlayerWidget::HealthUpdate(const float CurrentHealth)
{
	if (FrontHealthBar)
	{
		FrontHealthBar->SetPercent(CurrentHealth);
	}
}

void UPlayerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UPlayerWidget::HideStats()
{
	if (StatsHorBox)
	{
		StatsHorBox->SetVisibility(ESlateVisibility::Hidden);
		SoulsPointsText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerWidget::SetStatsComponent(class UStatsComponent* StatsComp)
{
	if (LevelUpMenu && StatsComp)
	{
		LevelUpMenu->SetStatsComponent(StatsComp);
		LevelUpMenu->UpdateStatsMenu();
	}
}

void UPlayerWidget::BackHealthBarUpdate()
{
	if (BackHealthBar)
	{
		auto BackHealth = FMath::Lerp(BackHealthBar->GetPercent(), FrontHealthBar->GetPercent(), 1 - AlphaFloat);
		BackHealthBar->SetPercent(BackHealth);
	}
}

void UPlayerWidget::StaminaUpdate(const float CurrentStamina)
{
	if (FrontStaminaBar)
	{
		FrontStaminaBar->SetPercent(CurrentStamina);
	}
	
}

void UPlayerWidget::BackStaminaUpdate()
{
	if (BackStaminaBar)
	{
		auto BackStamina = FMath::Lerp(BackStaminaBar->GetPercent(), FrontStaminaBar->GetPercent(), 1 - AlphaFloat);
		BackStaminaBar->SetPercent(BackStamina);
	}
}

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimer(BackHealthTimer, this, &UPlayerWidget::BackHealthBarUpdate, 0.016667f, true);
	
	GetWorld()->GetTimerManager().SetTimer(StaminaTimer, this, &UPlayerWidget::BackStaminaUpdate, 0.016667f, true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			auto* StatsComp = PlayerPawn->FindComponentByClass<UStatsComponent>();
			SetStatsComponent(StatsComp);
		}
	}, 0.1f, false); 
}

void UPlayerWidget::SoulsPointsUpdate(const int32 CurrentPoints)
{
	if (SoulsPointsText)
	{
		AddedSouls = CurrentPoints;

		HiddenSouls = AddedSouls + HiddenSouls;
		GetWorld()->GetTimerManager().SetTimer(SoulsTimer, [this]
		{
			if (HiddenSouls != CurrentSoulsVisible)
			{
				CurrentSoulsVisible = CurrentSoulsVisible + 1.0f;
				
				SoulsPointsText->SetText(FText::AsNumber(CurrentSoulsVisible));
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(SoulsTimer);
			}
			
		}, GetWorld()->GetDeltaSeconds(), true);
	}
}

void UPlayerWidget::LevelTextUpdate(int32 NewLevel)
{
	
	LevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewLevel)));
}

void UPlayerWidget::FlasksTextUpdate(int32 NewFlasks)
{
	FlaskText->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewFlasks)));
}



