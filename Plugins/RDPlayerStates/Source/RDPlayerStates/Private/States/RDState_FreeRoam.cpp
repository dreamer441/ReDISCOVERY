#include "States/RDState_FreeRoam.h"
#include "IRDPlayerStateHost.h"
#include "RDPlayerIntents.h"
#include "RDPlayerStateTags.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

URDState_FreeRoam::URDState_FreeRoam()
{
	StateTag = RDPlayerStateTags::FreeRoam;
	DisplayName = NSLOCTEXT("RDPlayerStates", "FreeRoam", "4 - Free Roam");
}

void URDState_FreeRoam::MapKeys(UInputMappingContext& Context, const URDPlayerIntents& Intents) const
{
	URDPlayerIntents::MapWASD(&Context, Intents.Move);
	URDPlayerIntents::MapKey(&Context, Intents.Vertical, EKeys::E);
	URDPlayerIntents::MapKey(&Context, Intents.Vertical, EKeys::Q, false, true);
	URDPlayerIntents::MapKey(&Context, Intents.CameraDrag, EKeys::RightMouseButton);
	URDPlayerIntents::MapKey(&Context, Intents.Point, EKeys::LeftMouseButton);
	URDPlayerIntents::MapKey(&Context, Intents.Cast, EKeys::SpaceBar);
}

void URDState_FreeRoam::OnEnter(IRDPlayerStateHost& Host)
{
	Walk.Stop();
	bDraggingCamera = false;
	if (UCameraComponent* Camera = Host.GetCamera())
	{
		// Detach where it is now (keep its world position and view); from here on this state turns it.
		CameraRotation = Camera->GetComponentRotation();
		Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Camera->bUsePawnControlRotation = false;
	}
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->bUseControllerRotationYaw = false; // the body must not turn with the camera
	}
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = true;
	}
	SetCursorMode(Host, true);
}

void URDState_FreeRoam::OnExit(IRDPlayerStateHost& Host)
{
	Walk.Stop();
	bDraggingCamera = false;
	SetCursorMode(Host, false);
}

void URDState_FreeRoam::HandleLook(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	if (!bDraggingCamera)
	{
		return; // the camera does not follow the mouse by default
	}
	CameraRotation.Yaw += Input.X * DragSensitivity;
	CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch + Input.Y * DragSensitivity, -89.f, 89.f);
	CameraRotation.Roll = 0.f;
	if (UCameraComponent* Camera = Host.GetCamera())
	{
		Camera->SetWorldRotation(CameraRotation);
	}
}

void URDState_FreeRoam::HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	// Fly where the camera points (including up/down when it looks up/down).
	const FRotationMatrix View(CameraRotation);
	FlyCamera(Host, View.GetUnitAxis(EAxis::X) * Input.Y + View.GetUnitAxis(EAxis::Y) * Input.X);
}

void URDState_FreeRoam::HandleVertical(IRDPlayerStateHost& Host, float Input)
{
	FlyCamera(Host, FVector::UpVector * Input);
}

void URDState_FreeRoam::FlyCamera(IRDPlayerStateHost& Host, const FVector& WorldDirection)
{
	UCameraComponent* Camera = Host.GetCamera();
	const APawn* Pawn = Host.GetStatePawn();
	if (!Camera || !Pawn || !Pawn->GetWorld())
	{
		return;
	}
	// Intents arrive once per frame while a key is held, so scale by frame time for a steady speed.
	const float DeltaSeconds = Pawn->GetWorld()->GetDeltaSeconds();
	Camera->AddWorldOffset(WorldDirection.GetClampedToMaxSize(1.f) * FlySpeed * DeltaSeconds);
}
