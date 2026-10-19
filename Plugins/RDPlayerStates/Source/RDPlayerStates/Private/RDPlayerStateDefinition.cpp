#include "RDPlayerStateDefinition.h"
#include "IRDPlayerStateHost.h"
#include "RDAuthority.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

namespace
{
	constexpr float AimTraceLength = 20000.f; // 200 m: far enough for any target in view
}

UInputMappingContext* URDPlayerStateDefinition::GetInputContext(const URDPlayerIntents& Intents)
{
	if (!CachedInputContext)
	{
		CachedInputContext = NewObject<UInputMappingContext>(this);
		MapKeys(*CachedInputContext, Intents);
	}
	return CachedInputContext;
}

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

void URDPlayerStateDefinition::SetCursorMode(const IRDPlayerStateHost& Host, bool bCursorVisible)
{
	APlayerController* PC = GetLocalPlayerController(Host);
	if (!PC)
	{
		return; // cursor is a local-only concern
	}
	PC->bShowMouseCursor = bCursorVisible;
	if (bCursorVisible)
	{
		// The mouse is captured only while a button is held, so dragging with a button can still turn the camera.
		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(Mode);
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
	}
}

bool URDPlayerStateDefinition::ComputeScreenCentreAim(const IRDPlayerStateHost& Host, FVector& OutTarget)
{
	APlayerController* PC = GetLocalPlayerController(Host);
	APawn* Pawn = Host.GetStatePawn();
	if (!PC || !Pawn || !Pawn->GetWorld())
	{
		return false;
	}
	FVector ViewLocation;
	FRotator ViewRotation;
	PC->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector End = ViewLocation + ViewRotation.Vector() * AimTraceLength;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(RDAim), false, Pawn); // never aim at our own body
	FHitResult Hit;
	OutTarget = Pawn->GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, End, ECC_Visibility, Params) ? Hit.ImpactPoint : End;
	return true;
}

bool URDPlayerStateDefinition::ComputeCursorAim(const IRDPlayerStateHost& Host, FVector& OutTarget)
{
	APlayerController* PC = GetLocalPlayerController(Host);
	APawn* Pawn = Host.GetStatePawn();
	if (!PC || !Pawn)
	{
		return false;
	}
	FHitResult Hit;
	if (PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		// Aim at chest height above the clicked point so the shot flies over the ground, not into it.
		OutTarget = Hit.ImpactPoint + FVector(0.f, 0.f, 50.f);
		return true;
	}
	// Cursor on the sky: aim along the cursor ray at the body's height.
	FVector RayOrigin, RayDir;
	if (PC->DeprojectMousePositionToWorld(RayOrigin, RayDir))
	{
		OutTarget = RayOrigin + RayDir * AimTraceLength;
		return true;
	}
	return false;
}
