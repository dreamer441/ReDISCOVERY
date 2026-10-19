#include "States/RDState_TopDown.h"
#include "IRDPlayerStateHost.h"
#include "RDPlayerStateTags.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

URDState_TopDown::URDState_TopDown()
{
	StateTag = RDPlayerStateTags::TopDown;
	DisplayName = NSLOCTEXT("RDPlayerStates", "TopDown", "3 - Top Down");
}

void URDState_TopDown::OnEnter(IRDPlayerStateHost& Host)
{
	bHasDestination = false;
	if (USpringArmComponent* Boom = Host.GetCameraBoom())
	{
		Boom->TargetArmLength = ArmLength;
		Boom->SocketOffset = FVector::ZeroVector;
		Boom->bUsePawnControlRotation = false;
		// Ignore the body's rotation so the camera stays fixed in the world while the body turns.
		Boom->bInheritPitch = false;
		Boom->bInheritYaw = false;
		Boom->bInheritRoll = false;
		Boom->SetRelativeRotation(FRotator(CameraPitch, 0.f, 0.f));
		Boom->bDoCollisionTest = false;
	}
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->bUseControllerRotationYaw = false;
	}
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = true;
	}
	// Cursor is a local-only concern: only the controlling player's machine shows it.
	if (APlayerController* PC = GetLocalPlayerController(Host))
	{
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
	}
}

void URDState_TopDown::OnExit(IRDPlayerStateHost& Host)
{
	bHasDestination = false;
	if (APlayerController* PC = GetLocalPlayerController(Host))
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}

float URDState_TopDown::GetCameraYaw(const IRDPlayerStateHost& Host) const
{
	const USpringArmComponent* Boom = Host.GetCameraBoom();
	return Boom ? Boom->GetComponentRotation().Yaw : 0.f;
}

void URDState_TopDown::HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	bHasDestination = false; // keyboard takes over from a clicked destination
	MoveRelativeToYaw(Host, GetCameraYaw(Host), Input);
}

void URDState_TopDown::HandlePointer(IRDPlayerStateHost& Host, const FVector& WorldTarget)
{
	Destination = WorldTarget;
	bHasDestination = true;
}

void URDState_TopDown::TickState(IRDPlayerStateHost& Host, float DeltaSeconds)
{
	APawn* Pawn = Host.GetStatePawn();
	if (!Pawn || !bHasDestination)
	{
		return;
	}
	FVector ToTarget = Destination - Pawn->GetActorLocation();
	ToTarget.Z = 0.f;
	if (ToTarget.Size() <= ArriveDistance)
	{
		bHasDestination = false; // arrived; the component will switch its tick off
		return;
	}
	Pawn->AddMovementInput(ToTarget.GetSafeNormal(), 1.f);
}
