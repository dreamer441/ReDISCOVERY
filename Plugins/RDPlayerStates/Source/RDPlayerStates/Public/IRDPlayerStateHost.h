// The pawn that can be in player states implements this interface.
// States reach the camera rig and body only through it, so they work with any pawn that provides these parts.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRDPlayerStateHost.generated.h"

class APawn;
class USpringArmComponent;
class UCameraComponent;
class UPrimitiveComponent;
class UCharacterMovementComponent;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class URDPlayerStateHost : public UInterface
{
	GENERATED_BODY()
};

class RDPLAYERSTATES_API IRDPlayerStateHost
{
	GENERATED_BODY()

public:
	virtual APawn* GetStatePawn() const = 0;
	virtual USpringArmComponent* GetCameraBoom() const = 0;
	virtual UCameraComponent* GetCamera() const = 0;
	virtual UPrimitiveComponent* GetBodyMesh() const = 0;
	virtual UCharacterMovementComponent* GetMovement() const = 0;
};
