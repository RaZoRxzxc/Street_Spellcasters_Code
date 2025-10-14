// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/EnemyAIController.h"

UBTT_FindRandomLocation::UBTT_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "FindRandomLocation";
}

EBTNodeResult::Type UBTT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	// Get AI Controllers
	if (AEnemyAIController* const cont = Cast<AEnemyAIController>(Comp.GetAIOwner()))
	{
		if (auto const NPC = cont->GetPawn())
		{
			auto const Origin = NPC->GetActorLocation();

			if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc))
				{
					Comp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				}

				// Finish with Success
				FinishLatentTask(Comp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
