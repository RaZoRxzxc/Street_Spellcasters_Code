
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Components/StatsComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "Interface/CombatInterface.h"
#include "Interface/InputHandlerInterface.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	E_Magic UMETA(DisplayName = "Magic"),
	E_Melee UMETA(DisplayName = "Melee"),
	E_Bow UMETA(DisplayName = "Bow"),
};

UCLASS()
class STREET_SPELLCASTERS_API ABaseCharacter : public ACharacter, public ICombatInterface, public IInputHandlerInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void OnConstruction(const FTransform& Transform) override;

	// Combat interface function 
	void IsPlayerAttack_Implementation(bool& bIsAttack) override;
	void isPlayerDead_Implementation(bool& IsDead) override;

	// InputHandlerInterface realization
	void HandleMove_Implementation(const FInputActionValue& Value) override;
	void HandleLook_Implementation(const FInputActionValue& Value) override;
	void HandleJump_Implementation() override;
	void HandleStopJumping_Implementation() override;
	void HandleSprintStart_Implementation() override;
	void HandleSprintStop_Implementation() override;
	void HandleAttack_Implementation() override;
	void HandleBlockStart_Implementation() override;
	void HandleBlockEnd_Implementation() override;
	void HandleEvade_Implementation(const FInputActionValue& Value) override;
	void HandleInteract_Implementation() override;
	void HandleHeal_Implementation() override;
	void HandleToggleMap_Implementation() override;

	UFUNCTION()
	UStatsComponent* GetStatsComp() const { return StatsComponent; }
	
protected:

	// Character face elements
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HairMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FaceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrecklesMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BrowsMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* EyesMesh;

	// Spring arm component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	// Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sprint, meta=(AllowPrivateAccess = "true"))
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStatsComponent* StatsComponent;
	
	/** Evade funtion */
	void Evade(const FInputActionValue& Value);

	// Evade/Dodge system
	bool bEvadeButtomPressed = false;
	
	/** Block functions */
	void StartBlocking();
	void EndBlocking();

	// Block effect for wizard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UNiagaraComponent* BlockEffect;

	// Block magic static mesh for wizard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UStaticMeshComponent* BlockEffectMesh;

	// Block animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Animation)
	UAnimMontage* BlockMontage;
	
	// Character type
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = CharacterTypes)
	ECharacterType CharacterType;

	// Begin play function
	virtual void BeginPlay() override;
	
private:
	
	/** Movement functions */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	/** Sprint functions */
	void StartSprint();

protected:
	// Base sensitivity values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseMouseSensitivity = 1.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseControllerSensitivity = 1.0f;
public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	void UpdateInputSensitivity();
	
	void StopSprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed;
	
private:

	// Jump function
	void Jump() override;
	void StopJumping() override;
	
	bool bIsJumping = false;
	
protected:
	// Attack system
	UFUNCTION()
	void Attack();

	// Play attack montages function 
	UFUNCTION()
	void PlayAttackAnim();
	
	UFUNCTION()
	void EnableAttack();

	// Attack end delegate for animation instance
	UFUNCTION()
	void AttackEndDelegate(UAnimMontage* Montage, bool bInterrupted);

	// index for attack animation array
	UPROPERTY()
	int32 LastAttackIndex;

	// Attack delay timer handle
	FTimerHandle AttackDelayHandle;

public:
	
	// Checking whether the player can be attacked
	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool CanAttack() const { return !bIsAttacking && !bIsDead; }
	
	// Attack end function
	UFUNCTION(BlueprintCallable)
	void OnAttackEnd();

	// Attack animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TArray<UAnimMontage*> AttackAnimations;

	// Check can attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool bIsAttacking = false;
	/** Dead system */
	UFUNCTION(BlueprintCallable)
	void isDead();

	// Dead animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* DeathMontage;

	// Check player is dead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	bool bIsDead = false;
	
	/** Weapon class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<ABaseWeapon> WeaponClass;

	// Weapon for spawn
	UPROPERTY(EditAnywhere)
	ABaseWeapon* CurrentWeapon;

	// Spawn weapon function
	UFUNCTION()
	void SpawnWeapon();

protected:

	UFUNCTION(BlueprintCallable)
	void Interact();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractable;
	
	// Perception stimulus source 
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;

	UFUNCTION()
	bool CanHeal() const { return bIsHeal; }
protected:
	// Tick function
	virtual void Tick(float DeltaTime) override;


	// Flasks heal animation montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Heal")
	UAnimMontage* HealFlaskMontage;

	// Flask heal function 
	UFUNCTION()
	void Heal();
	
	UFUNCTION(BlueprintCallable)
    void HealEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Heal")
	UStaticMeshComponent* PotionMesh;

	TSubclassOf<UStaticMeshComponent> PotionClass;

	bool bIsHeal = false;

	UFUNCTION(BlueprintCallable)
	void SpawnPotionMesh();

	UFUNCTION(BlueprintCallable)
	void DestroyPotionMesh();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	class UMiniMapWidget* MapWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<UUserWidget> MapWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
	FVector2D MouseMapPosition;
	
	bool bIsMapOpen = false;

	UFUNCTION(BlueprintCallable)
	void ToggleMap();

	UFUNCTION(BlueprintCallable)
	bool IsMapOpen() const { return bIsMapOpen; }
	
};


