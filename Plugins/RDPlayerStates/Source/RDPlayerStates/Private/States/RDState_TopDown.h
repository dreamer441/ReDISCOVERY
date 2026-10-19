// Top Down: the camera rides an invisible orbit above the body and looks down at it; the mouse cursor is visible.
// Keys: left mouse click/hold = walk to that point, right mouse drag = move the camera around the orbit
// (left/right all the way round, up/down within a limited range), Space = cast at the cursor. WASD does nothing.
#pragma once

#include "RDPlayerStateDefinition.h"
#include "States/RDClickToMove.h"
#include "RDState_TopDown.generated.h"

UCLASS()
class URDState_TopDown : public URDPlayerStateDefinition
{
	GENERATED_BODY()

public:
	URDState_TopDown();

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

	// Radius of the orbit (distance from the body to the camera).
	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float ArmLength = 1400.f;

	// Starting down-angle and how far the player may tilt it (negative = looking down).
	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float StartPitch = -60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float MinPitch = -80.f;

	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float MaxPitch = -35.f;

	// Degrees of orbit per unit of mouse movement.
	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float DragSensitivity = 2.5f;

private:
	void ApplyOrbit(IRDPlayerStateHost& Host) const;

	FRDClickToMove Walk;
	float OrbitYaw = 0.f;
	float OrbitPitch = -60.f;
	bool bDraggingCamera = false;
};
