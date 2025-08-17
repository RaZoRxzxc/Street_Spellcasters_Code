// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_isPlayerDead.generated.h"

/**
 * 
 */
UCLASS()
class STREET_SPELLCASTERS_API UBTD_isPlayerDead : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTD_isPlayerDead();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
