// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CharacterSelectWidget.h"
#include "Widgets/MenuWidget.h"
#include "Actors/MenuCameraActor.h"
#include "Characters/LobbyCharacter.h"
#include "Structs/CharacterStruct.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/MenuHUD.h"

void UCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PreviousButton)
	{
		PreviousButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::OnPreviousClicked);
	}
	
	if (NextButton)
	{
		NextButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::OnNextClicked);
	}

	if (SelectButton)
	{
		SelectButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::OnSelectClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::ReturnToMenuCamera);
	}

	LoadCharacterData();
	CurrentCharacterIndex = 0;
	UpdateCharacterDisplay();
	SwitchToLobbyCharacterCamera();
}

void UCharacterSelectWidget::OnPreviousClicked()
{
	if (AvailableCharacters.Num() == 0) return;

	CurrentCharacterIndex = (CurrentCharacterIndex - 1 + AvailableCharacters.Num()) % AvailableCharacters.Num();
	UpdateCharacterDisplay();

	SwitchToLobbyCharacterCamera();
}

void UCharacterSelectWidget::OnNextClicked()
{
	if (AvailableCharacters.Num() == 0) return;

	CurrentCharacterIndex = (CurrentCharacterIndex + 1) % AvailableCharacters.Num();
	UpdateCharacterDisplay();

	SwitchToLobbyCharacterCamera();
}

void UCharacterSelectWidget::OnSelectClicked()
{
	if (AvailableCharacters.Num() == 0) return;

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetSelectedCharacter(AvailableCharacters[CurrentCharacterIndex]);
		UE_LOG(LogTemp, Warning, TEXT("Character selected: %s"), *AvailableCharacters[CurrentCharacterIndex].CharacterName.ToString());
	}
}

void UCharacterSelectWidget::LoadCharacterData()
{
	if (!CharacterDataTable) return;

	AvailableCharacters.Empty();

	TArray<FCharacterStruct*> AllCharacters;
	CharacterDataTable->GetAllRows<FCharacterStruct>(TEXT(""), AllCharacters);

	for (auto* Character : AllCharacters)
	{
		AvailableCharacters.Add(*Character);
	}
}

void UCharacterSelectWidget::UpdateCharacterDisplay()
{
	if (AvailableCharacters.Num() == 0) return;

	const FCharacterStruct& CurrentCharacter = AvailableCharacters[CurrentCharacterIndex];

	if (CharacterNameText)
	{
		CharacterNameText->SetText(CurrentCharacter.CharacterName);
	}

	if (CharacterClassText)
	{
		CharacterClassText->SetText(CurrentCharacter.CharacterClass);
	}

	if (CharacterDescriptionText)
	{
		CharacterDescriptionText->SetText(CurrentCharacter.CharacterDescription);
	}

	if (CharacterIcon && CurrentCharacter.CharacterIcon)
	{
		CharacterIcon->SetBrushFromTexture(CurrentCharacter.CharacterIcon);
	}
}

void UCharacterSelectWidget::SwitchToLobbyCharacterCamera()
{
	if (AvailableCharacters.Num() == 0) return;

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALobbyCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ALobbyCharacter* LobbyCharacter = Cast<ALobbyCharacter>(Actor);
		if (LobbyCharacter && LobbyCharacter->GetClass() == AvailableCharacters[CurrentCharacterIndex].LobbyCharacter)
		{
			PC->SetViewTargetWithBlend(LobbyCharacter, 1.0f);
			break;
		}
	}
}

void UCharacterSelectWidget::ReturnToMenuCamera()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		AActor* MenuCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AMenuCameraActor::StaticClass());
		if (MenuCamera)
		{
			PC->SetViewTargetWithBlend(MenuCamera, 1.0f);

			if (AMenuHUD* HUD = Cast<AMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				HUD->ShowMenuWidget();
			}
		}
	}
}

