// One player state (First Person, Third Person, ...) = one subclass of this.
// A state owns: its camera/movement setup (OnEnter/OnExit), its key mapping (MapKeys), what the player's
// intents mean while it is active (e.g. "Move" walks the body in Third Person but flies the camera in Free Roam),
// and where a cast is aimed.
// A state must NOT: read keys directly, know about other states, or depend on magic/casting modules.
// Adding a fifth state = one new subclass + adding it to URDPlayerStateComponent::StateClasses.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "RDPlayerRigSnapshot.h"
#include "RDPlayerStateDefinition.generated.h"

class IRDPlayerStateHost;
class UInputMappingContext;
class URDPlayerIntents;
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

	// The keys of this state, built once from MapKeys and cached. The controller adds it while the state is active.
	UInputMappingContext* GetInputContext(const URDPlayerIntents& Intents);

	// Called by the component. These take a rig snapshot / restore it around the state's own OnEnter/OnExit.
	void Enter(IRDPlayerStateHost& Host);
	void Exit(IRDPlayerStateHost& Host);

	// Player intents, already translated from keys. Defaults do nothing (or shared behaviour).
	virtual void HandleMove(IRDPlayerStateHost& Host, const FVector2D& Input) {}          // X = right, Y = forward
	virtual void HandleLook(IRDPlayerStateHost& Host, const FVector2D& Input);           // default: turn the view
	virtual void HandleJump(IRDPlayerStateHost& Host, bool bPressed) {}
	virtual void HandleAim(IRDPlayerStateHost& Host, bool bPressed) {}
	virtual void HandleCameraDrag(IRDPlayerStateHost& Host, bool bPressed) {}
	virtual void HandleVertical(IRDPlayerStateHost& Host, float Input) {}                 // +1 up, -1 down
	virtual void HandlePointer(IRDPlayerStateHost& Host, const FVector& WorldTarget) {}   // click on the world

	// May the body sprint in this state? (Checked again on the server.)
	virtual bool AllowsSprint() const { return false; }

	// Where would a cast go right now? Runs on the controlling machine. False = this state cannot aim.
	virtual bool ComputeAimTarget(const IRDPlayerStateHost& Host, FVector& OutTarget) const { return false; }

	// Should the HUD draw the centre dot?
	virtual bool WantsCrosshair() const { return false; }

	// States that need per-frame work (camera blend, walking to a clicked point) return true; the component only ticks then.
	virtual bool WantsTick() const { return false; }
	virtual void TickState(IRDPlayerStateHost& Host, float DeltaSeconds) {}

protected:
	virtual void OnEnter(IRDPlayerStateHost& Host) {}
	virtual void OnExit(IRDPlayerStateHost& Host) {}
	virtual void MapKeys(UInputMappingContext& Context, const URDPlayerIntents& Intents) const {}

	// Shared helpers for subclasses.
	static void MoveRelativeToYaw(IRDPlayerStateHost& Host, float Yaw, const FVector2D& Input);
	static void SetJumping(IRDPlayerStateHost& Host, bool bPressed);
	static APlayerController* GetLocalPlayerController(const IRDPlayerStateHost& Host); // null unless this machine controls the pawn
	static void SetCursorMode(const IRDPlayerStateHost& Host, bool bCursorVisible);
	static bool ComputeScreenCentreAim(const IRDPlayerStateHost& Host, FVector& OutTarget); // what the centre dot points at
	static bool ComputeCursorAim(const IRDPlayerStateHost& Host, FVector& OutTarget);       // what the mouse cursor points at

private:
	FRDPlayerRigSnapshot SavedRig;

	UPROPERTY(Transient)
	TObjectPtr<UInputMappingContext> CachedInputContext;
};
