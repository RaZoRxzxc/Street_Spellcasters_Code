// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StepsSoundNotify.generated.h"

/**
 * 
 */
UCLASS()
class STREET_SPELLCASTERS_API UStepsSoundNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	void PlayFootstepSound(AActor* OwnerActor);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sound")
	TMap<UPhysicalMaterial*, USoundBase*> FootstepSounds;
};
