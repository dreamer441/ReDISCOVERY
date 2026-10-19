// Top Down: a high camera looks down at about 60 degrees; the mouse cursor is visible.
// Controls: click (or hold) the left mouse button on the ground to walk there; WASD also walks
// relative to the screen. Mouse movement does not turn the camera.
#pragma once

#include "RDPlayerStateDefinition.h"
#include "RDState_TopDown.generated.h"

UCLASS()
class URDState_TopDown : public URDPlayerStateDefinition
{
	GENERATED_BODY()

public:
	URDState_TopDown();

	virtual void HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input) override;
	virtual void HandleLook(IRDPlayerStateHost& Host, const FVector2D& Input) override {} // camera is fixed
	virtual void HandlePointer(IRDPlayerStateHost& Host, const FVector& WorldTarget) override;

	virtual bool WantsTick() const override { return bHasDestination; }
	virtual void TickState(IRDPlayerStateHost& Host, float DeltaSeconds) override;

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) override;
	virtual void OnExit(IRDPlayerStateHost& Host) override;

	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float ArmLength = 1400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float CameraPitch = -60.f;

	// How close counts as "arrived" at a clicked point (cm).
	UPROPERTY(EditDefaultsOnly, Category = "Top Down")
	float ArriveDistance = 60.f;

private:
	float GetCameraYaw(const IRDPlayerStateHost& Host) const;

	FVector Destination = FVector::ZeroVector;
	bool bHasDestination = false;
};
