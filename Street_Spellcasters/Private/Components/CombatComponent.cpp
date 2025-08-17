// // Fill out your copyright notice in the Description page of Project Settings.
//
//
// #include "Components/CombatComponent.h"
// #include "NiagaraFunctionLibrary.h"
// #include "Characters/BaseEnemyCharacter.h"
// #include "Kismet/GameplayStatics.h"
// #include "Characters/BaseCharacter.h"
// #include "Projectile/BaseProjectile.h"
// #include "Weapon/BaseWeapon.h"
//
// UCombatComponent::UCombatComponent()
// {
// 	PrimaryComponentTick.bCanEverTick = true;
// 	
// 	bIsTraceOn = false;
//
// 	EvadeSectionName = "";
// 	bIsEvading = false;
// 	EvadeDirection = EEvadeDirection::None;
// 	ResetEvadeDirectionTimerRate = 0.1f;
//
//
// 	MaxStamina = 100.0f;
// 	CurrentStamina = MaxStamina;
// 	StaminaRegenRate = 15.0f;
// 	StaminaRegenDelay = 1.5f;
// 	AttackStaminaCost = 10.0f;
// 	SprintStaminaCost = 5.0f;
// 	EvadeStaminaCost = 5.0f;
// }
//
// void UCombatComponent::CallToggleWeaponTrace_Implementation(bool bIsWeaponTraceOn)
// {
// 	bIsTraceOn = bIsWeaponTraceOn;
// }
//
// void UCombatComponent::MeleeTrace()
// {
// 	if (bIsTraceOn && Weapon)
// 	{
// 			FVector TopLocation = Weapon->WeaponMesh->GetSocketLocation("TopTrace");
// 			FVector BottomLocation = Weapon->WeaponMesh->GetSocketLocation("BottomTrace");
//
// 			FHitResult Hit;
// 		
// 			FCollisionQueryParams Params;
// 			Params.AddIgnoredActor(GetOwner());
// 		
// 			FCollisionShape CollisionShape;
// 			CollisionShape.ShapeType = ECollisionShape::Line;
// 		
// 			bool bHitSomething = GetWorld()->SweepSingleByChannel(Hit, TopLocation, BottomLocation, FQuat::Identity, ECC_Visibility, CollisionShape, Params);
// 			
// 				if (Hit.bBlockingHit)
// 				{
// 					AActor* HitActor = Hit.GetActor();
// 					ABaseEnemyCharacter* Enemy = Cast<ABaseEnemyCharacter>(HitActor);
//
// 					Damage = FMath::RandRange(20, 35);
// 					
// 					if (HitActor && !HitActors.Contains(HitActor))
// 					{
// 						UGameplayStatics::ApplyDamage(HitActor, Damage, GetOwner()->GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
//
// 						if (Enemy && HitEnemySystem)
// 						{
// 							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEnemySystem, BottomLocation, FRotator::ZeroRotator);
// 						}
// 						else if (!Enemy && HitActorSystem)
// 						{
// 							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitActorSystem, BottomLocation, FRotator::ZeroRotator);
// 						}
// 					}
// 					HitActors.Add(HitActor);
// 				}
// 			// FColor DebugColor = bHitSomething ? FColor::Red : FColor::Green;
// 			// DrawDebugCapsule(
// 			// 	GetWorld(),
// 			// 	(TopLocation + BottomLocation) * 0.5f,
// 			// 	FVector::Distance(TopLocation, BottomLocation) * 0.5f,
// 			// 	5.0f,
// 			// 	FRotationMatrix::MakeFromZ(BottomLocation - TopLocation).ToQuat(),
// 			// 	DebugColor,
// 			// 	false,
// 			// 	5.f,
// 			// 	0,
// 			// 	1.0f
// 			// );	
// 	}
// 	else if (!bIsTraceOn)
// 	{
// 		HitActors.Empty();
// 	}
// }
//
// void UCombatComponent::SendMovementVector(FVector2d MovementVector)
// {
// 	SetEvadeDirection(MovementVector);
//
// 	GetWorld()->GetTimerManager().SetTimer(ResetEvadeDirectionTimerHandle, this,
// 		&UCombatComponent::ResetEvadeDirection, ResetEvadeDirectionTimerRate);
// }
//
// void UCombatComponent::SetEvadeDirection(FVector2d MovementVector)
// {
// 	if (MovementVector.Y > 0)
// 	{
// 		EvadeDirection = EEvadeDirection::Evade_Forward;
// 	}
// 	else if (MovementVector.Y < 0)
// 	{
// 		EvadeDirection = EEvadeDirection::Evade_Backward;
// 	}
// 	else if (MovementVector.X > 0)
// 	{
// 		EvadeDirection = EEvadeDirection::Evade_Right;
// 	}
// 	else if (MovementVector.X < 0)
// 	{
// 		EvadeDirection = EEvadeDirection::Evade_Left;
// 	}
// 	else
// 	{
// 		EvadeDirection = EEvadeDirection::None;
// 	}
// 	
// }
//
// void UCombatComponent::Evade(ACharacter* OwnerCharacter)
// {
// 	if (!OwnerCharacter) return;
//
// 	if (EvadeDirection == EEvadeDirection::None) return;
//
// 	bIsEvading = true;
//
// 	if (OwnerCharacter)
// 	{
// 		UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
//
// 		switch (EvadeDirection)
// 		{
// 		case EEvadeDirection::None:
// 			EvadeSectionName = "";
// 			break;
// 		case EEvadeDirection::Evade_Forward:
// 			EvadeSectionName = "Evade_Forward";
// 			break;
// 		case EEvadeDirection::Evade_Backward:
// 			EvadeSectionName = "Evade_Backward";
// 			break;
// 		case EEvadeDirection::Evade_Right:
// 			EvadeSectionName = "Evade_Right";
// 			break;
// 		case EEvadeDirection::Evade_Left:
// 			EvadeSectionName = "Evade_Left";
// 			break;
// 		}
//
// 		if (AnimInstance)
// 		{
// 			if (EvadeMontage)
// 			{
// 				AnimInstance->Montage_Play(EvadeMontage);
//
// 				AnimInstance->Montage_JumpToSection(EvadeSectionName);
//
// 				ConsumeStamina(EvadeStaminaCost);
// 				
// 				OnMontageBlendedInEnded.BindUObject(this, &UCombatComponent::OnEvadeMontageBlendedInEnded);
// 				OnMontageBlendingOutStarted.BindUObject(this, &UCombatComponent::OnEvadeMontageBlendingOut);
//
// 				AnimInstance->Montage_SetBlendedInDelegate(OnMontageBlendedInEnded, EvadeMontage);
// 				AnimInstance->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, EvadeMontage);
// 			}
// 		}
// 	}
// }
//
// void UCombatComponent::OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
// {
// 	bIsEvading = false;
// 	EvadeDirection = EEvadeDirection::None;
// }
//
// void UCombatComponent::OnEvadeMontageBlendedInEnded(UAnimMontage* Montage)
// {
// 	
// }
//
// void UCombatComponent::ResetEvadeDirection()
// {
// 	EvadeDirection = EEvadeDirection::None;
// }
//
// void UCombatComponent::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// }
//
// void UCombatComponent::IncreaseStamina()
// {
// 	CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + (StaminaRegenRate * 0.7f));
//
// 	OnStaminaChanged.Broadcast(GetStaminaPercent());
//
// 	if (CurrentStamina >= MaxStamina)
// 	{
// 		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
// 	}
// }
//
// void UCombatComponent::StartRegenStamina()
// {
// 	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &UCombatComponent::IncreaseStamina, 0.1f, true);
// }
//
// void UCombatComponent::StopSprinting()
// {
// 	if (bIsSprinting)
// 	{
// 		bIsSprinting = false;
// 		bStaminaDepleted = false;
// 	}
// }
//
// void UCombatComponent::SetSprinting(bool bNewSprinting)
// {
// 	if (bNewSprinting && !CanSprinting())
// 	{
// 		bNewSprinting = false;
// 	}
// 	
// 	bIsSprinting = bNewSprinting;
// 	
// }
//
// void UCombatComponent::ConsumeStamina(float Amount)
// {
// 	CurrentStamina = FMath::Max(0.0f, CurrentStamina - Amount);
//
// 	OnStaminaChanged.Broadcast(GetStaminaPercent());
// 	
// 	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimer);
// 	GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimer, this, &UCombatComponent::StartRegenStamina, StaminaRegenDelay, false);
// }
//
//
// // Called every frame
// void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	MeleeTrace();
//
// 	if (bIsSprinting)
// 	{
// 		if (CanSprinting())
// 		{
// 			const float StaminaDrain = SprintStaminaCost * DeltaTime;
// 			ConsumeStamina(StaminaDrain);
// 			
// 			if (FMath::IsNearlyZero(CurrentStamina) && !bStaminaDepleted)
// 			{
// 				StopSprinting();
// 				SetSprinting(false);
// 			}
// 		}
// 		else
// 		{
// 			StopSprinting();
// 		}
// 	}
// 	else if (CurrentStamina >= MaxStamina * 0.2f)
// 	{
// 		bStaminaDepleted = false;
// 	}
// 	
// }
//
// void UCombatComponent::AddPoints(int32 Amout)
// {
// 	SoulPoint += Amout;
// 	OnSoulsPointsChanged.Broadcast(SoulPoint);
// }
//
// void UCombatComponent::LoseSouls()
// {
// 	SoulPoint = 0;
// 	OnSoulsPointsChanged.Broadcast(SoulPoint);
// }
//
