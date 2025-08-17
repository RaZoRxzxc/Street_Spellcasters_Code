// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_MeleeAttack.h"
#include "AIController.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Engine/LatentActionManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BaseEnemyCharacter.h"

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = "DefaultAttack";
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	auto const * const cont = OwnerComp.GetAIOwner();
	auto* const Enemy = Cast<ABaseEnemyCharacter>(cont->GetPawn());

	if (!Enemy || !Enemy->CanAttack())
	{
		return EBTNodeResult::Failed;
	}

	if (Enemy->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
	{
		ICombatInterface::Execute_MeleeAttack(Enemy);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	
}


