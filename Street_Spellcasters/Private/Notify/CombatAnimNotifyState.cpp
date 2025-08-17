// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/CombatAnimNotifyState.h"
#include "Components/StatsComponent.h"
#include "Interface/CombatInterface.h"
#include "Rendering/StaticLightingSystemInterface.h"

void UCombatAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (auto const PlayerMesh = MeshComp->GetOwner())
	{
		//AActor* Owner = MeshComp->GetOwner();
		UStatsComponent* CombatComponent = PlayerMesh->FindComponentByClass<UStatsComponent>();
		if (CombatComponent && CombatComponent->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_CallToggleWeaponTrace(CombatComponent, true);
		}
	}
}

void UCombatAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (auto const PlayerMesh = MeshComp->GetOwner())
	{
		//AActor* Owner = MeshComp->GetOwner();
		
		UStatsComponent* CombatComponent = PlayerMesh->FindComponentByClass<UStatsComponent>();
		if (CombatComponent && CombatComponent->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_CallToggleWeaponTrace(CombatComponent, false);
		}
	}
}
