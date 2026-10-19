// Small helper shared by Top Down and Free Roam: "walk the body to the point I clicked".
// Walks in a straight line (no navmesh pathfinding yet).
#pragma once

#include "CoreMinimal.h"
#include "IRDPlayerStateHost.h"
#include "GameFramework/Pawn.h"

struct FRDClickToMove
{
	FVector Destination = FVector::ZeroVector;
	bool bActive = false;
	float ArriveDistance = 60.f; // cm

	void SetDestination(const FVector& WorldTarget)
	{
		Destination = WorldTarget;
		bActive = true;
	}

	void Stop() { bActive = false; }

	// Call every frame while bActive. Stops by itself on arrival.
	void Tick(IRDPlayerStateHost& Host)
	{
		APawn* Pawn = Host.GetStatePawn();
		if (!Pawn || !bActive)
		{
			return;
		}
		FVector ToTarget = Destination - Pawn->GetActorLocation();
		ToTarget.Z = 0.f;
		if (ToTarget.Size() <= ArriveDistance)
		{
			bActive = false;
			return;
		}
		Pawn->AddMovementInput(ToTarget.GetSafeNormal(), 1.f);
	}
};
