// Lives on anything that can cast (the player's pawn now; creatures later).
// RequestCast(AimTarget) may be called on any machine. The client only sends WHERE it aims; the server works out
// where the cast starts (from its own copy of the body), checks the rules (cooldown, sane aim), and executes it.
// It must NOT know about input keys or player states: the caller supplies the aim point.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RDCastTypes.h"
#include "RDRequestResult.h"
#include "RDCasterComponent.generated.h"

class ARDBasicProjectile;

UCLASS(ClassGroup = (Rediscovery), meta = (BlueprintSpawnableComponent))
class RDCASTING_API URDCasterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URDCasterComponent();

	UFUNCTION(BlueprintCallable, Category = "Casting")
	void RequestCast(FVector AimTarget);

	// Server rules check. Public so tests and debug tools can ask "would this be allowed?".
	FRDRequestResult ValidateCast(const FVector& AimTarget) const;

	// Where a cast starts for a given aim: a little above the body's centre, pushed toward the target.
	FVector ComputeCastOrigin(const FVector& AimTarget) const;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	FRDProjectileModifiers ProjectileModifiers;

	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	TSubclassOf<ARDBasicProjectile> ProjectileClass;

	// Minimum seconds between casts.
	UPROPERTY(EditDefaultsOnly, Category = "Casting")
	float CooldownSeconds = 0.25f;

private:
	UFUNCTION(Server, Reliable)
	void ServerRequestCast(FVector AimTarget);

	void HandleCastOnServer(const FVector& AimTarget);

	double LastCastTime = -1000.0;
};
