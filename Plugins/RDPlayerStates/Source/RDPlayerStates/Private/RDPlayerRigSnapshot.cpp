#include "RDPlayerRigSnapshot.h"
#include "IRDPlayerStateHost.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"

FRDPlayerRigSnapshot FRDPlayerRigSnapshot::Capture(const IRDPlayerStateHost& Host)
{
	FRDPlayerRigSnapshot S;
	if (const USpringArmComponent* Boom = Host.GetCameraBoom())
	{
		S.ArmLength = Boom->TargetArmLength;
		S.SocketOffset = Boom->SocketOffset;
		S.TargetOffset = Boom->TargetOffset;
		S.BoomLocation = Boom->GetRelativeLocation();
		S.BoomRotation = Boom->GetRelativeRotation();
		S.bBoomUsesControlRotation = Boom->bUsePawnControlRotation;
		S.bBoomInheritPitch = Boom->bInheritPitch;
		S.bBoomInheritYaw = Boom->bInheritYaw;
		S.bBoomInheritRoll = Boom->bInheritRoll;
		S.bBoomCollisionTest = Boom->bDoCollisionTest;
	}
	if (const UCameraComponent* Camera = Host.GetCamera())
	{
		S.CameraLocation = Camera->GetRelativeLocation();
		S.CameraRotation = Camera->GetRelativeRotation();
		S.bCameraUsesControlRotation = Camera->bUsePawnControlRotation;
		S.FieldOfView = Camera->FieldOfView;
	}
	if (const APawn* Pawn = Host.GetStatePawn())
	{
		S.bPawnUsesControllerYaw = Pawn->bUseControllerRotationYaw;
	}
	if (const UCharacterMovementComponent* Move = Host.GetMovement())
	{
		S.bOrientToMovement = Move->bOrientRotationToMovement;
	}
	if (const UPrimitiveComponent* Body = Host.GetBodyMesh())
	{
		S.bBodyOwnerNoSee = Body->bOwnerNoSee;
		S.bBodyCastHiddenShadow = Body->bCastHiddenShadow;
	}
	S.bValid = true;
	return S;
}

void FRDPlayerRigSnapshot::Restore(IRDPlayerStateHost& Host) const
{
	if (!bValid)
	{
		return;
	}
	USpringArmComponent* Boom = Host.GetCameraBoom();
	if (Boom)
	{
		Boom->TargetArmLength = ArmLength;
		Boom->SocketOffset = SocketOffset;
		Boom->TargetOffset = TargetOffset;
		Boom->SetRelativeLocationAndRotation(BoomLocation, BoomRotation);
		Boom->bUsePawnControlRotation = bBoomUsesControlRotation;
		Boom->bInheritPitch = bBoomInheritPitch;
		Boom->bInheritYaw = bBoomInheritYaw;
		Boom->bInheritRoll = bBoomInheritRoll;
		Boom->bDoCollisionTest = bBoomCollisionTest;
	}
	if (UCameraComponent* Camera = Host.GetCamera())
	{
		// A state (Free Roam) may have detached the camera; always put it back on the end of the boom.
		if (Boom && Camera->GetAttachParent() != Boom)
		{
			Camera->AttachToComponent(Boom, FAttachmentTransformRules::SnapToTargetNotIncludingScale, USpringArmComponent::SocketName);
		}
		Camera->SetRelativeLocationAndRotation(CameraLocation, CameraRotation);
		Camera->bUsePawnControlRotation = bCameraUsesControlRotation;
		Camera->SetFieldOfView(FieldOfView);
	}
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->bUseControllerRotationYaw = bPawnUsesControllerYaw;
	}
	if (UCharacterMovementComponent* Move = Host.GetMovement())
	{
		Move->bOrientRotationToMovement = bOrientToMovement;
	}
	if (UPrimitiveComponent* Body = Host.GetBodyMesh())
	{
		Body->SetOwnerNoSee(bBodyOwnerNoSee);
		Body->bCastHiddenShadow = bBodyCastHiddenShadow;
		Body->MarkRenderStateDirty();
	}
}
