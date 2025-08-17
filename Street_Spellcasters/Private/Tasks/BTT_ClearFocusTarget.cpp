// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_ClearFocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/EnemyAIController.h"

UBTT_ClearFocusTarget::UBTT_ClearFocusTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "ClearFocusTarget";
}

EBTNodeResult::Type UBTT_ClearFocusTarget::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if (auto* cont = Cast<AEnemyAIController>(Comp.GetAIOwner()))
	{
		cont->ClearFocus(EAIFocusPriority::Gameplay);

		UE_LOG(LogTemp, Warning, TEXT("Clear Focus Target"));
		FinishLatentTask(Comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
	
}
