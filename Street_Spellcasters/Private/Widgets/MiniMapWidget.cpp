// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MiniMapWidget.h"
#include "Widgets/MapWidget.h"
#include "Components/Border.h"
#include "Components/Slider.h"

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ZoomSlider && MapWidget)
	{
		ZoomSlider->SetMinValue(MapWidget->GetMinScale());
		ZoomSlider->SetMaxValue(MapWidget->GetMaxScale());
		
		ZoomSlider->SetValue(MapWidget->GetCurrentScale());
		
		ZoomSlider->OnValueChanged.AddDynamic(this, &UMiniMapWidget::OnZoomSliderChanged);

		MapWidget->OnScaleChanged.AddDynamic(this, &UMiniMapWidget::OnMapScaleChanged);
	}
}

void UMiniMapWidget::OnZoomSliderChanged(float Value)
{
	if (MapWidget)
	{
		MapWidget->SetRenderScale(FVector2D(Value, Value));
		
		if (Value <= 1.0f + KINDA_SMALL_NUMBER)
		{
			MapWidget->SetRenderTranslation(FVector2D::ZeroVector);
		}
	}
}

void UMiniMapWidget::OnMapScaleChanged(float NewScale)
{
	if (ZoomSlider)
	{
		ZoomSlider->SetValue(NewScale);
	}
}
