


#include "Actors/ZoneActor.h"

#include "Characters/BaseCharacter.h"
#include "Components/StatsComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Interface/ZoneInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


AZoneActor::AZoneActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("ZoneMesh");
	ZoneMesh->SetupAttachment(RootComponent);

}

void AZoneActor::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* Controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (Controller->GetClass()->ImplementsInterface(UZoneInterface::StaticClass()))
		{
			IZoneInterface::Execute_OnZoneBeginPlay(Controller, this);
		}
	}

	FTimerHandle CheckZoneTimer;
	GetWorldTimerManager().SetTimer(CheckZoneTimer, this, &AZoneActor::CheckForInAndOutZone, 0.5f, true);

	FTimerHandle DamagePlayerTimer;
	GetWorldTimerManager().SetTimer(DamagePlayerTimer, this, &AZoneActor::DamagePlayerOutside, 1.0f, true);

	ZoneMesh->SetGenerateOverlapEvents(false);

	FTimerHandle ZoneInitialTimer;

	GetWorldTimerManager().SetTimer(ZoneInitialTimer,
		[this]
		{
			SetActorHiddenInGame(false);
			bIsZoneStarted = true;

			UE_LOG(LogTemp, Warning, TEXT("Zone initial"));
			
			if (SetupFirstStep())
			{
				ZoneInitialized(CurrentStepStruct.CurrentStepSettings.PauseTimer);

				FTimerHandle StartZoneTimer;
				GetWorldTimerManager().SetTimer(StartZoneTimer,
					[this]
					{
						bCanShrink = true;
						StartZoneShrink(CurrentStepStruct);
						UE_LOG(LogTemp, Warning, TEXT("Zone Started"));
						
					}, CurrentStepStruct.CurrentStepSettings.PauseTimer, false);
			}
		}, ZoneInitialDelay, false);
}

// Spawn bosses when the zone stops near a tree
void AZoneActor::SpawnBossAtTargetTree()
{
	if (!ZoneTreeTargets.IsValidIndex(CurrentTreeTargetIndex)) return;
	if (!BossClass) return;

	FVector SpawnLoc = ZoneTreeTargets[CurrentTreeTargetIndex]->GetActorLocation();
	FRotator SpawnRot = FRotator::ZeroRotator;

	CurrentBoss = GetWorld()->SpawnActor<AActor>(BossClass, SpawnLoc, SpawnRot);

	if (ABaseCharacter* Boss = Cast<ABaseCharacter>(CurrentBoss))
	{
		Boss->OnDestroyed.AddDynamic(this, &AZoneActor::OnBossKilled);
	}
}

// When boss is dead, start next zone or spawn portal
void AZoneActor::OnBossKilled(AActor* DestroyedActor)
{
	ResetZoneToInitialSize();

	CurrentTreeTargetIndex++;

	if (CurrentTreeTargetIndex < ZoneTreeTargets.Num())
	{
		StartNextZoneSequence();
	}
	else
	{
		SpawnPortal();
	}
}

// Reset zone on default value
void AZoneActor::ResetZoneToInitialSize()
{
	ZoneMesh->SetWorldScale3D(FVector(ZoneInitialScale, ZoneInitialScale, ZoneZScale));
	bCanShrink = false;
	CurrentStepIndex = 0;
}

void AZoneActor::StartNextZoneSequence()
{
	SetupFirstStep();

	FTimerHandle StartTimer;
	GetWorldTimerManager().SetTimer(StartTimer, [this]
	{
		bCanShrink = true;
		StartZoneShrink(CurrentStepStruct);
	}, CurrentStepStruct.CurrentStepSettings.PauseTimer, false);
}

// Spawn portal when second boss is died 
void AZoneActor::SpawnPortal()
{
	if (!PortalClass) return;

	FVector SpawnLoc = GetActorLocation();
	GetWorld()->SpawnActor<AActor>(PortalClass, SpawnLoc, FRotator::ZeroRotator);
}

bool AZoneActor::IsPointInCylinder(const FVector& Point, const FVector& CylinderCenter, float Radius, float HalfHeight)
{
	if (FMath::Abs(Point.Z - CylinderCenter.Z) > HalfHeight)
	{
		return false;
	}
    
	// Проверяем радиус в XY-плоскости
	FVector2D PointXY(Point.X, Point.Y);
	FVector2D CenterXY(CylinderCenter.X, CylinderCenter.Y);
	float Distance2D = FVector2D::Distance(PointXY, CenterXY);
    
	return Distance2D <= Radius;
}

void AZoneActor::ShrinkZone()
{
	// Setup new location zone
	auto NewLocation = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(),
		CurrentStepStruct.CurrentStepSettings.LocationGoal,
		ZoneDeltaTime, CurrentStepStruct.CurrentStepSettings.ShrinkSpeed);
	SetActorLocation(NewLocation);

	// Setup new scale zone
	auto NewScale = FMath::Lerp(CurrentStepStruct.CurrentStepSettings.CurrentScale,
		CurrentStepStruct.CurrentStepSettings.ScaleGoal, GetScaleAlpha());
	ZoneMesh->SetWorldScale3D(NewScale);
	
}

void AZoneActor::SetupTheNextPointOrEndTheZone()
{
	FVector WorldScale = ZoneMesh->GetComponentScale();
	if (WorldScale.X <= MaxShrinkValue)
	{
		bCanShrink = false;
		OnZoneEnded.Broadcast();
	}
	else
	{
		if (CurrentScaleAlpha == 1.0)
		{
			bCanShrink = false;

			if (SetupNextStep())
			{
				ZonePaused(CurrentStepStruct, CurrentStepStruct.CurrentStepSettings.PauseTimer);

				FTimerHandle NextPointTimer;
				GetWorldTimerManager().SetTimer(NextPointTimer,
					[this]
					{
						bCanShrink = true;
						StartZoneShrink(CurrentStepStruct);
						
					}, CurrentStepStruct.CurrentStepSettings.PauseTimer, false);
			}
			else
			{
				ZoneEnded();
			}
		}
	}
}

bool AZoneActor::SetupFirstStep()
{
	CurrentStepIndex = 0;

	if (ZoneStepConfig.Num() > 0)
	{
		SetupNextStep();
		
		return true;
	}
	
	return false;
}

void AZoneActor::GetZoneRadius(float& Radius, float& RadiusN)
{
	FVector Origin;
	FVector BoxExtent;
	float SphereRadius{};
    
	UKismetSystemLibrary::GetComponentBounds(ZoneMesh, Origin, BoxExtent, SphereRadius);

	Radius = BoxExtent.X / 2;
	RadiusN = (BoxExtent.X / 2) * -1.0f;
}

FVector2D AZoneActor::GetZoneRandomLocation()
{
	float ZRadius;
	float ZRadiusN;
	GetZoneRadius(ZRadius, ZRadiusN);

	float RandomX = UKismetMathLibrary::RandomFloatInRange(-ZRadius, ZRadius);
	float RandomY = UKismetMathLibrary::RandomFloatInRange(-ZRadius, ZRadius);
    
	return FVector2D(RandomX, RandomY);
	
}

float AZoneActor::GetRandomMultiplier()
{
	return UKismetMathLibrary::RandomFloatInRange(-1.0f, 1.0f);
}

bool AZoneActor::SetupNextStep()
{
	UE_LOG(LogTemp, Warning, TEXT("SetupNextStep: Index %d, ConfigNum %d"), CurrentStepIndex, ZoneStepConfig.Num());
	
	if (CurrentStepIndex < ZoneStepConfig.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting up step %d: %s"), CurrentStepIndex, *ZoneStepConfig[CurrentStepIndex].StepID.ToString());
		
		// FVector2D RandomOffset = GetZoneRandomLocation();
		// GoalStep = GetActorLocation() + FVector(RandomOffset.X, RandomOffset.Y, 0);
		if (ZoneTreeTargets.IsValidIndex(CurrentTreeTargetIndex))
		{
			FVector TreeLocation = ZoneTreeTargets[CurrentTreeTargetIndex]->GetActorLocation();
			FVector2D RandomOffset = GetZoneRandomLocation();

			GoalStep = FVector(TreeLocation.X + RandomOffset.X * 0.3f, TreeLocation.Y + RandomOffset.Y * 0.3f, GetActorLocation().Z);
		}
		else
		{
			GoalStep = GetActorLocation();
		}
        
		Difference = GoalStep - GetActorLocation();
		
		if (ZoneStepConfig.Num() > 0)
		{
			StepScale = ZoneMesh->GetComponentScale() / ZoneStepConfig.Num();
		}
		
		FCurrentStepSettings CurrentStepSettings;
		CurrentStepSettings.LocationGoal = GoalStep;
		CurrentStepSettings.CurrentGoalDifference = Difference;
		CurrentStepSettings.CurrentScale = ZoneMesh->GetComponentTransform().GetScale3D();
		CurrentStepSettings.ScaleGoal = GetGoalScale(StepScale);
		CurrentStepSettings.StepScale = StepScale;
		CurrentStepSettings.ShrinkSpeed = ZoneStepConfig[CurrentStepIndex].ShrinkSpeed;
		CurrentStepSettings.PauseTimer = ZoneStepConfig[CurrentStepIndex].PauseTime;
		CurrentStepSettings.Damage = ZoneStepConfig[CurrentStepIndex].DamagePerSecond;

		FCurrentStep CurStep;
		CurStep.Name = ZoneStepConfig[CurrentStepIndex].StepID;
		CurStep.StepIndex = CurrentStepIndex;
		CurStep.CurrentStepSettings = CurrentStepSettings;

		CurrentStepStruct = CurStep;
		CurrentStepIndex++;

		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("No more steps available"));
	return false;
}

FVector AZoneActor::GetGoalScale(FVector NewStepScale) const
{
	auto NextScale = ZoneStepConfig.Num() - (CurrentStepIndex + 1); 
	if (NextScale > 0)
	{
		auto Step = NewStepScale * NextScale;
		auto NewShrinkScale = UKismetMathLibrary::MakeVector(Step.X, Step.Y, ZoneZScale);
		return NewShrinkScale;
	}
	else
	{
		auto ShrinkScale = UKismetMathLibrary::MakeVector(MaxShrinkValue, MaxShrinkValue, ZoneZScale);
		return ShrinkScale;
	}
}

float AZoneActor::GetScaleAlpha()
{
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = CurrentStepStruct.CurrentStepSettings.LocationGoal;
	FVector InitialDifference = CurrentStepStruct.CurrentStepSettings.CurrentGoalDifference;
    
	float CurrentDistance = (TargetLocation - CurrentLocation).Size();
	float InitialDistance = InitialDifference.Size();
    
	if (InitialDistance > 0)
	{
		CurrentScaleAlpha = 1.0f - (CurrentDistance / InitialDistance);
		CurrentScaleAlpha = FMath::Clamp(CurrentScaleAlpha, 0.0f, 1.0f);
	}
	else
	{
		CurrentScaleAlpha = 1.0f;
	}
    
	return CurrentScaleAlpha;
	
}

FVector AZoneActor::GetTargetScale()
{
	return CurrentStepStruct.CurrentStepSettings.ScaleGoal;
}

FVector AZoneActor::GetTargetLocation()
{
	return CurrentStepStruct.CurrentStepSettings.LocationGoal;
}

void AZoneActor::DamagePlayerOutside()
{
	if (ActorsOutsideTheZone.Num() == 0)
	{
		return;
	}

	FVector Origin;
	FVector BoxExtent;
	float SphereRadius;
	UKismetSystemLibrary::GetComponentBounds(ZoneMesh, Origin, BoxExtent, SphereRadius);
    
	float CylinderRadius = BoxExtent.X;
	float HalfHeight = BoxExtent.Z;

	for (int32 i = ActorsOutsideTheZone.Num() - 1; i >= 0; i--)
	{
		AActor* Actor = ActorsOutsideTheZone[i];
		if (!Actor || !Actor->IsValidLowLevel())
		{
			ActorsOutsideTheZone.RemoveAt(i);
			continue;
		}
		
		if (IsPointInCylinder(Actor->GetActorLocation(), Origin, CylinderRadius, HalfHeight))
		{
			ActorsOutsideTheZone.RemoveAt(i);
			continue;
		}
		// if (Actor->Implements<UZoneInterface>())
		// {
		// 	IZoneInterface::Execute_OnZoneDamageTaken(Actor, ZoneDamage);
		// 	
		// }
		
		float ZoneDamage = CurrentStepStruct.CurrentStepSettings.Damage;
		
		AController* DamageInstigator = GetInstigator() ? GetInstigator()->GetController() : nullptr;
		UGameplayStatics::ApplyDamage(Actor, ZoneDamage, DamageInstigator, this, UDamageType::StaticClass());
	}
}

void AZoneActor::CheckForInAndOutZone()
{
	
	auto State = GetWorld()->GetGameState();
    if (!State) 
    {
        UE_LOG(LogTemp, Warning, TEXT("GameState is null!"));
        return;
    }

    for (auto Player : State->PlayerArray)
    {
        if (!Player) continue;
        
        ABaseCharacter* Character = Cast<ABaseCharacter>(Player->GetPawn());
        if (!Character)
        {
            Character = Cast<ABaseCharacter>(Player->GetOwner());
        }
        
        if (Character && Character->IsValidLowLevel())
        {
            FVector Origin;
            FVector BoxExtent;
            float SphereRadius;
            UKismetSystemLibrary::GetComponentBounds(ZoneMesh, Origin, BoxExtent, SphereRadius);
        	
            float CylinderRadius = BoxExtent.X;
            float HalfHeight = BoxExtent.Z;

            FVector CharacterLocation = Character->GetActorLocation();
        	
            bool bIsInsideCylinder = IsPointInCylinder(CharacterLocation, Origin, CylinderRadius, HalfHeight);

            if (!bIsInsideCylinder)
            {
                if (!ActorsOutsideTheZone.Contains(Character))
                {
                    UE_LOG(LogTemp, Warning, TEXT("ADDING %s to outside list"), *Character->GetName());
                    if (Character->GetClass()->ImplementsInterface(UZoneInterface::StaticClass()))
                    {
                        IZoneInterface::Execute_OnZoneOutSide(Character);
	
                    }
                    ActorsOutsideTheZone.Add(Character);
                }
            }
            else
            {
                if (ActorsOutsideTheZone.Contains(Character))
                {
                    UE_LOG(LogTemp, Warning, TEXT("REMOVING %s from outside list"), *Character->GetName());
                    if (Character->GetClass()->ImplementsInterface(UZoneInterface::StaticClass()))
                    {
                        IZoneInterface::Execute_OnZoneInside(Character);
                    }
                    ActorsOutsideTheZone.Remove(Character);
                }
            }
        }
    }
}

void AZoneActor::StartZoneShrink(FCurrentStep StepConfig)
{
	StepConfig = CurrentStepStruct;
	bCanShrink = true;
	OnZoneShrinking.Broadcast(CurrentStepStruct);
	bDrawNextPoint = true;
}

void AZoneActor::ZoneEnded()
{
	OnZoneEnded.Broadcast();
	bDrawNextPoint = true;
}

void AZoneActor::ZoneInitialized(float ZoneBeginIn)
{
	OnZoneInitialized.Broadcast(ZoneBeginIn);
}

void AZoneActor::ZonePaused(FCurrentStep NextStep, float NextPointIn)
{
	OnZonePaused.Broadcast(NextPointIn);
	NextStep = CurrentStepStruct;
}

void AZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZoneDeltaTime = DeltaTime;

	if (bCanShrink)
	{
		ShrinkZone();

		SetupTheNextPointOrEndTheZone();
	}

	// if (ZoneMesh)
	// {
	// 	FVector Origin;
	// 	FVector BoxExtent;
	// 	float SphereRadius;
	// 	UKismetSystemLibrary::GetComponentBounds(ZoneMesh, Origin, BoxExtent, SphereRadius);
 //        
	// 	DrawDebugCylinder(GetWorld(), 
	// 					 Origin - FVector(0, 0, BoxExtent.Z), 
	// 					 Origin + FVector(0, 0, BoxExtent.Z), 
	// 					 BoxExtent.X, 32, FColor::Black, false, -1.0f, 0, 2.0f);
 //        
	// 	DrawDebugPoint(GetWorld(), Origin, 10.0f, FColor::Red, false, -1.0f);
	// }

}

