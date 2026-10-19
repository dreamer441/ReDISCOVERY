#include "States/RDState_FirstPerson.h"
#include "IRDPlayerStateHost.h"
#include "RDPlayerStateTags.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"

URDState_FirstPerson::URDState_FirstPerson()
{
	StateTag = RDPlayerStateTags::FirstPerson;
	DisplayName = NSLOCTEXT("RDPlayerStates", "FirstPerson", "1 - First Person");
}

void URDState_FirstPerson::OnEnter(IRDPlayerStateHost& Host)
{
	// Collapse the boom to zero length at eye height: the camera is now "in the head".
	if (USpringArmComponent* Boom = Host.GetCameraBoom())
	{
		Boom->TargetArmLength = 0.f;
		Boom->SocketOffset = FVector::ZeroVector;
		Boom->TargetOffset = FVector::ZeroVector;
		Boom->SetRelativeLocation(EyeOffset);
		Boom->bUsePawnControlRotation = true;
		Boom->bDoCollisionTest = false;
	}
	// The body faces where the player looks.
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->bUseControllerRotationYaw = true;
	}
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = false;
	}
	// Hide the body from its own player only, but keep its shadow.
	if (UPrimitiveComponent* Body = Host.GetBodyMesh())
	{
		Body->SetOwnerNoSee(true);
		Body->bCastHiddenShadow = true;
		Body->MarkRenderStateDirty();
	}
}

void URDState_FirstPerson::HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	if (APawn* Pawn = Host.GetStatePawn())
	{
		MoveRelativeToYaw(Host, Pawn->GetControlRotation().Yaw, Input);
	}
}
