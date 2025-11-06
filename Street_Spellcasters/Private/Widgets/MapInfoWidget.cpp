// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MapInfoWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Structs/MapsStruct.h"

void UMapInfoWidget::InitializeMapData(const FMapsStruct& NewMapData)
{
	MapData = NewMapData;

	if (MapName)
		MapName->SetText(NewMapData.MapName);
	
	if (MapImage)
		MapImage->SetBrushFromTexture(NewMapData.MapIcon);
	
}

void UMapInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectMapButton)
	{
		SelectMapButton->OnClicked.AddDynamic(this, &UMapInfoWidget::OnSelectClicked);
	}
	
}

void UMapInfoWidget::OnSelectClicked()
{
	OnMapSelected.Broadcast(MapData);
}

