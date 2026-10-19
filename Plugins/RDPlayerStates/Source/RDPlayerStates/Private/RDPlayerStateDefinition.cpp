#include "RDPlayerStateDefinition.h"
#include "IRDPlayerStateHost.h"
#include "RDAuthority.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

void URDPlayerStateDefinition::Enter(IRDPlayerStateHost& Host)
{
	// Remember the rig exactly as it was, so Exit can put it back no matter what OnEnter changed.
	SavedRig = FRDPlayerRigSnapshot::Capture(Host);
	OnEnter(Host);
}

void URDPlayerStateDefinition::Exit(IRDPlayerStateHost& Host)
{
	OnExit(Host);
	SavedRig.Restore(Host);
	SavedRig = FRDPlayerRigSnapshot();
}

void URDPlayerStateDefinition::HandleLook(IRDPlayerStateHost& Host, const FVector2D& Input)
{
	if (APawn* Pawn = Host.GetStatePawn())
	{
		Pawn->AddControllerYawInput(Input.X);
		Pawn->AddControllerPitchInput(-Input.Y); // mouse up = look up
	}
}

void URDPlayerStateDefinition::MoveRelativeToYaw(IRDPlayerStateHost& Host, float Yaw, const FVector2D& Input)
{
	if (APawn* Pawn = Host.GetStatePawn())
	{
		const FRotator YawOnly(0.f, Yaw, 0.f);
		Pawn->AddMovementInput(FRotationMatrix(YawOnly).GetUnitAxis(EAxis::X), Input.Y);
		Pawn->AddMovementInput(FRotationMatrix(YawOnly).GetUnitAxis(EAxis::Y), Input.X);
	}
}

void URDPlayerStateDefinition::SetJumping(IRDPlayerStateHost& Host, bool bPressed)
{
	if (ACharacter* Character = Cast<ACharacter>(Host.GetStatePawn()))
	{
		bPressed ? Character->Jump() : Character->StopJumping();
	}
}

APlayerController* URDPlayerStateDefinition::GetLocalPlayerController(const IRDPlayerStateHost& Host)
{
	APawn* Pawn = Host.GetStatePawn();
	return RDAuthority::IsLocallyControlled(Pawn) ? Cast<APlayerController>(Pawn->GetController()) : nullptr;
}
