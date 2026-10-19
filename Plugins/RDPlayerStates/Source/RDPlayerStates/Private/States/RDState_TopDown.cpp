#include "States/RDState_TopDown.h"
#include "IRDPlayerStateHost.h"
#include "RDPlayerIntents.h"
#include "RDPlayerStateTags.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

URDState_TopDown::URDState_TopDown()
{
	StateTag = RDPlayerStateTags::TopDown;
	DisplayName = NSLOCTEXT("RDPlayerStates", "TopDown", "3 - Top Down");
}

void URDState_TopDown::MapKeys(UInputMappingContext& Context, const URDPlayerIntents& Intents) const
{
	URDPlayerIntents::MapKey(&Context, Intents.Point, EKeys::LeftMouseButton);
	URDPlayerIntents::MapKey(&Context, Intents.CameraDrag, EKeys::RightMouseButton);
	URDPlayerIntents::MapKey(&Context, Intents.Cast, EKeys::SpaceBar);
}

void URDState_TopDown::OnEnter(IRDPlayerStateHost& Host)
{
	Walk.Stop();
	bDraggingCamera = false;
	OrbitPitch = StartPitch;
	// Start the orbit behind the body, looking the way it faces.
	const APawn* Pawn = Host.GetStatePawn();
	OrbitYaw = Pawn ? Pawn->GetActorRotation().Yaw : 0.f;

	if (USpringArmComponent* Boom = Host.GetCameraBoom())
	{
		Boom->TargetArmLength = ArmLength;
		Boom->SocketOffset = FVector::ZeroVector;
		Boom->bUsePawnControlRotation = false;
		// Ignore the body's rotation: the orbit angle alone decides where the camera is.
		Boom->bInheritPitch = false;
		Boom->bInheritYaw = false;
		Boom->bInheritRoll = false;
		Boom->bDoCollisionTest = false;
	}
	ApplyOrbit(Host);

	if (APawn* MutablePawn = Host.GetStatePawn())
	{
		MutablePawn->bUseControllerRotationYaw = false;
	}
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = true; // body faces where it walks
	}
	SetCursorMode(Host, true);
}

void URDState_TopDown::OnExit(IRDPlayerStateHost& Host)
{
	Walk.Stop();
	bDraggingCamera = false;
	SetCursorMode(Host, false);
}

void URDState_TopDown::HandleLook(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	// The mouse only moves the camera while right mouse is held.
	if (!bDraggingCamera)
	{
		return;
	}
	OrbitYaw += Input.X * DragSensitivity;
	OrbitPitch = FMath::Clamp(OrbitPitch + Input.Y * DragSensitivity, MinPitch, MaxPitch);
	ApplyOrbit(Host);
}

void URDState_TopDown::ApplyOrbit(IRDPlayerStateHost& Host) const
{
	if (USpringArmComponent* Boom = Host.GetCameraBoom())
	{
		// Inherit flags are off, so this relative rotation is the orbit's world rotation.
		Boom->SetRelativeRotation(FRotator(OrbitPitch, OrbitYaw, 0.f));
	}
}
