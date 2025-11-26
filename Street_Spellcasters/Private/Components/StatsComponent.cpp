// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatsComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/BaseCharacter.h"
#include "Characters/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BaseWeapon.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsTraceOn = false;

	// Evade variables
	EvadeSectionName = "";
	bIsEvading = false;
	EvadeDirection = EEvadeDirection::None;
	ResetEvadeDirectionTimerRate = 0.1f;

	// Stamina variables
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	StaminaRegenRate = 15.0f;
	StaminaRegenDelay = 1.0f;
	AttackStaminaCost = 10.0f;
	SprintStaminaCost = 5.0f;
	EvadeStaminaCost = 25.0f;

	DamageBlockMultiplier = 0.7f;

	// Hit direction variables
	HitDirectionName = "";
	HitDirection = EHitDirection::None;

	// Health variables
	MaxHealth = 100;
	Health = MaxHealth;
	PlusHealth = 40.0f;

	// Flasks amount
	FlasksAmount = 3;
	
}

void UStatsComponent::InterruptActions()
{
	bIsEvading = false;
	bIsBlocking = false;
	bIsTakeHit = false;
	
	bIsAttack = false;

	if (AActor* Owner = GetOwner())
	{
		Owner->CallFunctionByNameWithArguments(TEXT("HealEnd"), *GLog, nullptr, true);
	}
}

void UStatsComponent::BeginPlay()
{
    Super::BeginPlay();
    
    AActor* Owner = GetOwner();
    if (Owner)
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UStatsComponent::ApplyDamage);
    }
}

void UStatsComponent::RestoreStatsToMax()
{
	Health = MaxHealth;
	CurrentStamina = MaxStamina;
	FlasksAmount = 3;
	OnFlasksChanged.Broadcast(FlasksAmount);
}

void UStatsComponent::AddHealth()
{
	if (FlasksAmount > 0)
	{
		Health = FMath::Clamp(Health + PlusHealth,0,MaxHealth);
		FlasksAmount--;
		OnFlasksChanged.Broadcast(FlasksAmount);
		OnStatsChanged.Broadcast(GetHealthPercent(), GetStaminaPercent());
	}
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MeleeTrace();

	if (bIsSprinting && bIsActuallyMoving && CanSprinting() && !bIsBlocking)
	{
		const float StaminaDrain = SprintStaminaCost * DeltaTime;
		ConsumeStamina(StaminaDrain);
        
		if (FMath::IsNearlyZero(CurrentStamina))
		{
			StopSprinting();
		}
	}
	else if (CurrentStamina >= MaxStamina * 0.2f)
	{
		bStaminaDepleted = false;
	}
}

void UStatsComponent::LevelUp()
{
    Level++;
    SoulsToNextLevel = FMath::RoundToInt(SoulsToNextLevel * 2.35f);
    
    // Increase stats on level up
    MaxHealth += 50.f;
    MaxStamina += 25.f;
	Weapon->Damage += 10.0f;
	
    Health = MaxHealth;
    CurrentStamina = MaxStamina;
    
    OnLevelChanged.Broadcast(Level);
	OnStatsChanged.Broadcast(GetHealthPercent(), GetStaminaPercent());
	OnDamageChanged.Broadcast(Weapon->Damage);
}

// ================= BLOCK SYSTEM =================
void UStatsComponent::StartBlocking()
{
	bIsBlocking = true;
}

void UStatsComponent::EndBlocking()
{
	bIsBlocking = false;
}

// ================= HIT DIRECTION SYSTEM =================
void UStatsComponent::CalculateHitDirection(FVector HitOrigin)
{
	const AActor* OwnerActor = GetOwner();
	
	FVector HitDirectionVector = HitOrigin - OwnerActor->GetActorLocation();
	HitDirectionVector.Z = 0.0f;
	HitDirectionVector.Normalize();

	const FVector Forward = OwnerActor->GetActorForwardVector();
	const FVector Right = OwnerActor->GetActorRightVector();

	const float ForwardDot = FVector::DotProduct(Forward, HitDirectionVector);
	const float RightDot = FVector::DotProduct(Right, HitDirectionVector);

	if (ForwardDot > 0.5f)
	{
		HitDirection = EHitDirection::Hit_Forward;
	}
	else if (ForwardDot < -0.5f)
	{
		HitDirection = EHitDirection::Hit_Backward;
	}
	else if (RightDot > 0.f)
	{
		HitDirection = EHitDirection::Hit_Right;
	}
	else if (RightDot < 0.f)
	{
		HitDirection = EHitDirection::Hit_Left;
	}
	else
	{
		HitDirection = EHitDirection::None;
	}
}

void UStatsComponent::PlayHitAnimation(bool bIsBlockedHit)
{
	if (HitDirection == EHitDirection::None) return;

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return;

	UAnimMontage* MontageToPlay = bIsBlockedHit ? BlockHitMontage : HitMontage;
	

	switch (HitDirection)
	{
	case EHitDirection::None:
		HitDirectionName = "";
		break;
	case EHitDirection::Hit_Forward:
		HitDirectionName = "Hit_Forward";
		break;
	case EHitDirection::Hit_Backward:
		HitDirectionName = "Hit_Backward";
		break;
	case EHitDirection::Hit_Left:
		HitDirectionName = "Hit_Left";
		break;
	case EHitDirection::Hit_Right:
		HitDirectionName = "Hit_Right";
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Playing Montage: %s, Section: %s"), 
	   *GetNameSafe(MontageToPlay), 
	   *HitDirectionName.ToString());
	
	bIsTakeHit = true;
	AnimInstance->Montage_Play(MontageToPlay);
	AnimInstance->Montage_JumpToSection(HitDirectionName);

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &UStatsComponent::OnHitMontageEnded);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, MontageToPlay);
}

void UStatsComponent::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsTakeHit = false;
	HitDirection = EHitDirection::None;
}

// Apply Damage
void UStatsComponent::ApplyDamage(AActor* DamagedActor,float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f && IsDead()) return;

	InterruptActions();
	
	float ActualDamage = Damage;
	bool bWasBlocked = false;
	bIsAttack = false;
	
	if (bIsBlocking)
	{
		const FVector DamageDirection = (DamageCauser->GetActorLocation() - DamagedActor->GetActorLocation()).GetSafeNormal();
		const FVector PlayerForwardVector = DamagedActor->GetActorForwardVector();
		
		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PlayerForwardVector, DamageDirection)));
		
		if (Angle <= 90.0f)
		{
			ActualDamage *= (1.0f - DamageBlockMultiplier);
			bWasBlocked = true;
		}
	}
	
	CalculateHitDirection(DamageCauser->GetActorLocation());
	PlayHitAnimation(bWasBlocked);
	
	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

	OnStatsChanged.Broadcast(GetHealthPercent(), GetStaminaPercent());

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

// ================= ATTACK TRACE SYSTEM =================
void UStatsComponent::CallToggleWeaponTrace_Implementation(bool bIsWeaponTraceOn)
{
	bIsTraceOn = bIsWeaponTraceOn;
}

void UStatsComponent::MeleeTrace()
{
	if (bIsTraceOn && Weapon)
	{
			FVector TopLocation = Weapon->WeaponMesh->GetSocketLocation("TopTrace");
			FVector BottomLocation = Weapon->WeaponMesh->GetSocketLocation("BottomTrace");

			FHitResult Hit;
		
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());
		
			FCollisionShape CollisionShape;
			CollisionShape.ShapeType = ECollisionShape::Line;
		
			bool bHitSomething = GetWorld()->SweepSingleByChannel(Hit, TopLocation, BottomLocation, FQuat::Identity, ECC_Visibility, CollisionShape, Params);
			
				if (Hit.bBlockingHit)
				{
					AActor* HitActor = Hit.GetActor();
					ABaseEnemyCharacter* Enemy = Cast<ABaseEnemyCharacter>(HitActor);
					
					if (HitActor && !HitActors.Contains(HitActor) && Weapon)
					{
						UGameplayStatics::ApplyDamage(HitActor, Weapon->Damage, GetOwner()->GetInstigatorController(), GetOwner(), UDamageType::StaticClass());

						if (Enemy && HitEnemySystem)
						{
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEnemySystem, BottomLocation, FRotator::ZeroRotator);
						}
						else if (!Enemy && HitActorSystem)
						{
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitActorSystem, BottomLocation, FRotator::ZeroRotator);
						}
					}
					HitActors.Add(HitActor);
				}
			// FColor DebugColor = bHitSomething ? FColor::Red : FColor::Green;
			// DrawDebugCapsule(
			// 	GetWorld(),
			// 	(TopLocation + BottomLocation) * 0.5f,
			// 	FVector::Distance(TopLocation, BottomLocation) * 0.5f,
			// 	5.0f,
			// 	FRotationMatrix::MakeFromZ(BottomLocation - TopLocation).ToQuat(),
			// 	DebugColor,
			// 	false,
			// 	5.f,
			// 	0,
			// 	1.0f
			// );	
	}
	else if (!bIsTraceOn)
	{
		HitActors.Empty();
	}
}

// ================= EVADE SYSTEM =================
void UStatsComponent::SendMovementVector(FVector2d MovementVector)
{
	SetEvadeDirection(MovementVector);

	GetWorld()->GetTimerManager().SetTimer(ResetEvadeDirectionTimerHandle, this,
		&UStatsComponent::ResetEvadeDirection, ResetEvadeDirectionTimerRate);
}

void UStatsComponent::SetEvadeDirection(FVector2d MovementVector)
{
	if (MovementVector.Y > 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Forward;
	}
	else if (MovementVector.Y < 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Backward;
	}
	else if (MovementVector.X > 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Right;
	}
	else if (MovementVector.X < 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Left;
	}
	else
	{
		EvadeDirection = EEvadeDirection::None;
	}
	
}

void UStatsComponent::Evade(ACharacter* OwnerCharacter)
{
	if (!OwnerCharacter) return;

	if (EvadeDirection == EEvadeDirection::None) return;

	bIsEvading = true;

	if (OwnerCharacter)
	{
		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

		switch (EvadeDirection)
		{
		case EEvadeDirection::None:
			EvadeSectionName = "";
			break;
		case EEvadeDirection::Evade_Forward:
			EvadeSectionName = "Evade_Forward";
			break;
		case EEvadeDirection::Evade_Backward:
			EvadeSectionName = "Evade_Backward";
			break;
		case EEvadeDirection::Evade_Right:
			EvadeSectionName = "Evade_Right";
			break;
		case EEvadeDirection::Evade_Left:
			EvadeSectionName = "Evade_Left";
			break;
		}

		if (AnimInstance)
		{
			if (EvadeMontage)
			{
				AnimInstance->Montage_Play(EvadeMontage);

				AnimInstance->Montage_JumpToSection(EvadeSectionName);

				ConsumeStamina(EvadeStaminaCost);
				
				OnMontageBlendedInEnded.BindUObject(this, &UStatsComponent::OnEvadeMontageBlendedInEnded);
				OnMontageBlendingOutStarted.BindUObject(this, &UStatsComponent::OnEvadeMontageBlendingOut);

				AnimInstance->Montage_SetBlendedInDelegate(OnMontageBlendedInEnded, EvadeMontage);
				AnimInstance->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, EvadeMontage);
			}
		}
	}
}

void UStatsComponent::OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	bIsEvading = false;
	EvadeDirection = EEvadeDirection::None;
}

void UStatsComponent::OnEvadeMontageBlendedInEnded(UAnimMontage* Montage)
{
	
}

void UStatsComponent::ResetEvadeDirection()
{
	EvadeDirection = EEvadeDirection::None;
}

// ================= SOULS SYSTEM =================
void UStatsComponent::AddPoints(int32 Amout)
{
	SoulPoint += Amout;
	OnSoulsPointsChanged.Broadcast(SoulPoint);
}

void UStatsComponent::LoseSouls()
{
	SoulPoint = 0;
	OnSoulsPointsChanged.Broadcast(SoulPoint);
}

// ================= STAMINA SYSTEM =================
void UStatsComponent::IncreaseStamina()
{
	CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + (StaminaRegenRate * 0.7f));

	OnStatsChanged.Broadcast(GetHealthPercent(), GetStaminaPercent());
	
	if (CurrentStamina >= MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	}
}

void UStatsComponent::StartRegenStamina()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &UStatsComponent::IncreaseStamina, 0.1f, true);
}

void UStatsComponent::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		bStaminaDepleted = false;
	}
}

void UStatsComponent::SetSprinting(bool bNewSprinting)
{
	bWantsToSprint = bNewSprinting;
	
	bIsSprinting = bWantsToSprint && (CanSprinting() || bIsActuallyMoving);
	
}

void UStatsComponent::UpdateMovementState(bool bIsMoving)
{
	bIsActuallyMoving = bIsMoving;
	
	bIsSprinting = bWantsToSprint && bIsActuallyMoving && CanSprinting();
}

void UStatsComponent::ConsumeStamina(float Amount)
{
	CurrentStamina = FMath::Max(0.0f, CurrentStamina - Amount);

	OnStatsChanged.Broadcast(GetHealthPercent(), GetStaminaPercent());
	
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &UStatsComponent::StartRegenStamina, StaminaRegenDelay, false);
}
