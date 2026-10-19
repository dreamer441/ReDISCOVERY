// One player state (First Person, Third Person, ...) = one subclass of this.
// A state owns: its camera/movement setup (OnEnter/OnExit) and what the player's intents mean while it is active
// (e.g. "Move" walks the body in Third Person but flies the camera in Free Roam).
// A state must NOT: read keys directly, know about other states, or change world state outside the pawn's own rig.
// Adding a fifth state = one new subclass + adding it to URDPlayerStateComponent::StateClasses.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "RDPlayerRigSnapshot.h"
#include "RDPlayerStateDefinition.generated.h"

class IRDPlayerStateHost;
class UInputMappingContext;
class APlayerController;

UCLASS(Abstract)
class RDPLAYERSTATES_API URDPlayerStateDefinition : public UObject
{
	GENERATED_BODY()

public:
	// Which state this is (RD.State.*).
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FGameplayTag StateTag;

	// Name shown on screen when the state becomes active.
	UPROPERTY(EditDefaultsOnly, Category = "State")
	FText DisplayName;

	// Rules check on the server: may the pawn enter this state right now? Default: yes.
	virtual bool CanEnter(const IRDPlayerStateHost& Host, FString& OutWhyNot) const { return true; }

	// Optional extra input mapping the controller should add while this state is active.
	virtual UInputMappingContext* GetInputContext() const { return nullptr; }

	// Called by the component. These take a rig snapshot / restore it around the state's own OnEnter/OnExit.
	void Enter(IRDPlayerStateHost& Host);
	void Exit(IRDPlayerStateHost& Host);

	// Player intents, already translated from devices by the controller. Defaults do nothing (or shared behaviour).
	virtual void HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input) {}          // X = right, Y = forward
	virtual void HandleLook(IRDPlayerStateHost& Host, const FVector2D& Input);           // default: turn the view
	virtual void HandleJump(IRDPlayerStateHost& Host, bool bPressed) {}
	virtual void HandleVertical(IRDPlayerStateHost& Host, float Input) {}                 // +1 up, -1 down
	virtual void HandlePointer(IRDPlayerStateHost& Host, const FVector& WorldTarget) {}   // primary click on the world

	// States that need per-frame work (e.g. walking to a clicked point) return true; the component only ticks then.
	virtual bool WantsTick() const { return false; }
	virtual void TickState(IRDPlayerStateHost& Host, float DeltaSeconds) {}

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) {}
	virtual void OnExit(IRDPlayerStateHost& Host) {}

	// Shared helpers for subclasses.
	static void MoveRelativeToYaw(IRDPlayerStateHost& Host, float Yaw, const FVector2D& Input);
	static void SetJumping(IRDPlayerStateHost& Host, bool bPressed);
	static APlayerController* GetLocalPlayerController(const IRDPlayerStateHost& Host); // null unless this machine controls the pawn

private:
	FRDPlayerRigSnapshot SavedRig;
};
