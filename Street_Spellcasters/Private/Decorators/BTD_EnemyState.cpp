// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_EnemyState.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTD_EnemyState::UBTD_EnemyState()
{
	NodeName = "Check Enemy State";
	BlackboardKey.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_EnemyState, BlackboardKey), StaticEnum<EEnemyState>());
}

bool UBTD_EnemyState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}

	const uint8 StateValue = BlackboardComp->GetValueAsEnum(BlackboardKey.SelectedKeyName);
	const EEnemyState CurrentState = static_cast<EEnemyState>(StateValue);
	return CurrentState == DesiredState;
}
