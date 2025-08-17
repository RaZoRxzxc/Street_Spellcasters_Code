
#include "Characters/BaseEnemyCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Characters/BaseCharacter.h"
#include "Components/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Projectile/BaseProjectile.h"
#include "Widgets/EnemyWidget.h"


ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("HealthComp"));
	// CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidgetComp->SetupAttachment(GetCapsuleComponent());
	HealthWidgetComp->SetVisibility(false);

	ProjectileArrow = CreateDefaultSubobject<UArrowComponent>("ProjectileArrow");
	ProjectileArrow->SetupAttachment(GetMesh());

	BlockEffectMesh = CreateDefaultSubobject<UStaticMeshComponent>("BlockEffectMesh");
	BlockEffectMesh->SetupAttachment(GetMesh());
	BlockEffectMesh->SetVisibility(false); 
	
	BlockEffect = CreateDefaultSubobject<UNiagaraComponent>("BlockEffect");
	BlockEffect->SetupAttachment(BlockEffectMesh);
	BlockEffect->SetVisibility(false);
}

void ABaseEnemyCharacter::ShowHealthWidget()
{
	HealthWidgetComp->SetVisibility(true);
}

void ABaseEnemyCharacter::ShowHealthWidgetEnd()
{
	HealthWidgetComp->SetVisibility(false);
}

void ABaseEnemyCharacter::isDead()
{
	int32 SoulsToGive = GenerateRandomSouls();
	if (!bIsDead)
	{
		bIsDead = true;
		
		if (ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
		{
			Player->StatsComponent->AddPoints(SoulsToGive);
		}
		
		GetMesh()->SetSimulatePhysics(true);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void ABaseEnemyCharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	ShowHealthWidget();
	/** Health widget end timer */
	GetWorldTimerManager().SetTimer(HealthWidgetTimer, this, &ABaseEnemyCharacter::ShowHealthWidgetEnd, 3.0f, false);
	
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatsComponent->OnDeath.AddDynamic(this, &ABaseEnemyCharacter::isDead);

	OnTakeAnyDamage.AddDynamic(this, &ABaseEnemyCharacter::OnTakeDamage);

	if (EnemyType == EEnemyType::E_Knight)
	{
		SpawnWeapon();
	}
	
	TeleportBodyEffectComp = NewObject<UParticleSystemComponent>(this);
	TeleportBodyEffectComp->SetupAttachment(GetMesh(), "spine_01");
	TeleportBodyEffectComp->SetTemplate(TeleportBodyEffect);
	TeleportBodyEffectComp->RegisterComponent();
	TeleportBodyEffectComp->SetVisibility(false);

	TeleportTrailEffectComp = NewObject<UParticleSystemComponent>(this);
	TeleportTrailEffectComp->SetupAttachment(GetMesh(), "spine_01");
	TeleportTrailEffectComp->SetTemplate(TeleportTrailEffect);
	TeleportTrailEffectComp->RegisterComponent();
	TeleportTrailEffectComp->SetVisibility(false);
}

void ABaseEnemyCharacter::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;
	OnAttackEnd.Broadcast();
}

void ABaseEnemyCharacter::SpawnProjectile()
{
	const FVector LeftSocketTransform = GetMesh()->GetSocketLocation("LeftProjectile");
	const FVector RightSocketTransform = GetMesh()->GetSocketLocation("RightProjectile");

	const FRotator LeftRotation = ProjectileArrow->GetComponentRotation();
	const FRotator RightRotation = ProjectileArrow->GetComponentRotation();
	
	if (bIsLeftHand)
	{
		bIsLeftHand = false;
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, RightSocketTransform,RightRotation);
	}
	else
	{
		bIsLeftHand = true;
		GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, LeftSocketTransform, LeftRotation);
	}
}

void ABaseEnemyCharacter::PlayRandomAttackAnimation()
{
	if (AttackAnimations.Num() == 0) return;

	int32 NewAttackIndex;
	do
	{
		NewAttackIndex = FMath::RandRange(0, AttackAnimations.Num() - 1);
	} while (AttackAnimations.Num() > 1 && NewAttackIndex == LastAttackIndex);

	LastAttackIndex = NewAttackIndex;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackAnimations[NewAttackIndex])
	{
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &ABaseEnemyCharacter::OnAttackMontageEnd);
		
		AnimInstance->Montage_Play(AttackAnimations[NewAttackIndex]);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackAnimations[NewAttackIndex]);
	}
}

void ABaseEnemyCharacter::AttackEnd()
{
	bIsAttacking = false;
}

int32 ABaseEnemyCharacter::GenerateRandomSouls() const
{
	int32 RandomSouls = UKismetMathLibrary::RandomIntegerInRange(MinSoulsReward, MaxSoulsReward);

	return FMath::RoundToInt(RandomSouls * SoulsMultiplier);
}

void ABaseEnemyCharacter::MeleeAttack_Implementation()
{
	Attack();
}

void ABaseEnemyCharacter::Attack()
{
	if (CanAttack() && AttackAnimations.Num() > 0)
	{
		bIsAttacking = true;
		PlayRandomAttackAnimation();
	}
}

void ABaseEnemyCharacter::SpawnWeapon()
{
	ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, GetMesh()->GetSocketTransform("Weapon_Socket"));
	
	if (NewWeapon)
	{
		CurrentWeapon = NewWeapon;
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Weapon_Socket");
		StatsComponent->Weapon = CurrentWeapon;
	}
}

void ABaseEnemyCharacter::StartBlock_Implementation()
{
	StartBlocking();
}

void ABaseEnemyCharacter::EndBlock_Implementation()
{
	EndBlocking();
}

void ABaseEnemyCharacter::StartBlocking()
{
	if (!bIsBlocking)
	{
		bIsBlocking = true;
		
		StatsComponent->StartBlocking();
		
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(BlockMontage);
		}

		if (EnemyType == EEnemyType::E_Wizard)
		{
			BlockEffectMesh->SetVisibility(true);
			BlockEffect->SetVisibility(true);
		}
	
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
}

void ABaseEnemyCharacter::EndBlocking()
{
	if (bIsBlocking)
	{
		bIsBlocking = false;
		StatsComponent->EndBlocking();
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.2f, BlockMontage);
		}

		if (EnemyType == EEnemyType::E_Wizard)
		{
			BlockEffectMesh->SetVisibility(false);
			BlockEffect->SetVisibility(false);
		}
	}
}

void ABaseEnemyCharacter::Teleport(const FVector& location)
{
	if (!bIsTeleporting)
	{
		bIsTeleporting = true;
		bIsAttacking = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying, 0);
		GetCharacterMovement()->MaxFlySpeed = 5000.0f;
		GetCharacterMovement()->MaxAcceleration = 99999.0f;

		GetMesh()->SetVisibility(false, true);

		if (bIsBlocking)
		{
			EndBlocking();
		}
		
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		TeleportBodyEffectComp->SetVisibility(true);
		TeleportBodyEffectComp->Activate(true);
		
		TeleportTrailEffectComp->SetVisibility(true);
		TeleportTrailEffectComp->Activate(true);
		
		if (BlockEffectMesh && BlockEffectMesh->IsVisible())
		{
			BlockEffectMesh->SetVisibility(false);
		}
		if (BlockEffect && BlockEffect->IsVisible())
		{
			BlockEffect->SetVisibility(false);
		}
		
		if (AAIController* aiController = Cast<AAIController>(GetController()))
		{
			aiController->MoveToLocation(location, 15.0f, false);
		
			aiController->ReceiveMoveCompleted.AddDynamic(this, &ABaseEnemyCharacter::OnMoveComplete);
		}
	}
}

void ABaseEnemyCharacter::TeleportEnd()
{
	if (bIsTeleporting)
	{
		bIsTeleporting = false;
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->MaxAcceleration = 2048.0f;

		GetMesh()->SetVisibility(true, false);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
		OnTeleportEnd.Broadcast();

		TeleportBodyEffectComp->SetVisibility(false);
		TeleportBodyEffectComp->Deactivate();
		
		TeleportTrailEffectComp->SetVisibility(false);
		TeleportTrailEffectComp->Deactivate();

		if (BlockEffectMesh)
		{
			BlockEffectMesh->SetVisibility(false);
		}
		if (BlockEffect)
		{
			BlockEffect->SetVisibility(false);
		}
	}
}

void ABaseEnemyCharacter::OnMoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		TeleportEnd();
	}
}


void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HealthWidgetComp)
	{
		UEnemyWidget* EnemyWidget = Cast<UEnemyWidget>(HealthWidgetComp->GetWidget());
		if (EnemyWidget)
		{
			EnemyWidget->UpdateFrontHealthBar(StatsComponent->GetHealthPercent());
		}
	}
}

void ABaseEnemyCharacter::Evade()
{
	if (!StatsComponent || StatsComponent->GetIsEvading()) return;

	FVector PlayerDirection = (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetActorLocation().GetSafeNormal2D());
	FVector2D EvadeDirection = FVector2D(PlayerDirection.X, PlayerDirection.Y);

	StatsComponent->SendMovementVector(EvadeDirection);
	StatsComponent->Evade(this);
}


