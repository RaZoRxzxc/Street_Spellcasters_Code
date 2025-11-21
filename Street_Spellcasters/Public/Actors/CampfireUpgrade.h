// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Interface/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "CampfireUpgrade.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class STREET_SPELLCASTERS_API ACampfireUpgrade : public AInteractableActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ACampfireUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector2D GetMapPosition() const { return FVector2D(GetActorLocation().X, GetActorLocation().Y); }
    
	// Иконка для карты
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	UTexture2D* MapIcon;
    
	// Размер иконки на карте
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector2D MapIconSize = FVector2D(32.0f, 32.0f);

	void InteractWith_Implementation(ACharacter* Character) override;
	
protected:

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
	// FText ActorName;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	// UStaticMeshComponent* Mesh;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	// UBoxComponent* OverlapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundBase* HealSound;
	

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	bool bIsOpenLevelMenu = false;
	
	virtual void SetCanShow(bool bCanShow) override;
	
	UFUNCTION(BlueprintCallable)
	bool CanLevelUp() const { return bCanShowLevelPanel; }
	
private:
	bool bCanShowLevelPanel = false;
};
