// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CampfireUpgrade.h"
#include "Blueprint/UserWidget.h"
#include "Actors/ZoneActor.h"
#include "MapWidget.generated.h"

class UTexture2D;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScaleChanged, float, NewScale);
UCLASS()
class STREET_SPELLCASTERS_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Map")
	FOnScaleChanged OnScaleChanged;
	
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
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMinScale() const { return MinScale; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxScale() const { return MaxScale; }
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxScale = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentScaleValue = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZoomSpeed = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D InitialSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D ZoomPivot;
        
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	bool bZoomToCursor = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AZoneActor* ZoneRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UPointOfInterestWidget*> PointOfInterests;
	
	UPROPERTY()
	TArray<AActor*> RegisteredPOIACtors;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanDrag;

	bool bDoOnce = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D LastKnowLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* OwnerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* MapImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	UDataTable* MapsDataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POI images")
	UTexture2D* PlayerImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POI images")
	UTexture2D* CampfireImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POI images")
	UTexture2D* ZoneTreeImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* MainOverlay;
	
	UFUNCTION()
	void UpdatePointOfInterests();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPointOfInterestWidget* POIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UPointOfInterestWidget> POIWidgetClass;

	UFUNCTION()
	void RegisterPOI(TSubclassOf<AActor> ActorClass, UTexture2D* Icon, FVector2D Size, FLinearColor Color);

	UFUNCTION()
	void UpdateIcons();

	UFUNCTION()
	void UpdateMapForCurrentLevel();

public:
	UFUNCTION(BlueprintCallable)
	void SetMapsDataTable(UDataTable* NewDataTable);

	UFUNCTION(BlueprintCallable)
	void ChangeMapForLevel(FName LevelName);
	
	UFUNCTION()
	void AddNewPOI(AActor* TrackActor, UTexture2D* Image, FVector2D ImageSize, FLinearColor SpecifiedColor);

	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override { return FReply::Unhandled(); }
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override {return FReply::Unhandled(); }
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override { return FReply::Unhandled(); }
	
	UFUNCTION()
	AZoneActor* SetZoneActor(AZoneActor* Zone) { return ZoneRef = Zone;}
	
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
