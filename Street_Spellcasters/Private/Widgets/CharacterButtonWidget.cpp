// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CharacterButtonWidget.h"
#include "Structs/CharacterStruct.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UCharacterButtonWidget::InitializeCharacter(const FCharacterStruct& NewCharacterData)
{
	CharacterData = NewCharacterData;

	if (CharacterIconImage && CharacterData.CharacterIcon)
	{
		CharacterIconImage->SetBrushFromTexture(CharacterData.CharacterIcon);
	}
}

void UCharacterButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectCharacterButton)
		SelectCharacterButton->OnClicked.AddDynamic(this, &UCharacterButtonWidget::OnSelectClicked);
}

void UCharacterButtonWidget::OnSelectClicked()
{
	OnCharacterSelected.Broadcast(CharacterData);
}
