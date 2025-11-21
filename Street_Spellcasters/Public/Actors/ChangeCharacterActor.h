// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/InteractableActor.h"
#include  "Interface/InteractInterface.h"
#include "ChangeCharacterActor.generated.h"
class UBoxComponent;

UCLASS()
class STREET_SPELLCASTERS_API AChangeCharacterActor : public AInteractableActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	
	AChangeCharacterActor();

	void InteractWith_Implementation(ACharacter* Character) override;
protected:
	virtual void BeginPlay() override;
	
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void SetCanShow(bool bCanShow) override;
	
	UFUNCTION(BlueprintCallable)
	bool CanChangeCharacter() const { return bCanShowCharSelectMenu; }
	
private:
	bool bCanShowCharSelectMenu = false;
};
