// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Components/ActorComponent.h"
// #include "Interface/CombatInterface.h"
// #include "CombatComponent.generated.h"
//
// class ABaseWeapon;
//
// // UENUM(BlueprintType)
// // enum class EEvadeDirection : uint8
// // {
// // 	None UMETA(DisplayName = "None"),
// // 	Evade_Forward UMETA(DisplayName = "Evade_Forward"),
// // 	Evade_Backward UMETA(DisplayName = "Evade_Backward"),
// // 	Evade_Right UMETA(DisplayName = "Evade_Right"),
// // 	Evade_Left UMETA(DisplayName = "Evade_Left")
// // };
//
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, StaminaAmount);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulsPointsChanged, int32, StaminaAmount);
// UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
// class STREET_SPELLCASTERS_API UCombatComponent : public UActorComponent, public ICombatInterface
// {
// 	GENERATED_BODY()
//
// public:	
// 	UCombatComponent();
//
// 	/** Interface function */
// 	void CallToggleWeaponTrace_Implementation(bool bIsWeaponTraceOn);
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	bool bIsTraceOn;
//
// 	/** Weapon class */
// 	ABaseWeapon* Weapon;
//
// 	/** Actor Array */
// 	TSet<AActor*> HitActors;
// 	
// 	UFUNCTION(BlueprintCallable)
// 	void MeleeTrace();
//
// 	void SendMovementVector(FVector2d MovementVector);
// 	void Evade(ACharacter* OwnerCharacter);
//
// 	void SetEvadeDirection(FVector2d MoveVector);
// 	
// 	FORCEINLINE bool GetIsEvading() const { return bIsEvading; }
//
// 	UPROPERTY(BlueprintAssignable)
// 	FOnStaminaChanged OnStaminaChanged;
// 	
// protected:
//
// 	FOnMontageBlendingOutStarted OnMontageBlendingOutStarted;
// 	FOnMontageBlendedInEnded OnMontageBlendedInEnded;
// 	
// 	void OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
// 	void OnEvadeMontageBlendedInEnded(UAnimMontage* Montage);
//
// 	FTimerHandle ResetEvadeDirectionTimerHandle;
//
// 	float ResetEvadeDirectionTimerRate;
//
// 	void ResetEvadeDirection();
// private:
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
// 	bool bIsEvading;
//
// 	// enum class evade direction
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
// 	EEvadeDirection EvadeDirection;
//
// 	// Evade Montage with sections
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
// 	UAnimMontage* EvadeMontage;
//
// 	// Evade section name
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
// 	FName EvadeSectionName;
// protected:
// 	
// 	/** The effect of hitting the all actors  */
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	class UNiagaraSystem* HitActorSystem;
//
// 	/** The effect of hitting the enemy  */
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	class UNiagaraSystem* HitEnemySystem;
//
// 	// Base float damage
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
// 	float Damage;
// 	
// 	virtual void BeginPlay() override;
//
// 	// Base float stamina
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
// 	float CurrentStamina;
//
// 	// Max float stamina
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
// 	float MaxStamina;
//
// 	// Stamina regeneration rate
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
// 	float StaminaRegenRate;
//
// 	// Stamina regeneration delay
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
// 	float StaminaRegenDelay;
//
// 	// Sprint stamina cost
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
// 	float SprintStaminaCost;
//
// 	// Evade stamina cost
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
// 	float EvadeStaminaCost;
//
// 	// Timer handle for stamina regeneration
// 	FTimerHandle StaminaRegenTimer;
//
// 	// Add stamina funstion
// 	UFUNCTION()
// 	void IncreaseStamina();
//
// 	// Start regeneration stamina function
// 	void StartRegenStamina();
//
// 	// Check stamina is depleted
// 	bool bStaminaDepleted = false;
//
// 	// Stop sprinting
// 	void StopSprinting();
// 	
// public:
// 	bool bIsSprinting = false;
//
// 	// Attack stamina cost
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
// 	float AttackStaminaCost;
//
// 	// Set sprinting function
// 	UFUNCTION(BlueprintCallable, Category = "Stamina")
// 	void SetSprinting(bool bNewSprinting);
//
// 	// Consume stamina function
// 	UFUNCTION()
// 	void ConsumeStamina(float Amount);
// 	
// 	UFUNCTION(BlueprintCallable)
// 	bool CanEvade() const { return CurrentStamina >= EvadeStaminaCost; }
// 	
// 	UFUNCTION(BlueprintCallable)
// 	bool CanSprinting() const { return CurrentStamina > 0.0f; }
// 	
// 	UFUNCTION(BlueprintCallable)
// 	bool CanAttacking() const { return CurrentStamina >= AttackStaminaCost; }
//
// 	// Get current stamina 
// 	UFUNCTION(BlueprintCallable)
// 	float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }
//
// 	// Tick Function
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
// 	
// 	UFUNCTION(BlueprintCallable)
// 	int GetSoulsPoint() const { return SoulPoint; }
//
// 	UFUNCTION()
// 	void AddPoints(int32 Amout);
//
// 	UFUNCTION()
// 	void LoseSouls();
// 	
// 	UPROPERTY()
// 	FOnSoulsPointsChanged OnSoulsPointsChanged;
// 	
// protected:
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Souls Point")
// 	int32 SoulPoint = 0;
//
// 	
// };
