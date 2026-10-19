// The player's body: the default Unreal mannequin with a camera on a spring arm and a player-state component.
// It only provides parts (camera rig, body, movement) to the active player state through IRDPlayerStateHost;
// how those parts are used is decided by the state definitions in RDPlayerStates, not here.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IRDPlayerStateHost.h"
#include "RDCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URDPlayerStateComponent;

UCLASS()
class REDISCOVERY_API ARDCharacter : public ACharacter, public IRDPlayerStateHost
{
	GENERATED_BODY()

public:
	ARDCharacter();

	URDPlayerStateComponent* GetPlayerStateComponent() const { return PlayerStates; }

	// IRDPlayerStateHost
	virtual APawn* GetStatePawn() const override { return const_cast<ARDCharacter*>(this); }
	virtual USpringArmComponent* GetCameraBoom() const override { return CameraBoom; }
	virtual UCameraComponent* GetCamera() const override { return FollowCamera; }
	virtual UPrimitiveComponent* GetBodyMesh() const override;
	virtual UCharacterMovementComponent* GetMovement() const override;

protected:
	// When a controller takes over this body, re-apply the current state so local-only bits
	// (cursor, hidden own body) are set up on the controlling machine.
	virtual void NotifyControllerChanged() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player States")
	TObjectPtr<URDPlayerStateComponent> PlayerStates;
};
