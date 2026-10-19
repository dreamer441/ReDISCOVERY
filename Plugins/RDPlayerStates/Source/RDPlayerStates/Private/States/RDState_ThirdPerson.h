// Third Person: the camera orbits behind the shoulder on a spring arm; the body turns toward where it walks.
// Holding right mouse zooms in over the shoulder, turns the body with the camera and shows a centre dot.
// Keys: WASD walk, mouse orbits, Space jump, Shift sprint (not while aiming), right mouse aim, left mouse cast.
#pragma once

#include "RDPlayerStateDefinition.h"
#include "RDState_ThirdPerson.generated.h"

UCLASS()
class URDState_ThirdPerson : public URDPlayerStateDefinition
{
	GENERATED_BODY()

public:
	URDState_ThirdPerson();

	virtual void HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input) override;
	virtual void HandleJump(IRDPlayerStateHost& Host, bool bPressed) override { SetJumping(Host, bPressed); }
	virtual void HandleAim(IRDPlayerStateHost& Host, bool bPressed) override;
	virtual bool AllowsSprint() const override { return !bAiming; }
	virtual bool WantsCrosshair() const override { return bAiming; }
	virtual bool ComputeAimTarget(const IRDPlayerStateHost& Host, FVector& OutTarget) const override { return ComputeScreenCentreAim(Host, OutTarget); }

	// Ticks only while the camera is blending between normal and aim views.
	virtual bool WantsTick() const override { return bBlending; }
	virtual void TickState(IRDPlayerStateHost& Host, float DeltaSeconds) override;

	bool IsAiming() const { return bAiming; }

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) override;
	virtual void OnExit(IRDPlayerStateHost& Host) override;
	virtual void MapKeys(UInputMappingContext& Context, const URDPlayerIntents& Intents) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	float ArmLength = 350.f;

	// Over-the-shoulder offset (right, up).
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	FVector ShoulderOffset = FVector(0.f, 50.f, 60.f);

	UPROPERTY(EditDefaultsOnly, Category = "Third Person|Aim")
	float AimArmLength = 130.f;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person|Aim")
	FVector AimShoulderOffset = FVector(0.f, 60.f, 65.f);

	UPROPERTY(EditDefaultsOnly, Category = "Third Person|Aim")
	float AimFieldOfView = 65.f;

	// How fast the camera blends in/out of aim (higher = snappier).
	UPROPERTY(EditDefaultsOnly, Category = "Third Person|Aim")
	float AimBlendSpeed = 12.f;

private:
	void SetBodyFacesCamera(IRDPlayerStateHost& Host, bool bFacesCamera) const;

	float NormalFieldOfView = 90.f;
	bool bAiming = false;
	bool bBlending = false;
};
