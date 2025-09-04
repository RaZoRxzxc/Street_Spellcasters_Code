// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_isPlayerDead.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CombatInterface.h"

UBTD_isPlayerDead::UBTD_isPlayerDead()
{
	NodeName = "isPlayerDead";
}

bool UBTD_isPlayerDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (auto* character = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()))
	{
		if (character->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
		{
			bool bIsDead;
			ICombatInterface::Execute_isPlayerDead(character, bIsDead);
			return !bIsDead;
		}
	}
	return false;
}
