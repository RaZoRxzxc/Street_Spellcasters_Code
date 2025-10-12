// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MainGameMode.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	SpawnSelectedCharacter();
}

void AMainGameMode::SpawnSelectedCharacter()
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
}
