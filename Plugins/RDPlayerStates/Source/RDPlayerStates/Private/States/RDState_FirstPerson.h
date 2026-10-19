// First Person: the camera sits at the head and the body turns with the mouse.
// The owning player does not see their own body (other players and shadows still do). A centre dot is always shown.
// Keys: WASD walk, mouse look, Space jump, Shift sprint, left mouse cast (at the centre dot).
#pragma once

#include "RDPlayerStateDefinition.h"
#include "RDState_FirstPerson.generated.h"

UCLASS()
class URDState_FirstPerson : public URDPlayerStateDefinition
{
	GENERATED_BODY()

public:
	URDState_FirstPerson();

	virtual void HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input) override;
	virtual void HandleJump(IRDPlayerStateHost& Host, bool bPressed) override { SetJumping(Host, bPressed); }
	virtual bool AllowsSprint() const override { return true; }
	virtual bool WantsCrosshair() const override { return true; }
	virtual bool ComputeAimTarget(const IRDPlayerStateHost& Host, FVector& OutTarget) const override { return ComputeScreenCentreAim(Host, OutTarget); }

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) override;
	virtual void MapKeys(UInputMappingContext& Context, const URDPlayerIntents& Intents) const override;

	// Eye position relative to the capsule centre.
	UPROPERTY(EditDefaultsOnly, Category = "First Person")
	FVector EyeOffset = FVector(10.f, 0.f, 70.f);
};
