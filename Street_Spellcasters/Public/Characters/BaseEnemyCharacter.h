// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "Interface/CombatInterface.h"
#include "Navigation/PathFollowingComponent.h"
#include "BaseEnemyCharacter.generated.h"

class UHealthComponent;
class UCombatComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    E_None UMETA(DisplayName = "None"),
    E_Knight UMETA(DisplayName = "Knight"),
    E_Wizard UMETA(DisplayName = "Wizard")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTeleportEnd);

UCLASS()
class STREET_SPELLCASTERS_API ABaseEnemyCharacter : public ACharacter, public ICombatInterface
{
    GENERATED_BODY()

public:
    ABaseEnemyCharacter();

    // Combat Interface implementation
    void MeleeAttack_Implementation() override;
    void StartBlock_Implementation() override;
    void EndBlock_Implementation() override;

    UPROPERTY(BlueprintAssignable)
    FOnAttackEnd OnAttackEnd;

    UPROPERTY(BlueprintAssignable)
    FOnTeleportEnd OnTeleportEnd;

    UFUNCTION(BlueprintCallable, Category = "Attack")
    void Attack();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
    class UBehaviorTree* EnemyBehaviorTree;

    UFUNCTION(BlueprintCallable)
    void AttackEnd();

    UFUNCTION(BlueprintCallable, Category = "Attack")
    bool CanAttack() const { return !bIsAttacking && !bIsDead && !bIsTeleporting; }

protected:
    /** Attack animation array */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TArray<UAnimMontage*> AttackAnimations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Souls", meta = (ClampMin = "0"))
    int32 MinSoulsReward = 50;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Souls", meta = (ClampMin = "0"))
    int32 MaxSoulsReward = 150;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Souls", meta = (ClampMin = "0.1"))
    float SoulsMultiplier = 1.0f;

    UFUNCTION(BlueprintCallable, Category = "Souls")
    int32 GenerateRandomSouls() const;

    UFUNCTION()
    void SpawnNiagara();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Souls")
    class UNiagaraSystem* SoulsNiagara;
    
    /** Block animation montage */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimMontage* BlockMontage;

    /** Effects */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    class UNiagaraComponent* BlockEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UStaticMeshComponent* BlockEffectMesh;

    /** Projectile related */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    UArrowComponent* ProjectileArrow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLeftHand = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<class ABaseProjectile> ProjectileActor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UStatsComponent* StatsComponent;
    
    /** Weapon */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ABaseWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    ABaseWeapon* CurrentWeapon;

    /** Enemy type */
    UPROPERTY(EditAnywhere)
    EEnemyType EnemyType;

    virtual void BeginPlay() override;

    /** Animation callbacks */
    UFUNCTION()
    void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);

    /** Attack helpers */
    UFUNCTION(BlueprintCallable)
    void PlayRandomAttackAnimation();


    // Spawn weapon function
    UFUNCTION()
    void SpawnWeapon();

    UFUNCTION(BlueprintCallable)
    void SpawnProjectile();

    /** Health widget */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UWidgetComponent* HealthWidgetComp;

    // Show health widget function
    UFUNCTION()
    void ShowHealthWidget();

    // Hide health widget function
    void ShowHealthWidgetEnd();

    /** Damage handling */
    UFUNCTION()
    void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    // is dead function for delegate
    UFUNCTION()
    void isDead();

    /** Blocking */
    void StartBlocking();
    void EndBlocking();
    
    UFUNCTION()
    void TeleportEnd();

    UFUNCTION()
    void OnMoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result);

public:
    /** Teleport */
    UFUNCTION(BlueprintCallable)
    void Teleport(const FVector& location);
    
    virtual void Tick(float DeltaTime) override;

    // State flags
    bool bIsBlocking = false;
    bool bIsDead = false;

private:
    // Attack state
    bool bIsAttacking = false;
    int32 LastAttackIndex = -1;
    FTimerHandle AttackHandle;
    FTimerHandle HealthWidgetTimer;

    // Teleport state
    bool bIsTeleporting = false;
    FTimerHandle TeleportEndTimer;

    // Effects
    UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* TeleportBodyEffectComp;

    UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* TeleportTrailEffectComp;
    
    UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystem* TeleportBodyEffect;

    UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
    UParticleSystem* TeleportTrailEffect;

public:

    // Evade function
    UFUNCTION()
    void Evade();

protected:
    // Health widget
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    class UEnemyWidget* HealthWidget;
};