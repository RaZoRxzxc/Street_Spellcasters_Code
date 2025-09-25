
#include "Characters/LobbyCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ALobbyCharacter::ALobbyCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 300.0f;
	
	MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	MainCamera->SetupAttachment(SpringArm);
	MainCamera->bUsePawnControlRotation = false;
	
}

void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

