// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"

#include "Widgets/MapWidget.h"
#include "Widgets/MiniMapWidget.h"
#include "Widgets/PlayerHUD.h"

ABasePlayerController::ABasePlayerController()
{
}

void ABasePlayerController::OnZoneBeginPlay_Implementation(AZoneActor* ZoneRef)
{
	ZoneRef = ZoneActor;
	FTimerHandle SetZoneTimer;

	// Set zone actor
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// GetWorldTimerManager().SetTimer(SetZoneTimer, [this, PlayerHUD]
	// {
	// 	if (PlayerHUD)
	// 	{
	// 		PlayerHUD->MapWidget->MapWidget->SetZoneActor(ZoneActor);
	// 	}
	// }, 0.2f, true);
	if (PlayerHUD)
	{
		PlayerHUD->MapWidget->MapWidget->SetZoneActor(ZoneActor);
	}


	// Bind delegate function
	// ZoneActor->OnZoneInitialized.AddDynamic(this, &ABasePlayerController::OnZoneInit);
	// ZoneActor->OnZoneShrinking.AddDynamic(this, &ABasePlayerController::OnZoneShrinking);
	// ZoneActor->OnZoneEnded.AddDynamic(this, &ABasePlayerController::OnZoneEnded);
	// ZoneActor->OnZonePaused.AddDynamic(this, &ABasePlayerController::OnZonePaused);
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlayerController::OnZoneInit(float ZoneBeginsIn)
{
	
}

void ABasePlayerController::OnZoneShrinking(FCurrentStep CurrentConfig)
{
}

void ABasePlayerController::OnZonePaused(float NextPointIn)
{
}

void ABasePlayerController::OnZoneEnded()
{
}
