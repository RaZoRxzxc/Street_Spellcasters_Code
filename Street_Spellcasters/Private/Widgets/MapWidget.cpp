// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MapWidget.h"

#include "Components/Image.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateColor.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/PointOfInterestWidget.h"


void UMapWidget::UpdatePointOfInterests()
{
	float LocalSize = GetCachedGeometry().GetLocalSize().X;
	UE_LOG(LogTemp, Warning, TEXT("LocalSize: %f, POI Count: %d"), LocalSize, PointOfInterests.Num());
    
	if (LocalSize > 0)
	{
		for (UPointOfInterestWidget* PointOfInterest : PointOfInterests)
		{
			if (PointOfInterest && PointOfInterest->OwnerActor)
			{
				FVector PlayerLoc = PointOfInterest->OwnerActor->GetActorLocation();
				UE_LOG(LogTemp, Warning, TEXT("Player Location: X=%f, Y=%f, Z=%f"), 
					  PlayerLoc.X, PlayerLoc.Y, PlayerLoc.Z);
				auto OverlaySlot = PointOfInterest->OverlaySlot;
                
				// Правильное преобразование мировых координат в координаты виджета
				float NormalizedX = (PlayerLoc.X / MapWidth) * LocalSize;
				float NormalizedY = (PlayerLoc.Y / MapWidth) * LocalSize;
                
				// Центрирование относительно размера виджета POI
				if (PointOfInterest->MainImage)
				{
					FVector2D POISize = PointOfInterest->MainImage->GetDesiredSize();
					NormalizedX -= POISize.X * 0.5f;
					NormalizedY -= POISize.Y * 0.5f;
				}
                
				OverlaySlot->SetPadding(FMargin(NormalizedX, NormalizedY, 0, 0));
                
				// Поворот (если нужно)
				PointOfInterest->SetRenderTransformAngle(PlayerLoc.Z);
			}
		}
	}
	
}

void UMapWidget::AddNewPOI(AActor* TrackActor, UTexture2D* Image, FVector2D ImageSize, FLinearColor SpecifiedColor)
{
	if (TrackActor)
	{
		POIWidget = CreateWidget<UPointOfInterestWidget>(GetOwningPlayer(), POIWidgetClass);
		if (POIWidget)
		{
			auto POIOverlaySlot = MainOverlay->AddChildToOverlay(POIWidget);
			PointOfInterests.Add(POIWidget);
			
			FSlateColor SlateColor(SpecifiedColor);
			
			POIWidget->Update(ImageSize,SlateColor , Image, POIOverlaySlot, TrackActor);
			
			POIOverlaySlot->SetVerticalAlignment(VAlign_Center);
			POIOverlaySlot->SetHorizontalAlignment(HAlign_Center);
		}
	}
	
}

float UMapWidget::GetCurrentScale()
{
	FWidgetTransform WidgetTransform = GetRenderTransform();
	
	FVector2D CurrentScale = WidgetTransform.Scale;
	float X = CurrentScale.X;
	return X;
	
}

float UMapWidget::GetClampValue()
{
	auto Size = GetCachedGeometry().GetLocalSize().X * 0.9f;
	auto Scale = GetCurrentScale() - 1.0f;
	
	return Size * Scale;

}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdatePointOfInterests();
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bCanDrag = false;
	
	if (!bDoOnce)
	{
		FSlateBrush SlateBrush;
		SlateBrush.SetResourceObject(MapTexture);
		SlateBrush.ImageSize = FVector2D(1024, 1024);
		MapImage->SetBrush(SlateBrush);
	
		FLinearColor LinearColor;
		LinearColor.FromSRGBColor(FColor::Green);
		
		if (GetOwningPlayer())
		{
			OwnerPawn = GetOwningPlayer()->GetPawn();
			if (OwnerPawn)
			{
				AddNewPOI(OwnerPawn, PlayerImage, FVector2D(50, 50), LinearColor);
				UE_LOG(LogTemp, Warning, TEXT("POI Added for player"));
			}
		}
	}
	bDoOnce = true; 
}

void UMapWidget::NativeDestruct()
{

	for (UPointOfInterestWidget* POI : PointOfInterests)
	{
		if (POI)
		{
			POI->RemoveFromParent();
		}
	}
	PointOfInterests.Empty();

	bCanDrag = false;
	Super::NativeDestruct();
	
}
