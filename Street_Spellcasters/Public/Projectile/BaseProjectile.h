
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
UCLASS()
class STREET_SPELLCASTERS_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();

protected:

	UFUNCTION()
	void SetHomingEnemy(AActor* Target);
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			   FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, Category = Projectile)
	class UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, Category = Projectile)
	class UNiagaraComponent* ProjectileEffect;
	
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float HomingAcceleration = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float MaxHomingDistance = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	bool bHomingEnabled = true;

private:
	AActor* HomingTarget;

public:	
	UFUNCTION()
	void SetHomingTarget(AActor* Target);

	UFUNCTION()
	void DestroyTile();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetDamage(float NewDamage) { Damage = NewDamage; }

};
