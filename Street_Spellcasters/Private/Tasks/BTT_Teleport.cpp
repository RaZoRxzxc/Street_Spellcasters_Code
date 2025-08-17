// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Teleport.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BaseEnemyCharacter.h"

UBTT_Teleport::UBTT_Teleport(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Teleport";
}

EBTNodeResult::Type UBTT_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto const * const cont = OwnerComp.GetAIOwner())
	{
		if (auto* const Enemy = Cast<ABaseEnemyCharacter>(cont->GetPawn()))
		{
			const auto location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
			Enemy->Teleport(location);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
