// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UProgressBar;
class USlider;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class STREET_SPELLCASTERS_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION()
	void HealthUpdate(const float CurrentHealth);

	virtual void NativePreConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelUp", meta = (BindWidget))
	class ULevelUpMenuWidget* LevelUpMenu;

	UFUNCTION()
	void HideStats();
protected:
	UFUNCTION()
	void SetStatsComponent(class UStatsComponent* StatsComp);
	
	FTimerHandle BackHealthTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (BindWidget))
	UHorizontalBox* StatsHorBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (BindWidget))
	UProgressBar* BackHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (BindWidget))
	UProgressBar* FrontHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (BindWidget))
	float AlphaFloat = 0.925f;
	
	UFUNCTION()
	void BackHealthBarUpdate();
	
public:

	UFUNCTION()
	void StaminaUpdate(const float CurrentStamina);

protected:

	FTimerHandle StaminaTimer;
	
	UFUNCTION()
	void BackStaminaUpdate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (BindWidget))
	UProgressBar* FrontStaminaBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina", meta = (BindWidget))
	UProgressBar* BackStaminaBar;

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Souls", meta = (BindWidget))
	UTextBlock* SoulsPointsText;

public:

	UFUNCTION()
	void SoulsPointsUpdate(const int32 CurrentPoints);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (BindWidget))
	class USizeBox* InteractBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (BindWidget))
	UTextBlock* InteractText;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (BindWidget))
	UTextBlock* LevelText;
	
public:
	UFUNCTION()
	void LevelTextUpdate(int32 NewLevel);

	UFUNCTION()
	void FlasksTextUpdate(int32 NewFlasks);
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flasks", meta = (BindWidget))
	UTextBlock* FlaskText;
	
};
