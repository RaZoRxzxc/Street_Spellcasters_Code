// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CharacterSelectWidget.h"
#include "Widgets/MenuWidget.h"
#include "Widgets/CharacterButtonWidget.h"
#include "Characters/BaseCharacter.h"
#include "Structs/CharacterStruct.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "GameFramework/PlayerStart.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/MenuHUD.h"

void UCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	LoadCharacterData();
	CreateCharacterButtons();

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->GetSelectedCharacter().Character)
	{
		CurrentCharacter = GameInstance->GetSelectedCharacter();
		UpdateCharacterDisplay(CurrentCharacter);
	}
	else if (AvailableCharacters.Num() > 0)
	{
		CurrentCharacter = AvailableCharacters[0];
		UpdateCharacterDisplay(CurrentCharacter);
	}
}

void UCharacterSelectWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UE_LOG(LogTemp, Warning, TEXT("CharacterSelectWidget is being destroyed!"));
}

void UCharacterSelectWidget::OnCharacterSelected(const FCharacterStruct& SelectedCharacter)
{
	CurrentCharacter = SelectedCharacter;
	UpdateCharacterDisplay(SelectedCharacter);
	
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetSelectedCharacter(SelectedCharacter);
	}

	SpawnAndPossessCharacter(SelectedCharacter);
}

void UCharacterSelectWidget::OnCharacterHovered(const FCharacterStruct& HoveredCharacter)
{
	bIsHovering = true;
	CurrentlyHoveredCharacter = HoveredCharacter;
	UpdateCharacterDisplay(HoveredCharacter);
}

void UCharacterSelectWidget::OnCharacterUnhovered(const FCharacterStruct& UnhoveredCharacter)
{
	if (CurrentlyHoveredCharacter.CharacterID == UnhoveredCharacter.CharacterID)
	{
		bIsHovering = false;
		UpdateCharacterDisplay(CurrentCharacter);
		UE_LOG(LogTemp, Warning, TEXT("Character Unhovered: %s - Returning to selected: %s"), 
			*UnhoveredCharacter.CharacterName.ToString(), 
			*CurrentCharacter.CharacterName.ToString());
	}
}

void UCharacterSelectWidget::SpawnAndPossessCharacter(const FCharacterStruct& Character)
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC || !Character.Character) return;

	UWorld* World = GetWorld();
	if (!World) return;

	APawn* CurrentPawn = PC->GetPawn();
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (CurrentPawn)
	{
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(CurrentPawn);
		if (PlayerCharacter && PlayerCharacter->CurrentWeapon)
		{
			PlayerCharacter->CurrentWeapon->Destroy();
			PlayerCharacter->CurrentWeapon = nullptr;
		}
		
		SpawnLocation = CurrentPawn->GetActorLocation();
		SpawnRotation = CurrentPawn->GetActorRotation();

		CurrentPawn->Destroy();
	}
	else
	{
		AActor* PlayerStart = UGameplayStatics::GetActorOfClass(World, APlayerStart::StaticClass());
		if (PlayerStart)
		{
			SpawnLocation = PlayerStart->GetActorLocation();
			SpawnRotation = PlayerStart->GetActorRotation();
		}
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	APawn* NewCharacter = World->SpawnActor<APawn>(Character.Character, SpawnLocation, SpawnRotation, SpawnParams);
	if (NewCharacter)
	{
		PC->Possess(NewCharacter);
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

void UCharacterSelectWidget::CreateCharacterButtons()
{
	if (!CharacterGridPanel || !CharacterButtonClass) return;

	CharacterGridPanel->ClearChildren();

	const int32 Columns = 3;
	int32 Row = 0;
	int32 Column = 0;

	for (int32 i = 0; i < AvailableCharacters.Num(); i++)
	{
		UCharacterButtonWidget* CharacterButton = CreateWidget<UCharacterButtonWidget>(this, CharacterButtonClass);
		if (CharacterButton)
		{
			CharacterButton->InitializeCharacter(AvailableCharacters[i]);
			CharacterButton->OnCharacterSelected.AddDynamic(this, &UCharacterSelectWidget::OnCharacterSelected);
			CharacterButton->OnCharacterHovered.AddDynamic(this, &UCharacterSelectWidget::OnCharacterHovered);
			CharacterButton->OnCharacterUnhovered.AddDynamic(this, &UCharacterSelectWidget::OnCharacterUnhovered);

			UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(CharacterButton);
			if (GridSlot)
			{
				GridSlot->SetRow(Row);
				GridSlot->SetColumn(Column);
			}

			Column++;
			if (Column >= Columns)
			{
				Column = 0;
				Row++;
			}
		}
	}
}

void UCharacterSelectWidget::UpdateCharacterDisplay(const FCharacterStruct& Character)
{
	if (CharacterNameText)
		CharacterNameText->SetText(Character.CharacterName);

	if (CharacterClassText)
		CharacterClassText->SetText(Character.CharacterClass);

	if (CharacterDescriptionText)
		CharacterDescriptionText->SetText(Character.CharacterDescription);

	if (CharacterIcon && Character.CharacterIcon)
		CharacterIcon->SetBrushFromTexture(Character.CharacterIcon);

	UpdateCharacterStats(Character);
}

void UCharacterSelectWidget::UpdateCharacterStats(const FCharacterStruct& Character)
{
	if (HealthProgressBar)
	{
		float HealthPercent = NormalizeStatValue(Character.BaseHealth, MaxHealthValue);
		HealthProgressBar->SetPercent(HealthPercent);
	}

	if (StaminaProgressBar)
	{
		float StaminaPercent = NormalizeStatValue(Character.BaseStamina, MaxStaminaValue);
		StaminaProgressBar->SetPercent(StaminaPercent);
	}

	if (DamageProgressBar)
	{
		float DamagePercent = NormalizeStatValue(Character.BaseDamage, MaxDamageValue);
		DamageProgressBar->SetPercent(DamagePercent);
	}
	
	if (HealthStatText)
	{
		FString HealthText = FString::Printf(TEXT("Health: %.0f"), Character.BaseHealth);
		HealthStatText->SetText(FText::FromString(HealthText));
	}

	if (StaminaStatText)
	{
		FString StaminaText = FString::Printf(TEXT("Stamina: %.0f"), Character.BaseStamina);
		StaminaStatText->SetText(FText::FromString(StaminaText));
	}

	if (DamageStatText)
	{
		FString DamageText = FString::Printf(TEXT("Damage: %.0f"), Character.BaseDamage);
		DamageStatText->SetText(FText::FromString(DamageText));
	}
}

