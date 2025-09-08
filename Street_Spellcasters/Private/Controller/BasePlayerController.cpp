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
	ZoneActor = ZoneRef;
	
	FTimerHandle SetZoneTimer;
	GetWorld()->GetTimerManager().SetTimer(SetZoneTimer, this, &ABasePlayerController::DelayedZoneSetup, 0.1f, false);
	
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlayerController::DelayedZoneSetup()
{
	if (!ZoneActor) return;
    
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetHUD());
	if (!PlayerHUD) return;
    
	// Ждем создания виджета карты
	FTimerHandle WidgetCheckTimer;
	GetWorld()->GetTimerManager().SetTimer(WidgetCheckTimer, 
		[this, PlayerHUD]()
		{
			if (PlayerHUD->MapWidget && PlayerHUD->MapWidget->MapWidget)
			{
				PlayerHUD->MapWidget->MapWidget->SetZoneActor(ZoneActor);
                
				// Привязываем делегаты
				// ZoneActor->OnZoneInitialized.AddDynamic(this, &ABasePlayerController::OnZoneInit);
				// ZoneActor->OnZoneShrinking.AddDynamic(this, &ABasePlayerController::OnZoneShrinking);
				// ZoneActor->OnZoneEnded.AddDynamic(this, &ABasePlayerController::OnZoneEnded);
				// ZoneActor->OnZonePaused.AddDynamic(this, &ABasePlayerController::OnZonePaused);
			}
		}, 
		0.1f, false);
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
