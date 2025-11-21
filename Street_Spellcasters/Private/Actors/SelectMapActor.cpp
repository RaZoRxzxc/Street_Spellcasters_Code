
#include "Actors/SelectMapActor.h"
#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Widgets/PlayerHUD.h"

// Sets default values
ASelectMapActor::ASelectMapActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;
}

void ASelectMapActor::InteractWith_Implementation(ACharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);

	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!PlayerHUD) return;
	
	if (PlayerHUD->IsMapSelectMenuOpen())
	{
		PlayerHUD->HideSelectMapWidget();

		PlayerHUD->ShowInteractBox(ActorName);
		return;
	}
	
	if (CanChangeMap())
	{
		PlayerHUD->ShowSelectMapWidget();
	}
}

// Called when the game starts or when spawned
void ASelectMapActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASelectMapActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		Player->CurrentInteractable = this;

		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			if (!CanChangeMap())
			{
				// Show interact horizontal box when player overlap collision
				PlayerHUD->ShowInteractBox(ActorName);
			}
			SetCanShow(true);
		}
	}
}

void ASelectMapActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		Player->CurrentInteractable = nullptr;

		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			PlayerHUD->HideInteractBox();
			
			if (PlayerHUD-> IsMapSelectMenuOpen())
			{
				PlayerHUD->HideSelectMapWidget();
			}
				
			SetCanShow(false);
		}
	}
}

void ASelectMapActor::SetCanShow(bool bCanShow)
{
	bCanShowMapSelectMenu = bCanShow;
}


