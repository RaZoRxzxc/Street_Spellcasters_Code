// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"
#include "Interface/InputHandlerInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
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

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(MappingContext, 0);

        UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings();
        if (UserSettings)
        {
            if (!UserSettings->IsMappingContextRegistered(MappingContext))
            {
                UserSettings->RegisterInputMappingContext(MappingContext);
            }
        }
    }

    if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // Jumping
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ABasePlayerController::HandleJump);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABasePlayerController::HandleStopJumping);

        // Moving
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleMove);

        // Looking
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleLook);

        // Attacking
        EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ABasePlayerController::HandleAttack);

        // Evading
        EnhancedInput->BindAction(EvadeAction, ETriggerEvent::Started, this, &ABasePlayerController::HandleEvade);
        EnhancedInput->BindAction(EvadeAction, ETriggerEvent::Completed, this, &ABasePlayerController::HandleEvade);

        // Sprinting
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleSprintStart);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasePlayerController::HandleSprintStop);

        // Blocking
        EnhancedInput->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleBlockStart);
        EnhancedInput->BindAction(BlockAction, ETriggerEvent::Completed, this, &ABasePlayerController::HandleBlockEnd);

        // Interact
        EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ABasePlayerController::HandleInteract);

        // Healing
        EnhancedInput->BindAction(HealAction, ETriggerEvent::Started, this, &ABasePlayerController::HandleHeal);

        // Open map
        EnhancedInput->BindAction(MapAction, ETriggerEvent::Started, this, &ABasePlayerController::HandleToggleMap);
    }
}

// Методы обработки ввода, использующие интерфейс
void ABasePlayerController::HandleMove(const FInputActionValue& Value)
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleMove(GetPawn(), Value);
    }
}

void ABasePlayerController::HandleLook(const FInputActionValue& Value)
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleLook(GetPawn(), Value);
    }
}

void ABasePlayerController::HandleJump()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleJump(GetPawn());
    }
}

void ABasePlayerController::HandleStopJumping()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleStopJumping(GetPawn());
    }
}

void ABasePlayerController::HandleSprintStart()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleSprintStart(GetPawn());
    }
}

void ABasePlayerController::HandleSprintStop()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleSprintStop(GetPawn());
    }
}

void ABasePlayerController::HandleAttack()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleAttack(GetPawn());
    }
}

void ABasePlayerController::HandleBlockStart()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleBlockStart(GetPawn());
    }
}

void ABasePlayerController::HandleBlockEnd()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleBlockEnd(GetPawn());
    }
}

void ABasePlayerController::HandleEvade(const FInputActionValue& Value)
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleEvade(GetPawn(), Value);
    }
}

void ABasePlayerController::HandleInteract()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleInteract(GetPawn());
    }
}

void ABasePlayerController::HandleHeal()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleHeal(GetPawn());
    }
}

void ABasePlayerController::HandleToggleMap()
{
    if (GetPawn() && GetPawn()->Implements<UInputHandlerInterface>())
    {
        IInputHandlerInterface::Execute_HandleToggleMap(GetPawn());
    }
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
