// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EnemyWidget.h"

#include "Components/ProgressBar.h"

void UEnemyWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	GetWorld()->GetTimerManager().SetTimer(BackHealthTimer, this, &UEnemyWidget::UpdateBackHealthBar, 0.016667f, true);
}

void UEnemyWidget::UpdateBackHealthBar()
{
	if (BackHealthbar)
	{
		auto BackHealth = FMath::Lerp(BackHealthbar->GetPercent(), FrontHealthbar->GetPercent(), 1 - AlphaFloat);
		BackHealthbar->SetPercent(BackHealth);
	}
}

void UEnemyWidget::UpdateFrontHealthBar(float Health)
{
	if (FrontHealthbar)
	{
		FrontHealthbar->SetPercent(Health);
	}
}
