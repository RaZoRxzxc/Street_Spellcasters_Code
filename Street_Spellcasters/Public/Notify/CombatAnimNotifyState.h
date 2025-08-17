// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CombatAnimNotifyState.generated.h"

class UActorComponent;

UCLASS()
class STREET_SPELLCASTERS_API UCombatAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	public:
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,  float TotalDuration,  const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	//TSubclassOf<UActorComponent> CombatComponent;
};
