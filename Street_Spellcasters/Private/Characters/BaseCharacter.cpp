
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
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControlRotation = false;

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
		bIsHeal = false;
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

// Spawn projectile in hands
void ABaseCharacter::SpawnProjectile()
{
	// Sockets location on character mesh
	FVector LeftSpawnLocation = GetMesh()->GetSocketLocation("LeftProjectile");
	FVector RightSpawnLocation = GetMesh()->GetSocketLocation("RightProjectile");

	// Camera rotation and location 
	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// The direction of the projectile's flight from the character's hands to the center of the screen
	FVector TargetLocation = CameraLocation + CameraRotation.Vector() * 2000.0f;
	
	FVector LeftShootDirection = (TargetLocation - LeftSpawnLocation).GetSafeNormal();
	FVector RightShootDirection = (TargetLocation - RightSpawnLocation).GetSafeNormal();
	
	ABaseProjectile* Projectile = nullptr;

	// Check on left hand and spawn projectile on left or right hands when player attacking
	if (bIsLeftHand)
	{
		bIsLeftHand = false;
		Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, RightSpawnLocation, RightShootDirection.Rotation());
	}
	else
	{
		bIsLeftHand = true;
		Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, LeftSpawnLocation, LeftShootDirection.Rotation());
	}

	// Set projectile damage
	if (ProjectileActor && CurrentWeapon)
	{
		Projectile->SetDamage(CurrentWeapon->Damage);
	}
}

// Spawn Projectile for magic staff weapon
void ABaseCharacter::SpawnStaffProjectile()
{
	// Find closest enemy
	AActor* TargetEnemy = FindClosestEnemy();

	// Spawn projectile location  
	FVector SpawnLocation = CurrentWeapon ? CurrentWeapon->WeaponMesh->GetSocketLocation(StaffProjectileSocket) : GetActorLocation();
    
	FRotator SpawnRotation;
	FVector ShootDirection;

	// Projectile flies into the nearest enemy
	if (TargetEnemy)
	{
		ShootDirection = (TargetEnemy->GetActorLocation() - SpawnLocation).GetSafeNormal();
	}
	else
	{
		// Projectile flies forward when there are no enemies nearby
		
		FVector CameraLocation;
		FRotator CameraRotation;
        
		if (Controller)
		{
			Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);
		}
		else
		{
			CameraLocation = GetActorLocation();
			CameraRotation = GetActorRotation();
		}
        
		ShootDirection = CameraRotation.Vector();
	}

	SpawnRotation = ShootDirection.Rotation();
	
	if (!ProjectileActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileActor is not set!"));
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	ABaseProjectile* Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, SpawnLocation, SpawnRotation, SpawnParams);
    
	if (Projectile)
	{
		Projectile->SetOwner(this);
		Projectile->SetDamage(CurrentWeapon->Damage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn projectile!"));
	}
}

// Find closest enemy in player view radius
AActor* ABaseCharacter::FindClosestEnemy() const
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemyCharacter::StaticClass(), FoundEnemies);

	AActor* ClosestEnemy = nullptr;
	float MinDistance = FLT_MAX;
	FVector ViewLocation;
	FRotator ViewRotation;
	
	if (Controller)
	{
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetActorLocation();
		ViewRotation = GetActorRotation();
	}

	FVector ViewDirection = ViewRotation.Vector();

	for (AActor* Enemy : FoundEnemies)
	{
		FVector EnemyLocation = Enemy->GetActorLocation();
		FVector ToEnemy = (EnemyLocation - ViewLocation).GetSafeNormal();
		
		float DotProduct = FVector::DotProduct(ViewDirection, ToEnemy);
		float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
		
		float Distance = FVector::Dist(ViewLocation, EnemyLocation);
		
		if (Angle <= TargetingConeAngle * 0.5f && Distance <= TargetingDistance)
		{
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestEnemy = Enemy;
			}
		}
	}

	return ClosestEnemy;
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
	//IInputHandlerInterface::HandleMove_Implementation(Value);
	Move(Value);
}

void ABaseCharacter::HandleLook_Implementation(const FInputActionValue& Value)
{
	//IInputHandlerInterface::HandleLook_Implementation(Value);
	Look(Value);
}

void ABaseCharacter::HandleJump_Implementation()
{
	//IInputHandlerInterface::HandleJump_Implementation();
	Jump();
}

void ABaseCharacter::HandleStopJumping_Implementation()
{
	//IInputHandlerInterface::HandleStopJumping_Implementation();
	StopJumping();
}

void ABaseCharacter::HandleSprintStart_Implementation()
{
	//IInputHandlerInterface::HandleSprintStart_Implementation();
	StartSprint();
}

void ABaseCharacter::HandleSprintStop_Implementation()
{
	//IInputHandlerInterface::HandleSprintStop_Implementation();
	StopSprint();
}

void ABaseCharacter::HandleAttack_Implementation()
{
	//IInputHandlerInterface::HandleAttack_Implementation();
	Attack();
}

void ABaseCharacter::HandleBlockStart_Implementation()
{
	//IInputHandlerInterface::HandleBlockStart_Implementation();
	StartBlocking();
}

void ABaseCharacter::HandleBlockEnd_Implementation()
{
	//IInputHandlerInterface::HandleBlockEnd_Implementation();
	EndBlocking();
}

void ABaseCharacter::HandleEvade_Implementation(const FInputActionValue& Value)
{
	//IInputHandlerInterface::HandleEvade_Implementation(Value);
	Evade(Value);
}

void ABaseCharacter::HandleInteract_Implementation()
{
	//IInputHandlerInterface::HandleInteract_Implementation();
	Interact();
}

void ABaseCharacter::HandleHeal_Implementation()
{
	//IInputHandlerInterface::HandleHeal_Implementation();
	Heal();
}

void ABaseCharacter::HandleToggleMap_Implementation()
{
	//IInputHandlerInterface::HandleToggleMap_Implementation();
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
		UE_LOG(LogTemp, Log, TEXT("Interacted with %s"), *CurrentInteractable->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No interactable object nearby"));
	}
}



