// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_CanSeePlayer.h"

#include "PhysicsAssetRenderUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CanSeePlayer::UBTD_CanSeePlayer()
{
	NodeName = "CanSeePlayer";
}

bool UBTD_CanSeePlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	if (const auto* cont = OwnerComp.GetAIOwner())
	{
		const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
		const FVector ActorLocation = TargetActor->GetActorLocation();
		const FVector OwnerLocation = OwnerComp.GetOwner()->GetActorLocation();
		
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(OwnerComp.GetOwner());
		
		
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, OwnerLocation, ActorLocation, ECC_Visibility, QueryParams);
		UE_LOG(LogTemp, Warning, TEXT("Player detected"));
		
		return !bHit;
		
	}
	
	return false;
}
