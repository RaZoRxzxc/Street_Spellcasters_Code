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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sound")
	USoundBase* WaterFootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sound")
    USoundBase* DeepWaterFootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PhysMat")
	UPhysicalMaterial* WaterPM;

	UFUNCTION()
	void FootStepTraceByChannel(AActor* Owner, FVector& Location, UPhysicalMaterial*& Material, bool& bTraceHit);

	UFUNCTION()
	void FootStepWaterSplashSound(AActor* Owner, FVector SoundLocation, float WaterDepth, bool TraceCollisionHit);
};
