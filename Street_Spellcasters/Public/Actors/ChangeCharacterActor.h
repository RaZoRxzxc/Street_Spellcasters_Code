// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "Interface/InteractInterface.h"
#include "ChangeCharacterActor.generated.h"
class UBoxComponent;

UCLASS()
class STREET_SPELLCASTERS_API AChangeCharacterActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	
	AChangeCharacterActor();

	void InteractWith_Implementation(ACharacter* Character) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
	FText ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Collision")
	UBoxComponent* OverlapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Collision")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void SetCanShowCharSelectMenu(bool bCanShow);
	
	UFUNCTION(BlueprintCallable)
	bool CanChangeCharacter() const { return bCanShowCharSelectMenu; }
	
private:
	bool bCanShowCharSelectMenu = false;
};
