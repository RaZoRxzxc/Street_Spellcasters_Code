// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Actors/ZoneActor.h"
#include "MapWidget.generated.h"

class UTexture2D;
class UImage;

#define M_DrawCircle(Ptr)\
	

UCLASS()
class STREET_SPELLCASTERS_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MapWidth = 80000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ZoneMainCircleTint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ZoneNextCircleTint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CircleTickness = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Texture)
	UTexture2D* MapTexture;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AZoneActor* ZoneRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UPointOfInterestWidget*> PointOfInterests;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDrag;

	bool bDoOnce = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D LastKnowLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* OwnerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* MapImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* PlayerImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* MainOverlay;
	
	UFUNCTION()
	void UpdatePointOfInterests();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPointOfInterestWidget* POIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPointOfInterestWidget> POIWidgetClass;

	UFUNCTION()
	void AddNewPOI(AActor* TrackActor, UTexture2D* Image, FVector2D ImageSize, FLinearColor SpecifiedColor);

	//virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	UFUNCTION()
	AZoneActor* SetZoneActor(AZoneActor* Zone) { return Zone = ZoneRef;}
	
	UFUNCTION()
	AZoneActor* GetZoneActor() const {return ZoneRef;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCurrentScale();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetClampValue();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
