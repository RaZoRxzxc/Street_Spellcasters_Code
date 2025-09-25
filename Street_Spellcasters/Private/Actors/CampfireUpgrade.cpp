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

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetupAttachment(RootComponent);
	
	OverlapBoxComp = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBoxComp->SetupAttachment(MeshComp);

	OverlapBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	OverlapBoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
}

// Called when the game starts or when spawned
void ACampfireUpgrade::BeginPlay()
{
	Super::BeginPlay();

	OverlapBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACampfireUpgrade::ACampfireUpgrade::OnOverlapBegin);
	OverlapBoxComp->OnComponentEndOverlap.AddDynamic(this, &ACampfireUpgrade::OnOverlapEnd);

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
		if (PlayerHUD)
		{
			if (!Player->CanLevelUp())
			{
				// Show interact horizontal box when player overlap collision
				PlayerHUD->ShowUpgradeBox();
			}
			
			// Show upgrade widget
			Player->SetCanShowLevelUpPanel(true);
		}

		// Restore stats to max
		Player->StatsComponent->RestoreStatsToMax();

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
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			// Hide interact horizontal box
			PlayerHUD->HideUpgradeBox();

			// If the level up menu is open, it will close when the player exits the collision.
			if(PlayerHUD->IsLevelMenuOpen())
			{
				PlayerHUD->HideLevelUpMenu();
			}
		}

		Player->SetCanShowLevelUpPanel(false);
	}
}


