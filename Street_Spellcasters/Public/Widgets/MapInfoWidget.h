// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/MapsStruct.h"
#include "MapInfoWidget.generated.h"

class UDataTable;
class UButton;
class UTextBlock;
class UImage;
class UVerticalBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapSelected, const FMapsStruct&, SelectedMap);
UCLASS()
class STREET_SPELLCASTERS_API UMapInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeMapData(const FMapsStruct& NewMapData);
	
	FMapsStruct GetMapData() const { return MapData; }
	
	FOnMapSelected OnMapSelected;
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Data")
	UDataTable* MapDB;

	FMapsStruct MapsStruct;

	UPROPERTY(meta = (BindWidget))
	UButton* SelectMapButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapName;
	
	UPROPERTY(meta = (BindWidget))
	UImage* MapImage;

private:

	UFUNCTION()
	void OnSelectClicked();
	
	TArray<FMapsStruct> AvailableMaps;
	int32 CurrentMapindex;

	FMapsStruct MapData;
	
};
