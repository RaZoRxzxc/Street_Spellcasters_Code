// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapsStruct.generated.h"

USTRUCT(BlueprintType)
struct STREET_SPELLCASTERS_API FMapsStruct: public FTableRowBase
{
public:
	
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MapNameToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MapIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MapName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MapDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MiniMapImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MapWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLobby;
};
