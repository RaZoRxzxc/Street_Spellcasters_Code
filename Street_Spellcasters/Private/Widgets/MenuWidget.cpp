
#include "Widgets/MenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/CharacterSelectWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/MenuHUD.h"
#include "Structs/MapsStruct.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UMenuWidget::StartGame()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		// Set input mode and hide mouse cursor
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;

		// Play  start game animation
		PlayAnimation(Fade, EUMGSequencePlayMode::Forward);

		// Open lobby level through 0.4 seconds
		if (MapsDataTable)
		{
			SelectMap("Lobby");
		}
	}
}

void UMenuWidget::OpenSettingsMenu()
{
	
}

void UMenuWidget::ExitGame()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
	}
}

void UMenuWidget::SelectMap(FName MapRowName)
{
	FMapsStruct* MapData = MapsDataTable->FindRow<FMapsStruct>(MapRowName, TEXT(""));

	if (MapData && !MapData->MapNameToLoad.IsNone())
	{
		FTimerHandle LoadLevelTimer;
		GetWorld()->GetTimerManager().SetTimer(LoadLevelTimer, [this, MapData]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), MapData->MapNameToLoad);
		}, 0.4f, false);
	}
}
