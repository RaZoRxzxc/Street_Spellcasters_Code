// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/CharacterStruct.h"
#include "CharacterButtonWidget.generated.h"

class UDataTable;
class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, const FCharacterStruct&, SelectedCharacter);
UCLASS()
class STREET_SPELLCASTERS_API UCharacterButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void InitializeCharacter(const FCharacterStruct& NewCharacterData);
	
	FOnCharacterSelected OnCharacterSelected;

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SelectCharacterButton;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIconImage;

private:
	UFUNCTION()
	void OnSelectClicked();
	
	FCharacterStruct CharacterData;
};
