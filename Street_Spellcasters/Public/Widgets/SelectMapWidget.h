// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/MapsStruct.h"
#include "Blueprint/UserWidget.h"
#include "SelectMapWidget.generated.h"

class UButton;
class UScrollBox;
class UMapInfoWidget;
class UVerticalBox;
class UDataTable;
class UTextBlock;

UCLASS()
class STREET_SPELLCASTERS_API USelectMapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* MapsScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* MapImage;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* InfoBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapNameBoxText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	UDataTable* MapDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UMapInfoWidget> MapInfoWidgetClass;

private:

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void OnMapSelected(const FMapsStruct& NewSelectedMap);

	void UpdateMapInfo(const FMapsStruct& MapData);

	void LoadAllMaps();
	void CreateMapWidgets();
	void SetupInputMode(bool bGameInput);

	TArray<FMapsStruct> AvailableMaps;
	FMapsStruct SelectedMap;
	UMapInfoWidget* SelectedMapWidget;
};
