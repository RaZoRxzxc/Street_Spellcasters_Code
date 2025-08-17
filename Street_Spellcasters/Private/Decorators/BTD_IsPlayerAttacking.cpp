
#include "Decorators/BTD_IsPlayerAttacking.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/CombatInterface.h"

UBTD_IsPlayerAttacking::UBTD_IsPlayerAttacking()
{
	NodeName = "IsPlayerAttacking";
}

bool UBTD_IsPlayerAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (auto* character = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()))
	{
		if (!character) return false;
		
		if (character->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
		{
			bool bIsAttack;
			ICombatInterface::Execute_IsPlayerAttack(character, bIsAttack);
			return bIsAttack;
		}
	}
	return false;
}
