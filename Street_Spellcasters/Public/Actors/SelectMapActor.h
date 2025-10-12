// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include  "Interface/InteractInterface.h"
#include "SelectMapActor.generated.h"

class UBoxComponent;

UCLASS()
class STREET_SPELLCASTERS_API ASelectMapActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	

	ASelectMapActor();

	void InteractWith_Implementation(ACharacter* Character) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Name)
	FText ActorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Collision")
	UBoxComponent* OverlapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Collision")
	UStaticMeshComponent* Mesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void SetCanShowMapSelectMenu(bool bCanShow);
	
	UFUNCTION(BlueprintCallable)
	bool CanChangeMap() const { return bCanShowMapSelectMenu; }
	
private:
	bool bCanShowMapSelectMenu = false;

};
