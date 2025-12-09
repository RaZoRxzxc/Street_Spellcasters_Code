// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/CombatInterface.h"
#include "StatsComponent.generated.h"

class ABaseWeapon;

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	None UMETA(DisplayName = "None"),
	Hit_Forward UMETA(DisplayName = "Forward"),
	Hit_Backward UMETA(DisplayName = "Backward"),
	Hit_Left UMETA(DisplayName = "Left"),
	Hit_Right UMETA(DisplayName = "Right")
	
};

UENUM(BlueprintType)
enum class EEvadeDirection : uint8
{
	None UMETA(DisplayName = "None"),
	Evade_Forward UMETA(DisplayName = "Evade_Forward"),
	Evade_Backward UMETA(DisplayName = "Evade_Backward"),
	Evade_Right UMETA(DisplayName = "Evade_Right"),
	Evade_Left UMETA(DisplayName = "Evade_Left")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatsChanged, float, CurrentHealth, float, StaminaPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageChanged, float, CurrentDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChanged, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulsChanged, int32, SoulsAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlasksChanged, int32, Flasks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STREET_SPELLCASTERS_API UStatsComponent : public UActorComponent, public ICombatInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

	// Stats changed delegate
	UPROPERTY(BlueprintAssignable)
	FOnStatsChanged OnStatsChanged;
	
protected:

	void InterruptActions();
	
	// Begin play
	virtual void BeginPlay() override;

public:

	// Tick component
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ================== HEALTH SYSTEM ==================

	// Enum class hit direction
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
	EHitDirection HitDirection;

	// Get is dead
	UFUNCTION(BlueprintCallable)
	bool IsDead() const {return Health <= 0.0f;}

	// Get character health  
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const {return Health / MaxHealth;}

	// On character dead delegate
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintReadWrite)
	float DamageBlockMultiplier;

	UPROPERTY(BlueprintReadWrite)
	bool bIsBlocking = false;

	// Start blocking function
	UFUNCTION()
	void StartBlocking();

	// End blocking function
	UFUNCTION()
	void EndBlocking();
	
protected:

	// Calculate hit direction function
	UFUNCTION()
	void CalculateHitDirection(FVector HitOrigin);

	// Play hit reaction animation function
	void PlayHitAnimation(bool bIsBlockedHit);

private:

	// Hit reaction montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage;

	// Block hit reaction montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* BlockHitMontage;

	// Hit direction name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
	FName HitDirectionName;

	// On hit montage ended
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool bIsAttack = false;
public:
	bool GetIsAttack() const {return bIsAttack;}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hit_Reaction")
	bool bIsTakeHit = false;

	// Current health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float Health;

	// Max health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth;

	// Restore health and stamina when player overlap campfire collision
	UFUNCTION()
	void RestoreStatsToMax();

	UFUNCTION()
	void AddHealth();

	UPROPERTY(BlueprintReadOnly, Category= "Health")
	bool bIsHeal = false;

	UPROPERTY(BlueprintAssignable)
	FOnFlasksChanged OnFlasksChanged;

	UFUNCTION()
	int32 GetFlasksAmount() const { return FlasksAmount; }
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Health")
	float PlusHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Flasks")
	int32 FlasksAmount;
	
	// Apply damage function
	UFUNCTION(BlueprintCallable)
	void ApplyDamage(AActor* DamagedActor,float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	// =========================  COMBAT SYSTEM =========================
	
public:

	// Damage changed delegate
	UPROPERTY(BlueprintAssignable)
	FOnDamageChanged OnDamageChanged;
	
	/** Interface function */
	void CallToggleWeaponTrace_Implementation(bool bIsWeaponTraceOn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTraceOn;
	
	/** Weapon class */
	ABaseWeapon* Weapon;

	/** Actor Array */
	TSet<AActor*> HitActors;
	
	UFUNCTION(BlueprintCallable)
	void MeleeTrace();

	void SendMovementVector(FVector2d MovementVector);
	void Evade(ACharacter* OwnerCharacter);

	void SetEvadeDirection(FVector2d MoveVector);
	
	FORCEINLINE bool GetIsEvading() const { return bIsEvading; }
	
protected:

	// Montage delegates
	FOnMontageBlendingOutStarted OnMontageBlendingOutStarted;
	FOnMontageBlendedInEnded OnMontageBlendedInEnded;
	
	void OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnEvadeMontageBlendedInEnded(UAnimMontage* Montage);

	FTimerHandle ResetEvadeDirectionTimerHandle;

	float ResetEvadeDirectionTimerRate;

	void ResetEvadeDirection();
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	bool bIsEvading;

	// enum class evade direction
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	EEvadeDirection EvadeDirection;

	// Evade Montage with sections
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EvadeMontage;

	// Evade section name
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	FName EvadeSectionName;
protected:
	
	/** The effect of hitting the all actors  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* HitActorSystem;

	/** The effect of hitting the enemy  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* HitEnemySystem;

public:
	
	// ================== STAMINA SYSTEM ==================
	
	// Base float stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina;
	
	// Max float stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;

protected:
	// Stamina regeneration rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate;

	// Stamina regeneration delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenDelay;

	// Sprint stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float SprintStaminaCost;

	// Evade stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float EvadeStaminaCost;

	// Timer handle for stamina regeneration
	FTimerHandle StaminaRegenTimer;

	// Add stamina funstion
	UFUNCTION()
	void IncreaseStamina();

	// Start regeneration stamina function
	void StartRegenStamina();

	// Check stamina is depleted
	bool bStaminaDepleted = false;

	// Stop sprinting
	void StopSprinting();

	bool bWantsToSprint = false;
	bool bIsActuallyMoving = false;
public:
	bool bIsSprinting = false;

	// Attack stamina cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float AttackStaminaCost;

	// Set sprinting function
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetSprinting(bool bNewSprinting);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateMovementState(bool bIsMoving);
	
	// Consume stamina function
	UFUNCTION()
	void ConsumeStamina(float Amount);
	
	UFUNCTION(BlueprintCallable)
	bool CanEvade() const { return CurrentStamina >= EvadeStaminaCost; }
	
	UFUNCTION(BlueprintCallable)
	bool CanSprinting() const { return CurrentStamina > 0.0f; }
	
	UFUNCTION(BlueprintCallable)
	bool CanAttacking() const { return CurrentStamina >= AttackStaminaCost; }

	// Get current stamina 
	UFUNCTION(BlueprintCallable)
	float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }

	// ================== SOULS SYSTEM ==================
	
	UFUNCTION(BlueprintCallable)
	int GetSoulsPoints() const { return SoulPoint; }

	// Add souls points when enemy death
	UFUNCTION()
	void AddPoints(int32 Amout);

	// Lose souls when player is dead
	UFUNCTION()
	void LoseSouls();

	// Souls changed delegate
	UPROPERTY()
	FOnSoulsChanged OnSoulsPointsChanged;
	
protected:

	// Souls points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Souls Point")
	int32 SoulPoint = 0;

	// ================== LEVEL SYSTEM ==================
	
public:

	// Player level 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	int32 SoulsToNextLevel = 130;

	// Level up function
	UFUNCTION()
	void LevelUp();

	// Level changed delegate
	UPROPERTY(BlueprintAssignable)
	FOnLevelChanged OnLevelChanged;
	
};
