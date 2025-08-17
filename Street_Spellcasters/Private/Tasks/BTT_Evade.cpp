// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Evade.h"

#include "AIController.h"
#include "Characters/BaseEnemyCharacter.h"

UBTT_Evade::UBTT_Evade(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Evade";
}

EBTNodeResult::Type UBTT_Evade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseEnemyCharacter* Enemy = Cast<ABaseEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Enemy->Evade();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
