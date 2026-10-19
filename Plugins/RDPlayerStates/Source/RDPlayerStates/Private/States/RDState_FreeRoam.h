// Free Roam: the camera leaves the body and flies freely; the mouse cursor is visible and the camera does NOT
// follow the mouse unless right mouse is held. For now the body is moved like in Top Down (click to walk);
// this will become something completely different later.
// Keys: WASD fly, E up, Q down, right mouse drag = turn the camera, left mouse click/hold = body walks there,
// Space = cast at the cursor. On exit the camera snaps back onto the body (rig snapshot restore).
#pragma once

#include "RDPlayerStateDefinition.h"
#include "States/RDClickToMove.h"
#include "RDState_FreeRoam.generated.h"

UCLASS()
class URDState_FreeRoam : public URDPlayerStateDefinition
{
	GENERATED_BODY()

public:
	URDState_FreeRoam();

	virtual void HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input) override;
	virtual void HandleVertical(IRDPlayerStateHost& Host, float Input) override;
	virtual void HandleLook(IRDPlayerStateHost& Host, const FVector2D& Input) override;
	virtual void HandleCameraDrag(IRDPlayerStateHost& Host, bool bPressed) override { bDraggingCamera = bPressed; }
	virtual void HandlePointer(IRDPlayerStateHost& Host, const FVector& WorldTarget) override { Walk.SetDestination(WorldTarget); }
	virtual bool ComputeAimTarget(const IRDPlayerStateHost& Host, FVector& OutTarget) const override { return ComputeCursorAim(Host, OutTarget); }

	virtual bool WantsTick() const override { return Walk.bActive; }
	virtual void TickState(IRDPlayerStateHost& Host, float DeltaSeconds) override { Walk.Tick(Host); }

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) override;
	virtual void OnExit(IRDPlayerStateHost& Host) override;
	virtual void MapKeys(UInputMappingContext& Context, const URDPlayerIntents& Intents) const override;

	// Flying speed in cm per second.
	UPROPERTY(EditDefaultsOnly, Category = "Free Roam")
	float FlySpeed = 1200.f;

	// Degrees of turn per unit of mouse movement while right mouse is held.
	UPROPERTY(EditDefaultsOnly, Category = "Free Roam")
	float DragSensitivity = 2.5f;

private:
	void FlyCamera(IRDPlayerStateHost& Host, const FVector& WorldDirection);

	FRDClickToMove Walk;
	FRotator CameraRotation = FRotator::ZeroRotator;
	bool bDraggingCamera = false;
};
