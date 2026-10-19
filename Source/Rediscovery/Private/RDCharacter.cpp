#include "RDCharacter.h"
#include "RDPlayerStateComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"

ARDCharacter::ARDCharacter()
{
	// Standard humanoid capsule (matches the mannequin).
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// The default Unreal mannequin (copied from the engine's template content into /Game/Characters/Mannequins).
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannyMesh(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> MannyAnim(TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));
	if (MannyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MannyMesh.Object);
	}
	if (MannyAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MannyAnim.Class);
	}
	// Feet on the capsule bottom, facing forward (+X).
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -96.f), FRotator(0.f, -90.f, 0.f));

	// Movement feel similar to the Unreal templates.
	UCharacterMovementComponent* Move = GetCharacterMovement();
	Move->bOrientRotationToMovement = true;
	Move->RotationRate = FRotator(0.f, 500.f, 0.f);
	Move->JumpZVelocity = 600.f;
	Move->AirControl = 0.35f;
	Move->MaxWalkSpeed = 500.f;
	Move->BrakingDecelerationWalking = 2000.f;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Neutral camera rig. Each player state reshapes it on enter and the rig is restored on exit.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PlayerStates = CreateDefaultSubobject<URDPlayerStateComponent>(TEXT("PlayerStates"));
}

UPrimitiveComponent* ARDCharacter::GetBodyMesh() const
{
	return GetMesh();
}

UCharacterMovementComponent* ARDCharacter::GetMovement() const
{
	return GetCharacterMovement();
}

void ARDCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if (PlayerStates)
	{
		PlayerStates->ReapplyCurrentState();
	}
}
