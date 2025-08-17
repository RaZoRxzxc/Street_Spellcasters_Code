// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

class UProgressBar;

UCLASS()
class STREET_SPELLCASTERS_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	FTimerHandle BackHealthTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* FrontHealthbar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* BackHealthbar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (BindWidget))
	float AlphaFloat = 0.925f;

	virtual void NativePreConstruct() override;
	
public:
	UFUNCTION()
	void UpdateBackHealthBar();

	UFUNCTION()
	void UpdateFrontHealthBar(float Health);
};
