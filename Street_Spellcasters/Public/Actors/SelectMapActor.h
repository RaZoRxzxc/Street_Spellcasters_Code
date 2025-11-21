// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "GameFramework/Actor.h"
#include  "Interface/InteractInterface.h"
#include "SelectMapActor.generated.h"

class UBoxComponent;

UCLASS()
class STREET_SPELLCASTERS_API ASelectMapActor : public AInteractableActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	

	ASelectMapActor();

	void InteractWith_Implementation(ACharacter* Character) override;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	
	virtual void SetCanShow(bool bCanShow) override;
	
	UFUNCTION(BlueprintCallable)
	bool CanChangeMap() const { return bCanShowMapSelectMenu; }
	
private:
	bool bCanShowMapSelectMenu = false;

};
