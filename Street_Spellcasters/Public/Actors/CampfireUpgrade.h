// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CampfireUpgrade.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class STREET_SPELLCASTERS_API ACampfireUpgrade : public AActor
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
	
protected:
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* OverlapBoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
	USoundBase* HealSound;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bIsOpenLevelMenu = false;
};
