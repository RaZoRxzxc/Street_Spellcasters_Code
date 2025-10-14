// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CharacterButtonWidget.h"
#include "Structs/CharacterStruct.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UCharacterButtonWidget::InitializeCharacter(const FCharacterStruct& NewCharacterData)
{
	CharacterData = NewCharacterData;

	if (SelectCharacterButton)
	{
		FButtonStyle ButtonStyle = SelectCharacterButton->GetStyle();
		
		if (CharacterData.CharacterIcon)
		{
			FSlateBrush NormalBrush = ButtonStyle.Normal;
			NormalBrush.SetResourceObject(CharacterData.CharacterIcon);
			ButtonStyle.SetNormal(NormalBrush);
			ButtonStyle.SetHovered(NormalBrush);
		}
		SelectCharacterButton->SetStyle(ButtonStyle);
	}
}

void UCharacterButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectCharacterButton)
	{
		SelectCharacterButton->OnClicked.AddDynamic(this, &UCharacterButtonWidget::OnSelectClicked);
		SelectCharacterButton->OnHovered.AddDynamic(this, &UCharacterButtonWidget::OnButtonHovered);
		SelectCharacterButton->OnUnhovered.AddDynamic(this, &UCharacterButtonWidget::OnButtonUnhovered);
	}
}

void UCharacterButtonWidget::OnSelectClicked()
{
	OnCharacterSelected.Broadcast(CharacterData);
}

void UCharacterButtonWidget::OnButtonHovered()
{
	OnCharacterHovered.Broadcast(CharacterData);
}

void UCharacterButtonWidget::OnButtonUnhovered()
{
	OnCharacterUnhovered.Broadcast(CharacterData);
}
