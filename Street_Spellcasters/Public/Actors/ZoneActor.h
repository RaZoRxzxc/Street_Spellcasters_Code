// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneActor.generated.h"

USTRUCT(BlueprintType)
struct FCurrentStepSettings
{
	GENERATED_BODY()

	FVector LocationGoal;
	FVector CurrentGoalDifference;
	FVector CurrentScale;
	FVector ScaleGoal;
	FVector StepScale;
	
	float ShrinkSpeed;
	
	float PauseTimer;
	
	float Damage;
};

USTRUCT(BlueprintType)
struct FCurrentStep
{
	GENERATED_BODY()
	
	FText Name;
	
	int32 StepIndex;
	
	FCurrentStepSettings CurrentStepSettings;
	
};

USTRUCT(BlueprintType)
struct FZoneStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StepID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShrinkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PauseTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamagePerSecond;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneShrinking, FCurrentStep, StepConfig);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZoneInitialized, float, ZoneBeginsIn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnZonePaused, float, NextPointIn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZoneEnded);

UCLASS()
class STREET_SPELLCASTERS_API AZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AZoneActor();

	FOnZoneShrinking OnZoneShrinking;
	FOnZoneInitialized OnZoneInitialized;
	FOnZonePaused OnZonePaused;
	FOnZoneEnded OnZoneEnded;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Logic")
	FCurrentStep CurrentStepStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Logic")
	FZoneStep ZoneStep;
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* ZoneMesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	FVector GoalStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	FVector Difference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	FVector StepScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float ZoneInitialScale = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float ZoneZScale = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float ZoneInitialDelay = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float MaxShrinkValue = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	bool bIsStartsHidden = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	TArray<FZoneStep> ZoneStepConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	int32 Temp = 0;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float NearZoneDistance = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float ZoneHeight = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Info")
	bool bIsZoneStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Info")
	int32 CurrentStepIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Logic")
	bool bCanShrink = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Logic")
	float ZoneDeltaTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Logic")
	float CurrentScaleAlpha = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Logic")
	TArray<AActor*> ActorsOutsideTheZone;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Logic")
	bool bDrawNextPoint = false;

	UFUNCTION(BlueprintCallable, BlueprintPure ,Category= "Zone info")
	bool GetZoneStarted() { return bIsZoneStarted;}

	UFUNCTION(BlueprintCallable, Category= "Zone info")
	FCurrentStep GetCurrentStep() { return CurrentStepStruct; }
	
protected:

	bool IsPointInCylinder(const FVector& Point, const FVector& CylinderCenter, float Radius, float HalfHeight);
	
	UFUNCTION()
	void ShrinkZone();

	UFUNCTION()
	void SetupTheNextPointOrEndTheZone();

	UFUNCTION(BlueprintCallable, Category= "Zone Logic")
	bool SetupFirstStep();

	UFUNCTION(BlueprintCallable,BlueprintPure , Category= "Zone Logic")
	void GetZoneRadius(float& Radius, float& RadiusN);

	UFUNCTION(BlueprintCallable, BlueprintPure , Category= "Zone Logic")
	FVector2D GetZoneRandomLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure ,Category= "Zone Logic")
	float GetRandomMultiplier();

	UFUNCTION(BlueprintCallable, Category= "Zone Logic")
	bool SetupNextStep();

	UFUNCTION(BlueprintCallable,BlueprintPure , Category= "Zone Logic")
	FVector GetGoalScale(FVector NewStepScale) const;

	UFUNCTION(BlueprintCallable,BlueprintPure , Category= "Zone Logic")
	float GetScaleAlpha();

	UFUNCTION(BlueprintCallable,BlueprintPure , Category= "Minimap")
	FVector GetTargetScale();

	UFUNCTION(BlueprintCallable, BlueprintPure , Category= "Minimap")
	FVector GetTargetLocation();

	UFUNCTION(BlueprintCallable)
	void DamagePlayerOutside();

	UFUNCTION()
	void CheckForInAndOutZone();

public:

	UFUNCTION()
	void StartZoneShrink(FCurrentStep StepConfig);

	UFUNCTION()
	void ZoneEnded();

	UFUNCTION()
	void ZoneInitialized(float ZoneBeginIn);

	UFUNCTION()
	void ZonePaused(FCurrentStep NextStep, float NextPointIn);
};
