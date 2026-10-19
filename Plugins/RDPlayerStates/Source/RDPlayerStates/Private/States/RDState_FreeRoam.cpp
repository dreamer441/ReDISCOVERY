#include "States/RDState_FreeRoam.h"
#include "IRDPlayerStateHost.h"
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

void URDState_FreeRoam::OnEnter(IRDPlayerStateHost& Host)
{
	if (UCameraComponent* Camera = Host.GetCamera())
	{
		// Detach where it is now (keep its world position) and let it follow the mouse look directly.
		Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Camera->bUsePawnControlRotation = true;
	}
	// The body must not turn with the free camera.
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->bUseControllerRotationYaw = false;
	}
	// The body is left alone: no movement input reaches it in this state, so it stands still.
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = true;
	}
}

void URDState_FreeRoam::HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	const APawn* Pawn = Host.GetStatePawn();
	if (!Pawn)
	{
		return;
	}
	// Fly where the view points (including up/down when looking up/down).
	const FRotationMatrix View(Pawn->GetControlRotation());
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
