// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterSelectWidget.h"
#include "SelectMapWidget.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerWidget;
class ULevelUpMenuWidget;
class UCharacterSelectWidget;
class ABaseEnemyCharacter;

UCLASS()
class STREET_SPELLCASTERS_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	// Player Widget 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UPlayerWidget* PlayerWidget;
private:
	
	virtual void BeginPlay() override;

	// Update Player Health
	UFUNCTION()
	void UpdateStats(float CurrentHealth, float CurrentStamina);

	// Update Player Stamina
	UFUNCTION()
	void UpdateStamina(float CurrentStamina);

	// Update Player Souls
	UFUNCTION()
	void UpdateSoulsPoints(int32 CurrentPoints);

public:

	UFUNCTION()
	void HideStatsBox();
	
	UFUNCTION()
	void ShowInteractBox(FText NewUpgradeText);
	
	UFUNCTION()
	void HideInteractBox();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	ULevelUpMenuWidget* LevelUpWidget;
	
public:

	UFUNCTION()
	void ShowLevelUpMenu();

	UFUNCTION()
	void HideLevelUpMenu();
	
	UFUNCTION()
	bool IsLevelMenuOpen() const;

protected:

	UFUNCTION()
	void UpdateLevel(int32 NewLevel);

	UFUNCTION()
	void UpdateFlasksAmount(int32 NewFlasks);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enemy")
	TArray<AActor*> EnemyArray;
	
public:

	// ====================== Mini map widget ======================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMiniMapWidget* MapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MapWidgetClass;

	UFUNCTION()
	void ToggleMap();

protected:
	bool bIsMapOpen = false;
	
	// ===================== Character select widget ===================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterSelectWidget* CharacterSelectWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCharacterSelectWidget> CharSelectWidgetClass;

public:

	UFUNCTION()
	bool IsCharSelectMenuOpen() const { return CharacterSelectWidget && CharacterSelectWidget->IsInViewport(); }
	
	UFUNCTION()
	void ShowCharacterSelectMenu();

	UFUNCTION()
	void HideCharacterSelectMenu();

protected:

	// ================== Map select widget =====================
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USelectMapWidget* SelectMapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USelectMapWidget> SelectMapWidgetClass;

public:

	UFUNCTION()
	bool IsMapSelectMenuOpen() const { return SelectMapWidget && SelectMapWidget->IsInViewport(); }

	UFUNCTION()
	void ShowSelectMapWidget();

	UFUNCTION()
	void HideSelectMapWidget(); 
	
};
