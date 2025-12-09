// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WizardCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/BaseProjectile.h"
#include "Characters/BaseEnemyCharacter.h"

void AWizardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWizardCharacter::SpawnProjectile()
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

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetInstigator();
	// Check on left hand and spawn projectile on left or right hands when player attacking
	if (bIsLeftHand)
	{
		bIsLeftHand = false;
		Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, RightSpawnLocation, RightShootDirection.Rotation(), Params);
	}
	else
	{
		bIsLeftHand = true;
		Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileActor, LeftSpawnLocation, LeftShootDirection.Rotation(), Params);
	}

	// Set projectile damage
	if (ProjectileActor && CurrentWeapon)
	{
		Projectile->SetDamage(CurrentWeapon->Damage);
	}
}

void AWizardCharacter::SpawnStaffProjectile()
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

	DrawDebugLine(GetWorld(), SpawnLocation, SpawnLocation + ShootDirection * 500, FColor::Red, false, 2);

	if (Projectile)
	{
		Projectile->SetOwner(this);
		Projectile->SetDamage(CurrentWeapon->Damage);

		if (TargetEnemy)
		{
			Projectile->SetHomingTarget(TargetEnemy);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn projectile!"));
	}
}

AActor* AWizardCharacter::FindClosestEnemy() const
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
