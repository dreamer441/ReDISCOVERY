// Third Person: the camera orbits behind the shoulder on a spring arm; the body turns toward where it walks.
// Controls: WASD walk relative to the camera, mouse orbits the camera, Space jumps.
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

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) override;

	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	float ArmLength = 350.f;

	// Over-the-shoulder offset (right, up).
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	FVector ShoulderOffset = FVector(0.f, 50.f, 60.f);
};
