// Free Roam: the camera leaves the body and flies freely; the body stays where it is.
// Controls: WASD fly relative to the view, E / Space up, Q / Ctrl down, mouse looks.
// On exit the camera snaps back onto the body (done by the rig snapshot restore).
#pragma once

#include "RDPlayerStateDefinition.h"
#include "RDState_FreeRoam.generated.h"

UCLASS()
class URDState_FreeRoam : public URDPlayerStateDefinition
{
	GENERATED_BODY()

public:
	URDState_FreeRoam();

	virtual void HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input) override;
	virtual void HandleVertical(IRDPlayerStateHost& Host, float Input) override;

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) override;

	// Flying speed in cm per second.
	UPROPERTY(EditDefaultsOnly, Category = "Free Roam")
	float FlySpeed = 1200.f;

private:
	void FlyCamera(IRDPlayerStateHost& Host, const FVector& WorldDirection);
};
