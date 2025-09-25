// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MapWidget.h"
#include "Actors/CampfireUpgrade.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

				float NormalizedX = ((PlayerLoc.X * -1.0f) / MapWidth) * LocalSize;
				float NormalizedY = (PlayerLoc.Y / MapWidth) * LocalSize;
				
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
	 FWidgetTransform CurrentTransform = GetRenderTransform();
    float OldScale = CurrentTransform.Scale.X;
	
    float WheelDelta = InMouseEvent.GetWheelDelta();
    float NewScaleValue = OldScale + (WheelDelta * ZoomSpeed);
    NewScaleValue = FMath::Clamp(NewScaleValue, MinScale, MaxScale);
    
    if (bZoomToCursor && FMath::Abs(NewScaleValue - OldScale) > 0.001f)
    {
        FVector2D LocalCursorPos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
        FVector2D WidgetSize = InGeometry.GetLocalSize();
    	
        FVector2D NormalizedCursorPos = (LocalCursorPos / WidgetSize) - FVector2D(0.5f, 0.5f);
    	
        FVector2D OffsetChange = NormalizedCursorPos * (NewScaleValue - OldScale) * WidgetSize;
    	
        FVector2D NewTranslation = CurrentTransform.Translation - OffsetChange;
    	
        float MaxOffsetX = (WidgetSize.X * (NewScaleValue - 1.0f)) / 2.0f;
        float MaxOffsetY = (WidgetSize.Y * (NewScaleValue - 1.0f)) / 2.0f;
        
        NewTranslation.X = FMath::Clamp(NewTranslation.X, -MaxOffsetX, MaxOffsetX);
        NewTranslation.Y = FMath::Clamp(NewTranslation.Y, -MaxOffsetY, MaxOffsetY);
        
        SetRenderTransform(FWidgetTransform(NewTranslation, FVector2D(NewScaleValue, NewScaleValue), CurrentTransform.Shear, CurrentTransform.Angle));
    }
    else
    {
        SetRenderScale(FVector2D(NewScaleValue, NewScaleValue));
    	
        if (NewScaleValue <= 1.0f + KINDA_SMALL_NUMBER)
        {
            SetRenderTranslation(FVector2D::ZeroVector);
        }
        else
        {
            FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();
            float MaxOffsetX = (WidgetSize.X * (NewScaleValue - 1.0f)) / 2.0f;
            float MaxOffsetY = (WidgetSize.Y * (NewScaleValue - 1.0f)) / 2.0f;
            
            FVector2D CurrentTranslation = CurrentTransform.Translation;
            CurrentTranslation.X = FMath::Clamp(CurrentTranslation.X, -MaxOffsetX, MaxOffsetX);
            CurrentTranslation.Y = FMath::Clamp(CurrentTranslation.Y, -MaxOffsetY, MaxOffsetY);
            
            SetRenderTranslation(CurrentTranslation);
        }
    }
    
    return FReply::Handled();

}

FReply UMapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	LastKnowLoc = InMouseEvent.GetScreenSpacePosition();
	auto LMB = InMouseEvent.GetEffectingButton();
	bCanDrag = UKismetMathLibrary::BooleanAND(LMB == EKeys::LeftMouseButton, GetCurrentScale() > 1.0f);
	
	return FReply::Handled();
	
}

FReply UMapWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bCanDrag = false;
	}
	
	return FReply::Handled();
}

FReply UMapWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bCanDrag)
	{
		FVector2D NewValue = InMouseEvent.GetScreenSpacePosition() - LastKnowLoc;
		
		FWidgetTransform CurrentTransform = GetRenderTransform();
		FVector2D NewTranslation = CurrentTransform.Translation + NewValue;
		
		FVector2D WidgetSize = GetCachedGeometry().GetLocalSize();
		float CurrentScale = GetCurrentScale();
        
		float MaxOffsetX = (WidgetSize.X * (CurrentScale - 1.0f)) / 2.0f;
		float MaxOffsetY = (WidgetSize.Y * (CurrentScale - 1.0f)) / 2.0f;
        
		NewTranslation.X = FMath::Clamp(NewTranslation.X, -MaxOffsetX, MaxOffsetX);
		NewTranslation.Y = FMath::Clamp(NewTranslation.Y, -MaxOffsetY, MaxOffsetY);
        
		SetRenderTranslation(NewTranslation);
		LastKnowLoc = InMouseEvent.GetScreenSpacePosition();
	}
    
	return FReply::Handled();
}

float UMapWidget::GetCurrentScale()
{
	return GetRenderTransform().Scale.X;
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
	}
	bDoOnce = true;
	
}

void UMapWidget::NativeDestruct()
{

	bCanDrag = false;
	Super::NativeDestruct();
	
}

