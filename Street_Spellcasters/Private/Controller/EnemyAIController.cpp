
#include "Controller/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BaseCharacter.h"
#include "Characters/BaseEnemyCharacter.h"
#include "Perception/AISense.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"

AEnemyAIController::AEnemyAIController()
{
	SetupPerceptionConfig();
}

void AEnemyAIController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (const auto* Enemy = Cast<ABaseEnemyCharacter>(PossessedPawn))
	{
		RunBehaviorTree(Enemy->EnemyBehaviorTree);
		
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		if (Blackboard = GetBlackboardComponent())
		{
			OnStateAsPassive();
		}
	}, 0.1f, false);
	
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* UpdatedActor : UpdatedActors)
	{
		bool Sensed;
		CanSenseActor(UpdatedActor, E_AISense::E_Sight, Sensed);
		if (Sensed)
		{
			HandleSensedSight(UpdatedActor);
		}
	}
}

void AEnemyAIController::HandleSensedSight(AActor* SenseActor)
{
	OnStateAsAttacking(SenseActor);
}

void AEnemyAIController::CanSenseActor(AActor* Actor, const E_AISense AISense, bool& Sensed)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	GetPerceptionComponent()->GetActorsPerception(Actor, PerceptionInfo);

	for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
	{
		if (UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld()))
		{
			UClass* SenseClass = PerceptionSystem->GetSenseClassForStimulus(GetWorld() ,Stimulus);

			switch (AISense)
			{
			case E_AISense::E_Sight:
				SenseClass = UAISense_Sight::StaticClass();
				break;
			case E_AISense::E_Hearing:
				SenseClass = UAISense_Hearing::StaticClass();
				break;
			case E_AISense::E_Damage:
				SenseClass = UAISense_Damage::StaticClass();
				break;
			}
			Sensed = Stimulus.WasSuccessfullySensed();
		}
	}
}

void AEnemyAIController::OnStateAsAttacking(AActor* AttackActor)
{
	if (Blackboard = GetBlackboardComponent())
	{
		Blackboard->SetValueAsObject(AttackTargetKeyName, AttackActor);
		Blackboard->SetValueAsEnum(StateTargetKeyName, static_cast<uint8>(EEnemyState::E_Attacking));
		TargetActor = AttackActor;
	}
}

void AEnemyAIController::OnStateAsPassive()
{
	if (Blackboard = GetBlackboardComponent())
	{
		Blackboard->SetValueAsEnum(StateTargetKeyName, static_cast<uint8>(EEnemyState::E_Default));
	}
}


void AEnemyAIController::SetupPerceptionConfig()
{
	SenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseConfig");
	
	if (SenseConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent"));
		SenseConfig->SightRadius = 1000.0f;
		SenseConfig->LoseSightRadius = SenseConfig->SightRadius + 25.f;
		SenseConfig->PeripheralVisionAngleDegrees = 180.0f;
		SenseConfig->SetMaxAge(5.0f);
		SenseConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SenseConfig->DetectionByAffiliation.bDetectEnemies = true;
		SenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SenseConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SenseConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
		GetPerceptionComponent()->ConfigureSense(*SenseConfig);
	}
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const PlayerCharacter = Cast<ABaseCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
