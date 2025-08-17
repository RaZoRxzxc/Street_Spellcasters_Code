// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;
class ABaseCharacter;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	E_Default UMETA(DisplayName = "Default"),
	E_Attacking UMETA(DisplayName = "Attacking"),
	E_Patrolling UMETA(DisplayName = "Patrolling"),
	E_Frozen UMETA(DisplayName = "Frozen")
};

UENUM(BlueprintType)
enum class E_AISense : uint8
{
	E_Sight UMETA(DisplayName = "Sight"),
	E_Hearing UMETA(DisplayName = "Hearing"),
	E_Damage UMETA(DisplayName = "Damage")
};

UCLASS()
class STREET_SPELLCASTERS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EEnemyState EnemyState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	E_AISense AISenseclass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseCharacter* TargetCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* TargetActor;
protected:

	virtual void OnPossess(APawn* PossessedPawn) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName AttackTargetKeyName = "AttackTarget";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName StateTargetKeyName = "State";

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	void CanSenseActor(AActor* Actor, const E_AISense AISense , bool& Sensed);

	void OnStateAsAttacking(AActor* AttackActor);
	
	void HandleSensedSight(AActor* SenseActor);
public:
	void OnStateAsPassive();

private:

	class UAISenseConfig_Sight* SenseConfig;

	UAIPerceptionComponent* PerceptionComponent;

	void SetupPerceptionConfig();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
};
