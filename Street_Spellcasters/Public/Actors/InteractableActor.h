// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class STREET_SPELLCASTERS_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
	FText ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Collision")
	class UBoxComponent* OverlapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Collision")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	virtual void SetCanShow(bool bCanShow);
	
	UFUNCTION(BlueprintCallable)
	bool CanChange() const { return bCanShowCharSelectMenu; }
	
private:
	bool bCanShowCharSelectMenu = false;

};
