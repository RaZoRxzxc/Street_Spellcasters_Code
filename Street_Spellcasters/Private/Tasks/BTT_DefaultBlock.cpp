// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_DefaultBlock.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BaseEnemyCharacter.h"

UBTT_DefaultBlock::UBTT_DefaultBlock(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "DefaultBlock";
}

EBTNodeResult::Type UBTT_DefaultBlock::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if (const auto* cont = Comp.GetAIOwner())
	{
		if (auto* character = Cast<ABaseEnemyCharacter>(cont->GetPawn()))
		{
			if (character->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
			{
				ICombatInterface::Execute_StartBlock(character);

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [character]
				{
					ICombatInterface::Execute_EndBlock(character);
				},
					2.0f, false);
				
				FinishLatentTask(Comp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
