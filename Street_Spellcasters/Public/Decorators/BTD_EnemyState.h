// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "Controller/EnemyAIController.h"
#include "BTD_EnemyState.generated.h"

UCLASS()
class STREET_SPELLCASTERS_API UBTD_EnemyState : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTD_EnemyState();
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    
	UPROPERTY(EditAnywhere, Category=Blackboard)
	EEnemyState DesiredState;	
};
