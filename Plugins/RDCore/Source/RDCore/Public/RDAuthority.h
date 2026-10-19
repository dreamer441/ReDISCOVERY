// Tiny helpers that answer "who is allowed to decide?" and "is this the local player's pawn?".
// Gameplay code uses these instead of guessing (and never uses "the first player controller").
#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

namespace RDAuthority
{
	// True on the server (or in single player), i.e. where world state may be changed.
	inline bool HasWorldAuthority(const AActor* Actor)
	{
		return Actor != nullptr && Actor->HasAuthority();
	}

	// True on the machine whose player controls this pawn. Cameras, cursor and UI only matter there.
	inline bool IsLocallyControlled(const APawn* Pawn)
	{
		return Pawn != nullptr && Pawn->IsLocallyControlled();
	}
}
