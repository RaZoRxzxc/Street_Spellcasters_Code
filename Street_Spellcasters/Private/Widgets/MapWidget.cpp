// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MapWidget.h"
#include "Actors/CampfireUpgrade.h"
#include "Actors/ZoneTree.h"
#include "Components/Image.h"
#include "Styling/SlateColor.h"
#include "Components/Overlay.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Structs/MapsStruct.h"
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

void UMapWidget::RegisterPOI(TSubclassOf<AActor> ActorClass, UTexture2D* Icon, FVector2D Size, FLinearColor Color)
{
	if (!GetWorld()) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (!RegisteredPOIACtors.Contains(Actor))
		{
			RegisteredPOIACtors.Add(Actor);

			AddNewPOI(Actor, Icon, Size, Color);
		}
	}
}

void UMapWidget::UpdateIcons()
{
	UpdatePointOfInterests();
}


void UMapWidget::UpdateMapForCurrentLevel()
{
	if (!GetWorld() || !MapsDataTable) 
        {
            UE_LOG(LogTemp, Warning, TEXT("World or MapsDataTable is null"));
            return;
        }
    
        FString FullLevelName = GetWorld()->GetMapName();
        FString LevelName = FPackageName::GetShortName(FullLevelName);
        CurrentLevelName = FName(LevelName);
        UE_LOG(LogTemp, Log, TEXT("Current Level Name: %s"), *LevelName);
    
        TArray<FMapsStruct*> AllMaps;
        MapsDataTable->GetAllRows<FMapsStruct>(TEXT(""), AllMaps);
    
        bool bFound = false;
        for (FMapsStruct* MapData : AllMaps)
        {
            if (MapData && MapData->LevelName == CurrentLevelName)
            {
                UE_LOG(LogTemp, Log, TEXT("Found map data for level: %s"), *CurrentLevelName.ToString());
                bFound = true;
                if (MapData->MiniMapImage && MapImage)
                {
                    MapTexture = MapData->MiniMapImage;
                    FVector2D TextureSize = FVector2D(MapTexture->GetSizeX(), MapTexture->GetSizeY());
                    UE_LOG(LogTemp, Log, TEXT("Setting map texture, Size: %s"), *TextureSize.ToString());
    
                    FSlateBrush SlateBrush;
                    SlateBrush.SetResourceObject(MapTexture);
                    SlateBrush.ImageSize = TextureSize;
                    MapImage->SetBrush(SlateBrush);
                }
                MapWidth = MapData->MapWidth;
                SetRenderTransform(FWidgetTransform(FVector2D::ZeroVector, FVector2D(1.0f, 1.0f), FVector2D::ZeroVector, 0.0f));
                break;
            }
        }
    
        if (!bFound)
        {
            UE_LOG(LogTemp, Warning, TEXT("No map data found for level: %s"), *CurrentLevelName.ToString());
        }
}

void UMapWidget::SetMapsDataTable(UDataTable* NewDataTable)
{
	MapsDataTable = NewDataTable;
	if (MapsDataTable && GetWorld())
	{
		UpdateMapForCurrentLevel();
		
	}
}

void UMapWidget::ChangeMapForLevel(FName LevelName)
{
	if (!MapsDataTable) return;

	FMapsStruct* MapData = MapsDataTable->FindRow<FMapsStruct>(LevelName, TEXT(""));
	if (MapData)
	{
		CurrentLevelName = LevelName;

		if (MapData->MiniMapImage && MapImage)
		{
			MapTexture = MapData->MiniMapImage;
			FVector2D TextureSize = FVector2D(MapTexture->GetSizeX(), MapTexture->GetSizeY());

			FSlateBrush SlateBrush;
			SlateBrush.SetResourceObject(MapTexture);
			SlateBrush.ImageSize = TextureSize;
			MapImage->SetBrush(SlateBrush);
		}

		MapWidth = MapData->MapWidth;

		SetRenderTransform(FWidgetTransform(FVector2D::ZeroVector, FVector2D(1.0f, 1.0f), FVector2D::ZeroVector, 0.0f));

		if(OwnerPawn && PlayerImage)
		{
			AddNewPOI(OwnerPawn, PlayerImage, FVector2D(24, 24), FLinearColor::Green);
		}

		if (GetWorld())
		{
			RegisterPOI(ACampfireUpgrade::StaticClass(), CampfireImage, FVector2D(32, 32), FLinearColor::Red); // Campfires
			RegisterPOI(AZoneTree::StaticClass(), ZoneTreeImage, FVector2D(32, 32), FLinearColor::Yellow);	// Zone trees
		}
	}
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

	OnScaleChanged.Broadcast(NewScaleValue);
	
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
	
	if (GetWorld())
	{
		FString CurrentLevel = GetWorld()->GetMapName();
		FString ShortLevelName = FPackageName::GetShortName(CurrentLevel);
        
		if (CurrentLevelName != FName(ShortLevelName))
		{
			UE_LOG(LogTemp, Log, TEXT("Level changed to: %s"), *ShortLevelName);
			UpdateMapForCurrentLevel();
		}
	}
	
	UpdatePointOfInterests();

}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bCanDrag = false;
	
	if (!bDoOnce)
	{
		InitialSize = GetDesiredSize();
		
		if (GetWorld())
		{
			FName CurrentLevel = FName(GetWorld()->GetMapName());
			UpdateMapForCurrentLevel();
		}
        
		if (GetOwningPlayer())
		{
			OwnerPawn = GetOwningPlayer()->GetPawn();
			if (OwnerPawn && PlayerImage)
			{
				AddNewPOI(OwnerPawn, PlayerImage, FVector2D(24, 24), FLinearColor::Green);
			}
		}

		// Register all actor on the map
		if (GetWorld())
		{
			RegisterPOI(ACampfireUpgrade::StaticClass(), CampfireImage, FVector2D(25,25), FLinearColor::Red); // Campfires
			RegisterPOI(AZoneTree::StaticClass(), ZoneTreeImage, FVector2D(30,30), FLinearColor::Yellow); // Zone trees
		}
	}
	bDoOnce = true;
	
}

void UMapWidget::NativeDestruct()
{

	bCanDrag = false;
	Super::NativeDestruct();
	
}