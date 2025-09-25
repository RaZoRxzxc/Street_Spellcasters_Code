
#include "Actors/MenuCameraActor.h"

#include "Camera/CameraComponent.h"

AMenuCameraActor::AMenuCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MenuCamera = CreateDefaultSubobject<UCameraComponent>("MenuCamera");
	MenuCamera->SetupAttachment(RootComponent);
}

void AMenuCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMenuCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

