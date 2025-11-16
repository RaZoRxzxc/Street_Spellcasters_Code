// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class STREET_SPELLCASTERS_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

protected: 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D InputIconSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class USizeBox* ImageSizeBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* InputImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MessageText;

	// Animation
	UPROPERTY(Transient ,meta = (BindWidgetAnim))
	class UWidgetAnimation* FadeAnim;
	
public:
	
	UFUNCTION()
	void OnInteractableOverlap(FText InteractionText);

	UFUNCTION()
	void Hide();
	
protected:
	
	UFUNCTION()
	void Show();
};
