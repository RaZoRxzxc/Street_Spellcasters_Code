// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Controller/EnemyAIController.h"

UBTT_ChasePlayer::UBTT_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "ChasePlayer";
}

EBTNodeResult::Type UBTT_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AEnemyAIController>(Comp.GetAIOwner()))
	{
		auto const PlayerLocation = Comp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

		FinishLatentTask(Comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
