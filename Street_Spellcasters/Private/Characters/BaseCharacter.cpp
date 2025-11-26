
#include "Characters/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "NiagaraComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "Widgets/MiniMapWidget.h"
#include "Components/StatsComponent.h"
#include "Projectile/BaseProjectile.h"
#include "Characters/BaseEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BaseWeapon.h"
#include "Widgets/PlayerHUD.h"
#include "Interface/InteractInterface.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(GetCapsuleComponent());

	/** Camera */
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControlRotation = false;

	// Setup Face elements to character mesh
	HairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	HairMesh->SetupAttachment(GetMesh(), TEXT("head"));
	
	FaceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Face"));
	FaceMesh->SetupAttachment(GetMesh(), TEXT("head"));
	
	FrecklesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Freckles"));
	FrecklesMesh->SetupAttachment(GetMesh(), TEXT("head"));
	
	BrowsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Brows"));
	BrowsMesh->SetupAttachment(GetMesh(), TEXT("head"));
	
	EyesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Eyes"));
	EyesMesh->SetupAttachment(GetMesh()), TEXT("head");

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	WalkSpeed = 300.0f;
	SprintSpeed = 900.f;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>("StatsComponent");

	BlockEffectMesh = CreateDefaultSubobject<UStaticMeshComponent>("BlockEffectMesh");
	BlockEffectMesh->SetupAttachment(GetMesh());
	BlockEffect = CreateDefaultSubobject<UNiagaraComponent>("BlockEffect");
	BlockEffect->SetupAttachment(BlockEffectMesh);

	PotionMesh = CreateDefaultSubobject<UStaticMeshComponent>("PotionMesh");
	PotionMesh->SetupAttachment(GetMesh());
	PotionMesh->SetHiddenInGame(true);
	
	if (CharacterType == ECharacterType::E_Magic)
	{
		BlockEffectMesh->SetVisibility(false); 
		BlockEffect->SetVisibility(false);
	}

	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("PerceptionStimuliSource");
	if (PerceptionStimuliSource)
	{
		PerceptionStimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		PerceptionStimuliSource->RegisterWithPerceptionSystem();
	}

	MapWidgetClass = nullptr;
	MapWidget = nullptr;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	StatsComponent->OnDeath.AddDynamic(this, &ABaseCharacter::isDead);
	
	SpawnWeapon();

	bIsAttacking = GetStatsComp()->GetIsAttack();

	if (PotionMesh)
	{
		PotionMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Potion_Socket");
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const bool bIsMovingNow = !GetVelocity().IsNearlyZero(10.0f);
	if (StatsComponent)
	{
		StatsComponent->UpdateMovementState(bIsMovingNow);
	}
	
}

void ABaseCharacter::Heal()
{
	if (StatsComponent->Health < StatsComponent->MaxHealth && !bIsHeal)
	{
		bIsHeal = true;
		StatsComponent->AddHealth();
		PlayAnimMontage(HealFlaskMontage);
	}
}

void ABaseCharacter::HealEnd()
{
	bIsHeal = false;
}

void ABaseCharacter::SpawnPotionMesh()
{
	
	if (PotionMesh && CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		PotionMesh->SetHiddenInGame(false);
	}
}

void ABaseCharacter::DestroyPotionMesh()
{
	if (PotionMesh && CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(false);
		PotionMesh->SetHiddenInGame(true);
		HealEnd();
	}
}

void ABaseCharacter::ToggleMap()
{
	if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		PlayerHUD->ToggleMap();
	}
}

void ABaseCharacter::Jump()
{
	if (!bIsJumping && !bIsAttacking && !bIsHeal)
	{
		bIsJumping = true;
		Super::Jump();
	}
}

void ABaseCharacter::StopJumping()
{
	if (bIsJumping)
	{
		bIsJumping = false;
		Super::StopJumping();
	}
}

// Death function
void ABaseCharacter::isDead()
{
		bIsDead = true;
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (DeathMontage)
		{
			AnimInstance->Montage_Play(DeathMontage);
		}

		StatsComponent->LoseSouls();
	
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->Deactivate();
		DisableInput(PlayerController);
}

// Evade function
void ABaseCharacter::Evade(const FInputActionValue& Value)
{
	bEvadeButtomPressed = Value.Get<bool>();

	if (!StatsComponent) return;

	if (!StatsComponent->GetIsEvading() && bEvadeButtomPressed && StatsComponent->CanEvade() && !bIsHeal)
	{
		if (StatsComponent)
		{
			StatsComponent->Evade(this);
		}
	}
}
// Movement function
void ABaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (StatsComponent)
		StatsComponent->SendMovementVector(MovementVector);

	if (StatsComponent && StatsComponent->GetIsEvading()) return;
	
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

// Mouse movement function
void ABaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookDirection = Value.Get<FVector2D>();
	
	float SensitivityMultiplier = 1.0f;
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.MouseSensitivity")))
		SensitivityMultiplier = CVar->GetFloat();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X * SensitivityMultiplier);
		AddControllerPitchInput(LookDirection.Y * SensitivityMultiplier);
	}
}

void ABaseCharacter::StartSprint()
{
	if (!bIsSprint && StatsComponent &&StatsComponent->CanSprinting() && !bIsHeal)
	{
		bIsSprint = true;
		StatsComponent->SetSprinting(bIsSprint);
		
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else if (!StatsComponent->CanSprinting())
	{
		StopSprint();
	}
}

void ABaseCharacter::UpdateInputSensitivity()
{
	float MouseSensitivityMultiplier = 1.0f;
	float ControllerSensitivityMultiplier = 1.0f;
    
	if (IConsoleVariable* MouseCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.MouseSensitivity")))
		MouseSensitivityMultiplier = MouseCVar->GetFloat();
        
	if (IConsoleVariable* ControllerCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.ControllerSensitivity")))
		ControllerSensitivityMultiplier = ControllerCVar->GetFloat();
}

void ABaseCharacter::StopSprint()
{
	if (bIsSprint && StatsComponent)
	{
		bIsSprint = false;
		StatsComponent->SetSprinting(bIsSprint);
		
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	
}

// Attack function
void ABaseCharacter::Attack()
{
	if (CanAttack() && AttackAnimations.Num() > 0 && StatsComponent->CanAttacking() && !bIsHeal)
	{
		bIsAttacking = true;
		PlayAttackAnim();

		StatsComponent->ConsumeStamina(StatsComponent->AttackStaminaCost);
	}
	
	if (StatsComponent->bIsBlocking || StatsComponent->GetIsEvading() || StatsComponent->bIsTakeHit)
	{
		OnAttackEnd();
	}
}

// Play attack animation
void ABaseCharacter::PlayAttackAnim()
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
		AnimInstance->Montage_Play(AttackAnimations[NewAttackIndex]);
	}
}

void ABaseCharacter::EnableAttack()
{
	bIsAttacking = true;
}

// Attack end delegate for animation instance
void ABaseCharacter::AttackEndDelegate(UAnimMontage* Montage, bool bInterrupted)
{
	OnAttackEnd();
}

// attack end
void ABaseCharacter::OnAttackEnd()
{
	bIsAttacking = false;
}

void ABaseCharacter::IsPlayerAttack_Implementation(bool& bIsAttack)
{
	bIsAttack = bIsAttacking;
}

void ABaseCharacter::isPlayerDead_Implementation(bool& IsDead)
{
	IsDead = bIsDead;
}

void ABaseCharacter::HandleMove_Implementation(const FInputActionValue& Value)
{
	Move(Value);
}

void ABaseCharacter::HandleLook_Implementation(const FInputActionValue& Value)
{
	Look(Value);
}

void ABaseCharacter::HandleJump_Implementation()
{
	Jump();
}

void ABaseCharacter::HandleStopJumping_Implementation()
{
	StopJumping();
}

void ABaseCharacter::HandleSprintStart_Implementation()
{
	StartSprint();
}

void ABaseCharacter::HandleSprintStop_Implementation()
{
	StopSprint();
}

void ABaseCharacter::HandleAttack_Implementation()
{
	Attack();
}

void ABaseCharacter::HandleBlockStart_Implementation()
{
	StartBlocking();
}

void ABaseCharacter::HandleBlockEnd_Implementation()
{
	EndBlocking();
}

void ABaseCharacter::HandleEvade_Implementation(const FInputActionValue& Value)
{
	Evade(Value);
}

void ABaseCharacter::HandleInteract_Implementation()
{
	Interact();
}

void ABaseCharacter::HandleHeal_Implementation()
{
	Heal();
}

void ABaseCharacter::HandleToggleMap_Implementation()
{
	ToggleMap();
}

void ABaseCharacter::StartBlocking()
{
	StatsComponent->StartBlocking();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && !bIsHeal)
	{
		AnimInstance->Montage_Play(BlockMontage);
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;

		if (CharacterType == ECharacterType::E_Magic)
		{
			BlockEffectMesh->SetVisibility(true);
			BlockEffect->SetVisibility(true);
		}
	}
	
	
}

void ABaseCharacter::EndBlocking()
{
	StatsComponent->EndBlocking();
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.2f, BlockMontage);
	}

	if (CharacterType == ECharacterType::E_Magic)
	{
		BlockEffectMesh->SetVisibility(false);
		BlockEffect->SetVisibility(false);
	}
}

// Spawn weapon in hand
void ABaseCharacter::SpawnWeapon()
{
	ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, GetMesh()->GetSocketTransform("Weapon_Socket"));
	
	if (NewWeapon)
	{
		CurrentWeapon = NewWeapon;
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Weapon_Socket");
		StatsComponent->Weapon = CurrentWeapon;
	}
}

void ABaseCharacter::Interact()
{
	if (CurrentInteractable && CurrentInteractable->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		IInteractInterface::Execute_InteractWith(CurrentInteractable, this);
		//UE_LOG(LogTemp, Log, TEXT("Interacted with %s"), *CurrentInteractable->GetName());
	}
}



