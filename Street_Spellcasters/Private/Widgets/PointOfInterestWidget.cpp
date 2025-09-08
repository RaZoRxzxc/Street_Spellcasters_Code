// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PointOfInterestWidget.h"
#include "Styling/SlateBrush.h" 
#include "Components/Image.h"

void UPointOfInterestWidget::Update(FVector2D ImageSize, FSlateColor Tint, UObject* Image, UOverlaySlot* OverlayOb,
                                    AActor* ActorOwner)
{
	if (MainImage)
	{
		OverlaySlot = OverlayOb;
		OwnerActor = ActorOwner;
		
		FSlateBrush SlateBrush;
		SlateBrush.ImageSize = ImageSize;
		SlateBrush.TintColor = Tint;
		
		SlateBrush.SetResourceObject(Image);
		SlateBrush.DrawAs = ESlateBrushDrawType::Image;
		
		MainImage->SetBrush(SlateBrush);

		UE_LOG(LogTemp, Warning, TEXT("Update POI"));
	}
}
