// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

class UMenuWidget;
class UCharacterSelectWidget;

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
	UCharacterSelectWidget* CharacterSelectWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCharacterSelectWidget> CharSelectWidgetClass;
	
public:
	
	UFUNCTION()
	void ShowCharacterSelectMenu();

	UFUNCTION()
	void HideCharacterSelectMenu();

	UFUNCTION()
	void ShowMenuWidget();

	UFUNCTION()
	void HideMenuWidget();
};
