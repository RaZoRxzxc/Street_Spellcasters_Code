//
// #include "Components/HealthComponent.h"
//
// #include "Characters/BaseCharacter.h"
// #include "Components/CapsuleComponent.h"
// #include "Kismet/GameplayStatics.h"
//
// UHealthComponent::UHealthComponent()
// {
// 	PrimaryComponentTick.bCanEverTick = false;
//
// 	DamageBlockMultiplier = 0.9f;
//
// 	HitDirectionName = "";
// 	HitDirection = EHitDirection::None;
// 	
// 	MaxHealth = 100;
// 	Health = MaxHealth;
// }
//
// void UHealthComponent::StartBlock()
// {
// 	bIsBlocking = true;
// }
//
// void UHealthComponent::EndBlock()
// {
// 	bIsBlocking = false;
// }
//
// void UHealthComponent::CalculateHitDirection(FVector HitOrigin)
// {
// 	const AActor* OwnerActor = GetOwner();
// 	
// 	FVector HitDirectionVector = HitOrigin - OwnerActor->GetActorLocation();
// 	HitDirectionVector.Z = 0.0f;
// 	HitDirectionVector.Normalize();
//
// 	const FVector Forward = OwnerActor->GetActorForwardVector();
// 	const FVector Right = OwnerActor->GetActorRightVector();
//
// 	const float ForwardDot = FVector::DotProduct(Forward, HitDirectionVector);
// 	const float RightDot = FVector::DotProduct(Right, HitDirectionVector);
//
// 	if (ForwardDot > 0.5f)
// 	{
// 		HitDirection = EHitDirection::Hit_Forward;
// 	}
// 	else if (ForwardDot < -0.5f)
// 	{
// 		HitDirection = EHitDirection::Hit_Backward;
// 	}
// 	else if (RightDot > 0.f)
// 	{
// 		HitDirection = EHitDirection::Hit_Right;
// 	}
// 	else if (RightDot < 0.f)
// 	{
// 		HitDirection = EHitDirection::Hit_Left;
// 	}
// 	else
// 	{
// 		HitDirection = EHitDirection::None;
// 	}
// }
//
// void UHealthComponent::PlayHitAnimation(bool bIsBlockedHit)
// {
// 	if (HitDirection == EHitDirection::None) return;
//
// 	const ACharacter* Character = Cast<ACharacter>(GetOwner());
// 	if (!Character) return;
//
// 	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
// 	if (!AnimInstance) return;
//
// 	UAnimMontage* MontageToPlay = bIsBlockedHit ? BlockHitMontage : HitMontage;
// 	
//
// 	switch (HitDirection)
// 	{
// 	case EHitDirection::None:
// 		HitDirectionName = "";
// 		break;
// 	case EHitDirection::Hit_Forward:
// 		HitDirectionName = "Hit_Forward";
// 		break;
// 	case EHitDirection::Hit_Backward:
// 		HitDirectionName = "Hit_Backward";
// 		break;
// 	case EHitDirection::Hit_Left:
// 		HitDirectionName = "Hit_Left";
// 		break;
// 	case EHitDirection::Hit_Right:
// 		HitDirectionName = "Hit_Right";
// 		break;
// 	}
//
// 	UE_LOG(LogTemp, Warning, TEXT("Playing Montage: %s, Section: %s"), 
// 	   *GetNameSafe(MontageToPlay), 
// 	   *HitDirectionName.ToString());
// 	
// 	bIsTakeHit = true;
// 	AnimInstance->Montage_Play(MontageToPlay);
// 	AnimInstance->Montage_JumpToSection(HitDirectionName);
//
// 	FOnMontageEnded OnMontageEnded;
// 	OnMontageEnded.BindUObject(this, &UHealthComponent::OnHitMontageEnded);
// 	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, MontageToPlay);
// }
//
// void UHealthComponent::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
// {
// 	bIsTakeHit = false;
// 	HitDirection = EHitDirection::None;
// }
//
// void UHealthComponent::ApplyDamage(AActor* DamagedActor,float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
// {
// 	if (Damage <= 0.0f && IsDead()) return;
// 	
// 	float ActualDamage = Damage;
// 	bool bWasBlocked = false;
// 	
// 	if (bIsBlocking)
// 	{
// 		const FVector DamageDirection = (DamageCauser->GetActorLocation() - DamagedActor->GetActorLocation()).GetSafeNormal();
// 		const FVector PlayerForwardVector = DamagedActor->GetActorForwardVector();
// 		
// 		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PlayerForwardVector, DamageDirection)));
// 		
// 		if (Angle <= 90.0f)
// 		{
// 			ActualDamage *= (1.0f - DamageBlockMultiplier);
// 			bWasBlocked = true;
// 		}
// 	}
//
// 	CalculateHitDirection(DamageCauser->GetActorLocation());
// 	PlayHitAnimation(bWasBlocked);
// 	
// 	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
// 	
// 	OnHealthChanged.Broadcast(Health, MaxHealth);
//
// 	if (IsDead())
// 	{
// 		OnDeath.Broadcast();
// 	}
// }
//
// void UHealthComponent::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// 	if (AActor* MyTarget = GetOwner())
// 	{
// 		MyTarget->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::ApplyDamage);
// 	}
// }
//
// void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// 	
// }
//
