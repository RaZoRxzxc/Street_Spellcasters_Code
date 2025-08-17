// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/EnemyAIController.h"

UBTT_FocusTarget::UBTT_FocusTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Focus Target";
}

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(Comp.GetAIOwner()))
	{
		AActor* AttackTarget = Cast<AActor>(Comp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
		if (AttackTarget == nullptr) return EBTNodeResult::Failed;
		
		cont->SetFocus(AttackTarget, EAIFocusPriority::Gameplay);
		
		FinishLatentTask(Comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	
	return EBTNodeResult::Failed;
}
