// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

class UMenuWidget;
class UCharacterSelectWidget;
class USelectMapWidget;

UCLASS()
class STREET_SPELLCASTERS_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMenuWidget* MenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USelectMapWidget* SelectMapWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USelectMapWidget> SelectMapWidgetClass;
	
public:

	UFUNCTION()
	void ShowMenuWidget();

	UFUNCTION()
	void HideMenuWidget();

	UFUNCTION()
	void ShowMapSelectWidget();

	UFUNCTION()
	void HideMapSelectWidget();
};
