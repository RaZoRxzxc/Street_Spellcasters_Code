// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CampfireUpgrade.h"
#include "Widgets/MiniMapWidget.h"
#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/PlayerHUD.h"

// Sets default values
ACampfireUpgrade::ACampfireUpgrade()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	OverlapBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
}

void ACampfireUpgrade::InteractWith_Implementation(ACharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);

	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!PlayerHUD) return;
	
	if (PlayerHUD->IsLevelMenuOpen())
	{
		PlayerHUD->HideLevelUpMenu();
		
		PlayerHUD->ShowInteractBox(ActorName);
		return;
	}

	//ABaseCharacter* Player = Cast<ABaseCharacter>(Character);
	if (CanLevelUp())
	{
		PlayerHUD->HideInteractBox();
		PlayerHUD->ShowLevelUpMenu();
	}
}

// Called when the game starts or when spawned
void ACampfireUpgrade::BeginPlay()
{
	Super::BeginPlay();

	// Register this campfire on all player maps
	TArray<AActor*> PlayerCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), PlayerCharacters);
	
}

// Called every frame
void ACampfireUpgrade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACampfireUpgrade::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	if (Player && !PlayerHUD->IsLevelMenuOpen()) 
	{
		Player->CurrentInteractable = this;
		
		if (PlayerHUD)
		{
			if (!CanLevelUp())
			{
				// Show interact horizontal box when player overlap collision
				PlayerHUD->ShowInteractBox(ActorName);
			}
			
			// Show upgrade widget
			SetCanShow(true);
		}
	
		// Restore stats to max
		Player->GetStatsComp()->RestoreStatsToMax();

		// Play heal sound when player overlap collision 
		if (HealSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), HealSound);
		}
	}
}

void ACampfireUpgrade::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			Player->CurrentInteractable = nullptr;
			
			// Hide interact horizontal box
			PlayerHUD->HideInteractBox();

			// If the level up menu is open, it will close when the player exits the collision.
			if(PlayerHUD->IsLevelMenuOpen())
			{
				PlayerHUD->HideLevelUpMenu();
				PlayerHUD->ShowInteractBox(ActorName);
			}
		}

		SetCanShow(false);
	}
}

void ACampfireUpgrade::SetCanShow(bool bCanShow)
{
	bCanShowLevelPanel = bCanShow;
}


