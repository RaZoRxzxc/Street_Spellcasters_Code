// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class STREET_SPELLCASTERS_API UBTT_SetMovementSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	explicit UBTT_SetMovementSpeed(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 200.0f;
};
