// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SelectMapWidget.h"
#include "Widgets/MapInfoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameInstance/MyGameInstance.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Widgets/MenuHUD.h"

void USelectMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartGameButton)
		StartGameButton->OnClicked.AddDynamic(this, &USelectMapWidget::StartGame);

	LoadAllMaps();
	CreateMapWidgets();

	if (AvailableMaps.Num() > 0)
	{
		UpdateMapInfo(AvailableMaps[0]);
	}
}

void USelectMapWidget::StartGame()
{
	if (SelectedMap.MapNameToLoad != NAME_None)
	{
		SetupInputMode(true);
		UGameplayStatics::OpenLevel(GetWorld(), SelectedMap.MapNameToLoad);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No map selected or map name is invalid"));
	}
}

void USelectMapWidget::OnMapSelected(const FMapsStruct& NewSelectedMap)
{
	SelectedMap = NewSelectedMap;
	UpdateMapInfo(NewSelectedMap);
}

void USelectMapWidget::UpdateMapInfo(const FMapsStruct& MapData)
{
	if (MapNameBoxText)
		MapNameBoxText->SetText(MapData.MapName);

	if (DescriptionText)
		DescriptionText->SetText(MapData.MapDescription);
}

void USelectMapWidget::LoadAllMaps()
{
	if (!MapDB) return;

	AvailableMaps.Empty();

	TArray<FMapsStruct*> AllMaps;
	MapDB->GetAllRows<FMapsStruct>(TEXT(""), AllMaps);

	for (auto* Map : AllMaps)
	{
		AvailableMaps.Add(*Map);
	}
}

void USelectMapWidget::CreateMapWidgets()
{
	if (!MapsScrollBox || !MapInfoWidgetClass) return;

	MapsScrollBox->ClearChildren();

	for (const FMapsStruct& MapData : AvailableMaps)
	{
		UMapInfoWidget* MapInfoWidget = CreateWidget<UMapInfoWidget>(this, MapInfoWidgetClass);
		if (MapInfoWidget)
		{
			MapInfoWidget->InitializeMapData(MapData);
			MapInfoWidget->OnMapSelected.AddDynamic(this, &USelectMapWidget::OnMapSelected);
			MapsScrollBox->AddChild(MapInfoWidget);
		}
	}
	
	if (AvailableMaps.Num() > 0)
	{
		SelectedMap = AvailableMaps[0];
		UpdateMapInfo(AvailableMaps[0]);
	}
}

void USelectMapWidget::SetupInputMode(bool bGameInput)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	if (bGameInput)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}
	else
	{
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetWidgetToFocus(TakeWidget());
        
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}
