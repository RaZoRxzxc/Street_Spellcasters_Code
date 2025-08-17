// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
// #include "Components/ActorComponent.h"
// #include "HealthComponent.generated.h"
//
//
// // UENUM(BlueprintType)
// // enum class EHitDirection : uint8
// // {
// // 	None UMETA(DisplayName = "None"),
// // 	Hit_Forward UMETA(DisplayName = "Forward"),
// // 	Hit_Backward UMETA(DisplayName = "Backward"),
// // 	Hit_Left UMETA(DisplayName = "Left"),
// // 	Hit_Right UMETA(DisplayName = "Right")
// // 	
// // };
//
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
// UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
// class STREET_SPELLCASTERS_API UHealthComponent : public UActorComponent
// {
// 	GENERATED_BODY()
//
// public:	
// 	UHealthComponent();
// 	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
// 	EHitDirection HitDirection;
// 	
// 	UFUNCTION(BlueprintCallable)
// 	bool IsDead() const {return Health <= 0.0f;}
// 	
// 	UFUNCTION(BlueprintCallable)
// 	float GetHealthPercent() const {return Health / MaxHealth;}
// 	
// 	UPROPERTY(BlueprintAssignable)
// 	FOnDeath OnDeath;
//
// 	UPROPERTY(BlueprintAssignable)
// 	FOnHealthChanged OnHealthChanged;
// 	
// 	UPROPERTY(BlueprintReadWrite)
// 	float DamageBlockMultiplier;
//
// 	UPROPERTY(BlueprintReadWrite)
// 	bool bIsBlocking = false;
//
// 	UFUNCTION()
// 	void StartBlock();
//
// 	UFUNCTION()
// 	void EndBlock();
// protected:
//
// 	UFUNCTION()
// 	void CalculateHitDirection(FVector HitOrigin);
//
// 	void PlayHitAnimation(bool bIsBlockedHit);
//
// private:
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
// 	UAnimMontage* HitMontage;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
// 	UAnimMontage* BlockHitMontage;
// 	
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit_Reaction", meta = (AllowPrivateAccess = "true"))
// 	FName HitDirectionName;
//
//
// 	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
//
// public:	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hit_Reaction")
// 	bool bIsTakeHit = false;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
// 	float Health;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
// 	float MaxHealth;
// protected:
//
// 	UFUNCTION(BlueprintCallable)
// 	void ApplyDamage(AActor* DamagedActor,float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
// 	
// 	virtual void BeginPlay() override;
//
// public:	
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
// 		
// };
