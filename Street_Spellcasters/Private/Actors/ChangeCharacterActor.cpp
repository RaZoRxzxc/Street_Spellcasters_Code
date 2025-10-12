// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ChangeCharacterActor.h"
#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Widgets/PlayerHUD.h"

AChangeCharacterActor::AChangeCharacterActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	Mesh->SetupAttachment(RootComponent);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(Mesh);
}

void AChangeCharacterActor::InteractWith_Implementation(ACharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);

	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!PlayerHUD) return;
	
	if (PlayerHUD->IsCharSelectMenuOpen())
	{
		PlayerHUD->HideCharacterSelectMenu();

		PlayerHUD->ShowInteractBox(ActorName);
		return;
	}
	
	if (CanChangeCharacter())
	{
		PlayerHUD->ShowCharacterSelectMenu();
	}
}

void AChangeCharacterActor::BeginPlay()
{
	Super::BeginPlay();

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AChangeCharacterActor::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AChangeCharacterActor::OnOverlapEnd);

}

void AChangeCharacterActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		Player->CurrentInteractable = this;

		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			if (!CanChangeCharacter())
			{
				// Show interact horizontal box when player overlap collision
				PlayerHUD->ShowInteractBox(ActorName);
			}
			SetCanShowCharSelectMenu(true);
		}
	}
}

void AChangeCharacterActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		Player->CurrentInteractable = nullptr;

		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			PlayerHUD->HideInteractBox();
			
			if (PlayerHUD->IsCharSelectMenuOpen())
			{
				PlayerHUD->HideCharacterSelectMenu();
				PlayerHUD->ShowInteractBox(ActorName);
			}
				
			SetCanShowCharSelectMenu(false);
		}
	}
}

void AChangeCharacterActor::SetCanShowCharSelectMenu(bool bCanShow)
{
	bCanShowCharSelectMenu = bCanShow;
}




