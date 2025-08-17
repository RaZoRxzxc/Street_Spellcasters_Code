// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/BTService_IsPlayerInMeleeRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BaseEnemyCharacter.h"
#include "Controller/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("IsPlayerInRange");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	auto const * const cont = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	auto const * const Enemy = Cast<ABaseEnemyCharacter>(cont->GetPawn());

	auto const * const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), Enemy->GetDistanceTo(Player) <= AttackRange);
}
