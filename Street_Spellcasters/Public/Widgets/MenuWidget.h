// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UCharacterSelectWidget;
class UButton;
class UTextBlock;

UCLASS()
class STREET_SPELLCASTERS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StartLevelName;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Fade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	UDataTable* MapsDataTable;
	
	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void OpenSettingsMenu();

	UFUNCTION(BlueprintCallable)
	void ExitGame();

private:
	

	UFUNCTION()
	void SelectMap(FName MapRowName);
};
