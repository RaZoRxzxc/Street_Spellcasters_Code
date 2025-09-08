// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MapWidget.h"
#include "Actors/CampfireUpgrade.h"
#include "Components/Image.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateColor.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/PointOfInterestWidget.h"


void UMapWidget::UpdatePointOfInterests()
{

	float LocalSize = GetCachedGeometry().GetLocalSize().X;
    
	if (LocalSize > 0)
	{
		for (UPointOfInterestWidget* PointOfInterest : PointOfInterests)
		{
			if (PointOfInterest && PointOfInterest->OwnerActor)
			{
				FVector PlayerLoc = PointOfInterest->OwnerActor->GetActorLocation();
	
				auto OverlaySlot = PointOfInterest->OverlaySlot;
				
				float NormalizedX = ((PlayerLoc.X * -1.0f) / MapWidth) * LocalSize / CurrentScaleValue;
				float NormalizedY = (PlayerLoc.Y / MapWidth) * LocalSize / CurrentScaleValue;
                
				OverlaySlot->SetPadding(FMargin(NormalizedY, NormalizedX, 0, 0));
                
				auto ActorRot = PointOfInterest->OwnerActor->GetActorRotation().Yaw;
				PointOfInterest->SetRenderTransformAngle(ActorRot + 0.0f);
			}
		}
	}
}

void UMapWidget::RegisterCampfires(ACampfireUpgrade* Campfire)
{
	if (Campfire && !RegisteredCampfires.Contains(Campfire))
	{
		RegisteredCampfires.Add(Campfire);

		if (CampfireImage)
		{
			AddNewPOI(Campfire, CampfireImage, FVector2D(25, 25), FLinearColor::Red);
		}
	}
}

void UMapWidget::UpdateCampfires()
{
	UpdatePointOfInterests();
}

void UMapWidget::AddNewPOI(AActor* TrackActor, UTexture2D* Image, FVector2D ImageSize, FLinearColor SpecifiedColor)
{
	
	if (TrackActor && POIWidgetClass)
	{
		UPointOfInterestWidget* NewPOI = CreateWidget<UPointOfInterestWidget>(GetOwningPlayer(), POIWidgetClass);
		if (NewPOI)
		{
			UOverlaySlot* POIOverlaySlot = MainOverlay->AddChildToOverlay(NewPOI);
			PointOfInterests.Add(NewPOI);
            
			FSlateColor SlateColor(SpecifiedColor);
			NewPOI->Update(ImageSize, SlateColor, Image, POIOverlaySlot, TrackActor);
			
			POIOverlaySlot->SetVerticalAlignment(VAlign_Center);
			POIOverlaySlot->SetHorizontalAlignment(HAlign_Center);
			
			UpdatePointOfInterests();
		}
	}
}

FReply UMapWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetWheelDelta() > 0)
	{
		ZoomIn();
	}
	else
	{
		ZoomOut();
	}
    
	return FReply::Handled();
}

float UMapWidget::GetCurrentScale()
{
	// FWidgetTransform WidgetTransform = GetRenderTransform();
	//
	// FVector2D CurrentScale = WidgetTransform.Scale;
	// float X = CurrentScale.X;
	// return X;
	return CurrentScaleValue;
	
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

	UE_LOG(LogTemp, Warning, TEXT("Tick"));
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bCanDrag = false;
	
	if (!bDoOnce)
	{
		InitialSize = GetDesiredSize();
		if (MapTexture && MapImage)
		{
			FVector2D TextureSize = FVector2D(MapTexture->GetSizeX(), MapTexture->GetSizeY());
            
			FSlateBrush SlateBrush;
			SlateBrush.SetResourceObject(MapTexture);
			SlateBrush.ImageSize = TextureSize;
			MapImage->SetBrush(SlateBrush);
		}

		if (GetOwningPlayer())
		{
			OwnerPawn = GetOwningPlayer()->GetPawn();
			if (OwnerPawn && PlayerImage)
			{
				AddNewPOI(OwnerPawn, PlayerImage, FVector2D(24, 24), FLinearColor::Green);
			}
		}

		if (GetWorld())
		{
			TArray<AActor*> CampfireActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACampfireUpgrade::StaticClass(), CampfireActors);
            
			for (AActor* Actor : CampfireActors)
			{
				if (ACampfireUpgrade* Campfire = Cast<ACampfireUpgrade>(Actor))
				{
					RegisterCampfires(Campfire);
				}
			}
		}
		
		bDoOnce = true;
	}
	bDoOnce = true; 
}

void UMapWidget::NativeDestruct()
{

	bCanDrag = false;
	Super::NativeDestruct();
	
}

void UMapWidget::ZoomIn()
{
	SetZoom(CurrentScaleValue + ZoomSpeed);
}

void UMapWidget::ZoomOut()
{
	SetZoom(CurrentScaleValue - ZoomSpeed);
}

void UMapWidget::SetZoom(float NewScale)
{
	float OldScale = CurrentScaleValue;
	CurrentScaleValue = FMath::Clamp(NewScale, MinScale, MaxScale);
    
	// Применяем масштаб через Render Transform
	FWidgetTransform Transform;
	Transform.Scale = FVector2D(CurrentScaleValue, CurrentScaleValue);
    
	// Центрируем масштабирование
	FVector2D CurrentSize = GetCachedGeometry().GetLocalSize();
	Transform.Translation.X = (1.0f - CurrentScaleValue) * CurrentSize.X * 0.5f;
	Transform.Translation.Y = (1.0f - CurrentScaleValue) * CurrentSize.Y * 0.5f;
    
	SetRenderTransform(Transform);
	UpdatePointOfInterests();
}
