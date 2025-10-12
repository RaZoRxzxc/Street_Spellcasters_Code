// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MenuHUD.h"
#include "Widgets/MenuWidget.h"
#include "Widgets/CharacterSelectWidget.h"
#include "Widgets/SelectMapWidget.h"


// Show menu widget
void AMenuHUD::ShowMenuWidget()
{
	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
}

void AMenuHUD::HideMenuWidget()
{
	if (MenuWidget->IsVisible())
	{
		MenuWidget->RemoveFromParent();
	}
}

void AMenuHUD::ShowMapSelectWidget()
{
	if (SelectMapWidgetClass)
	{
		SelectMapWidget = CreateWidget<USelectMapWidget>(GetWorld(), SelectMapWidgetClass);
		if (SelectMapWidget)
		{
			SelectMapWidget->AddToViewport();
		}
	}
}

void AMenuHUD::HideMapSelectWidget()
{
	if (SelectMapWidget->IsVisible())
	{
		SelectMapWidget->RemoveFromParent();
	}
}
