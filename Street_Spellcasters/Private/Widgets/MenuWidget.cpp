
#include "Widgets/MenuWidget.h"

#include "Components/Button.h"
#include "Widgets/CharacterSelectWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/MenuHUD.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayButton)
		PlayButton->OnClicked.AddDynamic(this, &UMenuWidget::StartGame);

	if (SettingsButton)
		SettingsButton->OnClicked.AddDynamic(this, &UMenuWidget::OpenSettingsMenu);
	
	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UMenuWidget::ExitGame);
	
}

void UMenuWidget::StartGame()
{
	if (AMenuHUD* HUD = Cast<AMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		HUD->ShowCharacterSelectMenu();
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
