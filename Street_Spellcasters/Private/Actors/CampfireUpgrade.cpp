// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CampfireUpgrade.h"

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
				PlayerHUD->ShowUpgradeBox();
			}
			Player->SetCanShowLevelUpPanel(true);
		}
		Player->StatsComponent->RestoreStatsToMax();
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
			PlayerHUD->HideUpgradeBox();
		}

		Player->SetCanShowLevelUpPanel(false);
	}
}


