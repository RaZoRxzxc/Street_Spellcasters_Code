// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MenuHUD.h"
#include "Widgets/MenuWidget.h"
#include "Widgets/CharacterSelectWidget.h"

// Show character select menu
void AMenuHUD::ShowCharacterSelectMenu()
{
	if (CharSelectWidgetClass)
	{
		CharacterSelectWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), CharSelectWidgetClass);
		if (CharacterSelectWidget)
		{
			HideMenuWidget();
			CharacterSelectWidget->AddToViewport();
		}
	}
}

void AMenuHUD::HideCharacterSelectMenu()
{
	if (CharacterSelectWidget->IsVisible())
	{
		CharacterSelectWidget->RemoveFromParent();
	}
}

// Show menu widget
void AMenuHUD::ShowMenuWidget()
{
	if (MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			HideCharacterSelectMenu();
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
