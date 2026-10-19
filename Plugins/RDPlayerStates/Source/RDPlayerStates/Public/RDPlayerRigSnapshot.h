// A copy of every camera/body/movement setting a state is allowed to change.
// Taken just before a state enters and restored when it exits, so "Exit fully undoes Enter" holds
// even if a state forgets to clean something up.
#pragma once

#include "CoreMinimal.h"

class IRDPlayerStateHost;

struct RDPLAYERSTATES_API FRDPlayerRigSnapshot
{
	// Camera boom (spring arm)
	float ArmLength = 0.f;
	FVector SocketOffset = FVector::ZeroVector;
	FVector TargetOffset = FVector::ZeroVector;
	FVector BoomLocation = FVector::ZeroVector;
	FRotator BoomRotation = FRotator::ZeroRotator;
	bool bBoomUsesControlRotation = false;
	bool bBoomInheritPitch = true;
	bool bBoomInheritYaw = true;
	bool bBoomInheritRoll = true;
	bool bBoomCollisionTest = true;

	// Camera
	FVector CameraLocation = FVector::ZeroVector;
	FRotator CameraRotation = FRotator::ZeroRotator;
	bool bCameraUsesControlRotation = false;
	float FieldOfView = 90.f;

	// Pawn and movement
	bool bPawnUsesControllerYaw = false;
	bool bOrientToMovement = true;

	// Body visibility
	bool bBodyOwnerNoSee = false;
	bool bBodyCastHiddenShadow = false;

	bool bValid = false;

	static FRDPlayerRigSnapshot Capture(const IRDPlayerStateHost& Host);
	void Restore(IRDPlayerStateHost& Host) const;
};
