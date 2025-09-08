// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerWidget;
class ULevelUpMenuWidget;
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
	void ShowUpgradeBox();
	
	UFUNCTION()
	void HideUpgradeBox();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	ULevelUpMenuWidget* LevelUpWidget;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	// TSubclassOf<UUserWidget> WidgetClass;
	
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

	bool bIsMapOpen = false;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// UTexture2D* EnemyImage;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enemy")
	// TSubclassOf<ABaseEnemyCharacter> Enemyclass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Enemy")
	TArray<AActor*> EnemyArray;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMiniMapWidget* MapWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MapWidgetClass;

	UFUNCTION()
	void ToggleMap();
};
