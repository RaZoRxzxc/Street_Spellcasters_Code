// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MiniMapWidget.h"
#include "Widgets/MapWidget.h"
#include "Components/Border.h"


void UMiniMapWidget::ZoomInMap()
{
	if (MapWidget)
	{
		MapWidget->ZoomIn();
	}
}

void UMiniMapWidget::ZoomOutMap()
{
	if (MapWidget)
	{
		MapWidget->ZoomOut();
	}
}

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}
