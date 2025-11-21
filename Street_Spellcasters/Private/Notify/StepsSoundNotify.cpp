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
    
        UWorld* World = OwnerActor->GetWorld();
        if (!World) return;
	
        FVector Start = OwnerActor->GetActorLocation();
        FVector End = Start - FVector(0, 0, 150.0f);
    
        FHitResult WaterHit;
        FHitResult GroundHit;;
		
    
        FCollisionQueryParams Params;
        Params.bReturnPhysicalMaterial = true;
        Params.AddIgnoredActor(OwnerActor);
	
        bool bInWater = World->LineTraceSingleByProfile(
            WaterHit, Start, End, FName("WaterBodyCollision"), Params
        );
    
        if (bInWater && WaterHit.bBlockingHit)
        {
            if (WaterHit.PhysMaterial.Get() == WaterPM)
            {
            	
            	bool bGroundHit = World->LineTraceSingleByChannel(GroundHit,WaterHit.Location + FVector(0, 0, 10.0f),WaterHit.Location - FVector(0, 0, 200.0f),
            		ECC_Visibility,Params);

            	float Depth = 0.f;

            	if (bGroundHit)
            	{
            		Depth = WaterHit.Location.Z - GroundHit.Location.Z;
            	}
            	
            	FootStepWaterSplashSound(OwnerActor, WaterHit.Location, Depth, bGroundHit);
            	return;
            }
        }
	
        bool bHit = World->LineTraceSingleByChannel(GroundHit, Start, End, ECC_Visibility, Params);
    
        if (bHit && GroundHit.bBlockingHit)
        {
            UPhysicalMaterial* PhysMat = GroundHit.PhysMaterial.Get();
    
            if (PhysMat && FootstepSounds.Contains(PhysMat))
            {
                UGameplayStatics::PlaySoundAtLocation(OwnerActor, FootstepSounds[PhysMat], GroundHit.Location);
            }
        }
}

void UStepsSoundNotify::FootStepTraceByChannel(AActor* Owner, FVector& Location, UPhysicalMaterial*& Material, bool& bTraceHit)
{
	if (Owner)
	{
		FVector StartLoc = FVector(30.0f, 10.0f, 0.0f);
		FVector EndLoc = FVector(30.0f, 10.0f, -120.0f);

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECC_Visibility, Params);
		if (bHit)
		{
			Location = HitResult.Location;
			Material = HitResult.PhysMaterial.Get();
			bTraceHit = bHit;
		}
	}
}

void UStepsSoundNotify::FootStepWaterSplashSound(AActor* Owner, FVector SoundLocation, float WaterDepth, bool TraceCollisionHit)
{
	FVector SoundLoc = SoundLocation;

	bool DeepWater = !TraceCollisionHit;

	if (DeepWater)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner, WaterFootStepSound, SoundLoc);
	}
	else
	{
		if (WaterDepth > 75.0f)
		{
			UGameplayStatics::PlaySoundAtLocation(Owner, DeepWaterFootStepSound, SoundLoc);
		}
		else if (WaterDepth > 35.0f)
		{
			UGameplayStatics::PlaySoundAtLocation(Owner, DeepWaterFootStepSound, SoundLoc);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(Owner, WaterFootStepSound, SoundLoc);
		}
	}
}
