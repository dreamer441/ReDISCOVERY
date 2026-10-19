// First Person: the camera sits at the head and the body turns with the mouse.
// The owning player does not see their own body (other players and shadows still do).
// Controls: WASD walk relative to where you look, mouse looks, Space jumps.
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

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) override;

	// Eye position relative to the capsule centre.
	UPROPERTY(EditDefaultsOnly, Category = "First Person")
	FVector EyeOffset = FVector(10.f, 0.f, 70.f);
};
