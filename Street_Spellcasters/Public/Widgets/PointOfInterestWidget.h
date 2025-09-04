// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/OverlaySlot.h"
#include "Styling/SlateTypes.h"
#include "PointOfInterestWidget.generated.h"

/**
 * 
 */
UCLASS()
class STREET_SPELLCASTERS_API UPointOfInterestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void Update(FVector2D ImageSize, FSlateColor Tint, UObject* Image, UOverlaySlot* OverlayOb, AActor* ActorOwner);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UOverlaySlot* OverlaySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* OwnerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* MainImage;
	
};
