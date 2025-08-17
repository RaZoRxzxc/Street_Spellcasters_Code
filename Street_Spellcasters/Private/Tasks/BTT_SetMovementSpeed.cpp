// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_SetMovementSpeed.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_SetMovementSpeed::UBTT_SetMovementSpeed(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "SetMovementSpeed";
	
}

EBTNodeResult::Type UBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	if (auto* cont = Comp.GetAIOwner())
	{
		if (auto const * Enemy = Cast<ACharacter>(cont->GetPawn()))
		{
			if (auto* CharMovement = Enemy->GetCharacterMovement())
			{
				CharMovement->MaxWalkSpeed = MovementSpeed;

				FinishLatentTask(Comp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
				
			}
		}
	}
	return EBTNodeResult::Failed;
}
