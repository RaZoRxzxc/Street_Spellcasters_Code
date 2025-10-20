// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/PlayerHUD.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance && !GameInstance->GetSelectedCharacter().Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("No character selected in GameInstance!"));
	}
	
	SpawnSelectedCharacter();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			PlayerHUD->HideStatsBox();
		}
	}, 0.1f, false);
}

void ALobbyGameMode::SpawnSelectedCharacter()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	FCharacterStruct SelectedCharacter = GameInstance->GetSelectedCharacter();

	if (SelectedCharacter.Character)
	{
		AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
		if (!PlayerStart) return;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABaseCharacter* SpawnedChar = GetWorld()->SpawnActor<ABaseCharacter>(SelectedCharacter.Character,
			PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParams);

		if (SpawnedChar && GetWorld()->GetFirstPlayerController())
		{
			GetWorld()->GetFirstPlayerController()->Possess(SpawnedChar);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid character class to spawn in lobby!"));
	}
}
