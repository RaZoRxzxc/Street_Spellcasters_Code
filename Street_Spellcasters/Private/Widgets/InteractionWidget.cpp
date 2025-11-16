// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteractionWidget.h"

#include "Components/TextBlock.h"

void UInteractionWidget::OnInteractableOverlap(FText InteractionText)
{
	if (MessageText)
	{
		MessageText->SetText(InteractionText);
		Show();
	}
}

void UInteractionWidget::Hide()
{
	if (IsVisible())
	{
		QueuePlayAnimationReverse(FadeAnim);

		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]
		{
			SetVisibility(ESlateVisibility::Collapsed);
		}, 0.125f, false);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractionWidget::Show()
{
	PlayAnimation(FadeAnim);
	SetVisibility(ESlateVisibility::Visible);
}
