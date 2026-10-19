#include "States/RDState_ThirdPerson.h"
#include "IRDPlayerStateHost.h"
#include "RDPlayerIntents.h"
#include "RDPlayerStateTags.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

URDState_ThirdPerson::URDState_ThirdPerson()
{
	StateTag = RDPlayerStateTags::ThirdPerson;
	DisplayName = NSLOCTEXT("RDPlayerStates", "ThirdPerson", "2 - Third Person");
}

void URDState_ThirdPerson::MapKeys(UInputMappingContext& Context, const URDPlayerIntents& Intents) const
{
	URDPlayerIntents::MapWASD(&Context, Intents.Move);
	URDPlayerIntents::MapKey(&Context, Intents.Jump, EKeys::SpaceBar);
	URDPlayerIntents::MapKey(&Context, Intents.Sprint, EKeys::LeftShift);
	URDPlayerIntents::MapKey(&Context, Intents.Aim, EKeys::RightMouseButton);
	URDPlayerIntents::MapKey(&Context, Intents.Cast, EKeys::LeftMouseButton);
}

void URDState_ThirdPerson::OnEnter(IRDPlayerStateHost& Host)
{
	bAiming = false;
	bBlending = false;
	if (USpringArmComponent* Boom = Host.GetCameraBoom())
	{
		Boom->TargetArmLength = ArmLength;
		Boom->SocketOffset = ShoulderOffset;
		Boom->bUsePawnControlRotation = true; // mouse orbits the camera
		Boom->bDoCollisionTest = true;        // pull in when a wall is behind us
	}
	if (const UCameraComponent* Camera = Host.GetCamera())
	{
		NormalFieldOfView = Camera->FieldOfView;
	}
	SetBodyFacesCamera(Host, false);
}

void URDState_ThirdPerson::OnExit(IRDPlayerStateHost& Host)
{
	bAiming = false;
	bBlending = false;
}

void URDState_ThirdPerson::HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	if (APawn* Pawn = Host.GetStatePawn())
	{
		MoveRelativeToYaw(Host, Pawn->GetControlRotation().Yaw, Input);
	}
}

void URDState_ThirdPerson::HandleAim(IRDPlayerStateHost& Host, bool bPressed)
{
	bAiming = bPressed;
	bBlending = true;
	// While aiming the body faces where the camera looks, so shots leave from the front.
	SetBodyFacesCamera(Host, bAiming);
}

void URDState_ThirdPerson::SetBodyFacesCamera(IRDPlayerStateHost& Host, bool bFacesCamera) const
{
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->bUseControllerRotationYaw = bFacesCamera;
	}
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = !bFacesCamera;
	}
}

void URDState_ThirdPerson::TickState(IRDPlayerStateHost& Host, float DeltaSeconds)
{
	USpringArmComponent* Boom = Host.GetCameraBoom();
	UCameraComponent* Camera = Host.GetCamera();
	if (!Boom || !Camera)
	{
		bBlending = false;
		return;
	}
	const float TargetLength = bAiming ? AimArmLength : ArmLength;
	const FVector TargetOffset = bAiming ? AimShoulderOffset : ShoulderOffset;
	const float TargetFOV = bAiming ? AimFieldOfView : NormalFieldOfView;

	Boom->TargetArmLength = FMath::FInterpTo(Boom->TargetArmLength, TargetLength, DeltaSeconds, AimBlendSpeed);
	Boom->SocketOffset = FMath::VInterpTo(Boom->SocketOffset, TargetOffset, DeltaSeconds, AimBlendSpeed);
	Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaSeconds, AimBlendSpeed));

	// Close enough: snap to the exact values and stop ticking.
	if (FMath::IsNearlyEqual(Boom->TargetArmLength, TargetLength, 1.f) && FMath::IsNearlyEqual(Camera->FieldOfView, TargetFOV, 0.2f))
	{
		Boom->TargetArmLength = TargetLength;
		Boom->SocketOffset = TargetOffset;
		Camera->SetFieldOfView(TargetFOV);
		bBlending = false;
	}
}
