// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "WizardCharacter.generated.h"

/**
 * 
 */
UCLASS()
class STREET_SPELLCASTERS_API AWizardCharacter : public ABaseCharacter
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
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
};
