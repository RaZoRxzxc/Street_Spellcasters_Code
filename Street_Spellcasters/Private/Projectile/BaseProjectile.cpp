
#include "Projectile/BaseProjectile.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/BaseEnemyCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->SetupAttachment(GetRootComponent());
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	
	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>("ProjectileEffect");
	ProjectileEffect->SetupAttachment(CollisionComponent);
	
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileComponent->InitialSpeed = 400.f;
	ProjectileComponent->MaxSpeed = 1000.f;
}

void ABaseProjectile::SetHomingEnemy(AActor* Target)
{
	if (ProjectileComponent && Target)
	{
		ProjectileComponent->HomingTargetComponent = Target->GetRootComponent();
	}
}


void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}
	
	AActor* InstigatorActor = GetInstigator();
	AActor* OwnerActor = GetOwner();

	if (OtherActor == InstigatorActor || OtherActor == OwnerActor)
	{
		return;
	}
	
	AController* Controller = (GetInstigator() != nullptr) ? GetInstigator()->GetController() : nullptr;
	
	UGameplayStatics::ApplyDamage(OtherActor, Damage, Controller, this, UDamageType::StaticClass());

	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, GetActorLocation(), GetActorRotation());
	}
		
	Destroy();
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	
	// Ignore projectile owner 
	CollisionComponent->MoveIgnoreActors.Add(GetOwner());
	CollisionComponent->MoveIgnoreActors.Add(GetInstigator());
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseProjectile::DestroyTile, 3.0f, false);
}

void ABaseProjectile::SetHomingTarget(AActor* Target)
{
	HomingTarget = Target;

	if (ProjectileComponent && HomingTarget)
	{
		ProjectileComponent->bIsHomingProjectile = true;
		ProjectileComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		ProjectileComponent->HomingAccelerationMagnitude = HomingAcceleration;
	}
}

void ABaseProjectile::DestroyTile()
{
	Destroy();
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHomingEnabled && HomingTarget && ProjectileComponent)
	{
		float Distance = FVector::Dist(GetActorLocation(), HomingTarget->GetActorLocation());
		if (Distance > MaxHomingDistance)
		{
			ProjectileComponent->bIsHomingProjectile = false;
		}
		else
		{
			FVector NewDirection = (HomingTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ProjectileComponent->Velocity = NewDirection * ProjectileComponent->InitialSpeed;
		}
	}
}

