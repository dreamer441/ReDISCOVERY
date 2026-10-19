// The vocabulary of player intents ("move", "cast", "aim"...), as Enhanced Input actions created in code.
// Which KEY produces which intent depends on the active state (Space = Jump in Third Person, Cast in Top Down),
// so each state builds its own key mapping from these actions (URDPlayerStateDefinition::MapKeys).
// Gameplay systems only ever see intents, never raw keys.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputCoreTypes.h"
#include "RDPlayerIntents.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(Transient)
class RDPLAYERSTATES_API URDPlayerIntents : public UObject
{
	GENERATED_BODY()

public:
	static URDPlayerIntents* Create(UObject* Outer);

	UPROPERTY() TObjectPtr<UInputAction> Move;       // 2D: X = right, Y = forward
	UPROPERTY() TObjectPtr<UInputAction> Look;       // 2D mouse delta
	UPROPERTY() TObjectPtr<UInputAction> Jump;
	UPROPERTY() TObjectPtr<UInputAction> Sprint;     // held
	UPROPERTY() TObjectPtr<UInputAction> Aim;        // held (zoom over the shoulder)
	UPROPERTY() TObjectPtr<UInputAction> Cast;       // pressed
	UPROPERTY() TObjectPtr<UInputAction> Point;      // pressed/held on the world (walk there)
	UPROPERTY() TObjectPtr<UInputAction> CameraDrag; // held (mouse moves the camera)
	UPROPERTY() TObjectPtr<UInputAction> Vertical;   // 1D: +1 up, -1 down
	UPROPERTY() TObjectPtr<UInputAction> StateNext;
	UPROPERTY() TObjectPtr<UInputAction> StatePrev;

	// Mapping helpers for states. Swizzle turns a key into the Y ("forward") axis, Negate flips it.
	static void MapKey(UInputMappingContext* Context, const UInputAction* Action, const FKey& Key, bool bSwizzleYX = false, bool bNegate = false);
	static void MapWASD(UInputMappingContext* Context, const UInputAction* Action);
};
