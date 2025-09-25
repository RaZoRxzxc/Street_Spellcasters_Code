// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "Actors/MenuCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/MenuHUD.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		AActor* MenuCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AMenuCameraActor::StaticClass());
		if (MenuCamera)
		{
			PC->SetViewTargetWithBlend(MenuCamera, 0.0f);
			PC->SetInputMode(FInputModeUIOnly());

			if (AMenuHUD* HUD = Cast<AMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				HUD->ShowMenuWidget();
			}
			
			
		}
	}
}
