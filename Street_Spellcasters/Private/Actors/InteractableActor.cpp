// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractableActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	Mesh->SetupAttachment(RootComponent);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOverlapEnd);
}

void AInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AInteractableActor::SetCanShow(bool bCanShow)
{
	
}


