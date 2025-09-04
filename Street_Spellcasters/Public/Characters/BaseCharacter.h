
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "Interface/CombatInterface.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	E_Magic UMETA(DisplayName = "Magic"),
	E_Melee UMETA(DisplayName = "Melee"),
	E_Bow UMETA(DisplayName = "Bow"),
};

UCLASS()
class STREET_SPELLCASTERS_API ABaseCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	// Combat interface function 
	void IsPlayerAttack_Implementation(bool& bIsAttack) override;
	void isPlayerDead_Implementation(bool& IsDead) override;

	// Spring arm componentw
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	// Camera componentw
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Sprint Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	
	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	// Attack Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	// Block Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	// Evade Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* EvadeAction;

	// Interact Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	// Heal Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* HealAction;

	// Map Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MapAction;

	// Zoom in Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ZoomInAction;

	// Zoom out Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ZoomOutAction;
	
	// Mapping context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Sprint, meta=(AllowPrivateAccess = "true"))
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComponent;
protected:
	
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
public:
	void StopSprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed;
	
private:
	/** Setup input function */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Jumping function */
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
	
	// Spawn projectile in hands
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();

	// Spawn projectile for magic staff weapon
	UFUNCTION(BlueprintCallable)
	void SpawnStaffProjectile();

	// Find closest enemy function
	UFUNCTION(BlueprintCallable)
	AActor* FindClosestEnemy() const;

	// Angle for detected enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float TargetingConeAngle = 60.0f;

	// Distance for detected enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float TargetingDistance = 2000.0f;

	// Distance for spawn projectile forward 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ForwardAttackDistance = 1500.0f;

	// Search enemy radius 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float EnemySearchRadius = 2000.0f;

	// Socket name for spawn projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FName StaffProjectileSocket = "Projectile_Socket";
	
	// Check for a left hand 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLeftHand = false;

	// Projectile class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class ABaseProjectile> ProjectileActor;
	
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
	void TryOpenLevelUpPanel();
	
public:
	UFUNCTION(BlueprintCallable)
	void SetCanShowLevelUpPanel(bool bCanShow);
	
	UFUNCTION(BlueprintCallable)
	bool CanLevelUp() const { return bCanShowLevelPanel; }
	
private:
	bool bCanShowLevelPanel = false;
	
	// Perception stimulus source 
	class UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
public:

	// Tick function
	virtual void Tick(float DeltaTime) override;

protected:

	// Flasks heal animation montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Animation")
	UAnimMontage* HealFlaskMontage;

	// Flask heal function 
	UFUNCTION()
	void Heal();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	class UMiniMapWidget* MapWidget;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<UUserWidget> MapWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Map")
	FVector2D MouseMapPosition;
	
	bool bIsMapOpen = false;

	UFUNCTION(BlueprintCallable)
	void ToggleMap();

	UFUNCTION()
	void ZoomInMap();

	UFUNCTION()
	void ZoomOutMap();

	UFUNCTION(BlueprintCallable)
	bool IsMapOpen() const { return bIsMapOpen; }
	

};


