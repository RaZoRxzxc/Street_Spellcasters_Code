// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/StepsSoundNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UStepsSoundNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		PlayFootstepSound(MeshComp->GetOwner());
	}
}

void UStepsSoundNotify::PlayFootstepSound(AActor* OwnerActor)
{
	if (!OwnerActor) return;

	FVector StartLocation = OwnerActor->GetActorLocation();
	FVector EndLocation = StartLocation - FVector(0, 0, 400.0f); 

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(OwnerActor);

	if (OwnerActor->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();

		if (PhysMaterial && FootstepSounds.Contains(PhysMaterial))
		{
			USoundBase* SoundToPlay = FootstepSounds[PhysMaterial];
			if (SoundToPlay)
			{
				UGameplayStatics::PlaySoundAtLocation(OwnerActor, SoundToPlay, HitResult.Location);
			}
		}
	}
}
