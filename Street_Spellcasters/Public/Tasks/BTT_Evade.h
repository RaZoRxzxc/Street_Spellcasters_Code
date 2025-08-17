// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Evade.generated.h"

/**
 * 
 */
UCLASS()
class STREET_SPELLCASTERS_API UBTT_Evade : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTT_Evade(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
