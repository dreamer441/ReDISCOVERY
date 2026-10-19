#include "States/RDState_ThirdPerson.h"
#include "IRDPlayerStateHost.h"
#include "RDPlayerStateTags.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

URDState_ThirdPerson::URDState_ThirdPerson()
{
	StateTag = RDPlayerStateTags::ThirdPerson;
	DisplayName = NSLOCTEXT("RDPlayerStates", "ThirdPerson", "2 - Third Person");
}

void URDState_ThirdPerson::OnEnter(IRDPlayerStateHost& Host)
{
	if (USpringArmComponent* Boom = Host.GetCameraBoom())
	{
		Boom->TargetArmLength = ArmLength;
		Boom->SocketOffset = ShoulderOffset;
		Boom->bUsePawnControlRotation = true; // mouse orbits the camera
		Boom->bDoCollisionTest = true;        // pull in when a wall is behind us
	}
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->bUseControllerRotationYaw = false;
	}
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = true; // body faces where it walks
	}
}

void URDState_ThirdPerson::HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	if (APawn* Pawn = Host.GetStatePawn())
	{
		MoveRelativeToYaw(Host, Pawn->GetControlRotation().Yaw, Input);
	}
}
