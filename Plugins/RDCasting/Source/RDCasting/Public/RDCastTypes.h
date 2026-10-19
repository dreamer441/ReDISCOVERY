// Data shapes for casting.
// PLACEHOLDER (Experimental): until the real magic language exists, the only Function is "basic projectile",
// described by a few Modifiers. Its Results (launch velocity, impact impulse) are calculated from these values,
// never authored as fixed outcomes.
#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "RDCastTypes.generated.h"

namespace RDCastingTags
{
	RDCASTING_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BasicProjectile); // RD.Function.BasicProjectile
}

// Scalable values of the basic projectile Function.
USTRUCT(BlueprintType)
struct RDCASTING_API FRDProjectileModifiers
{
	GENERATED_BODY()

	// Launch speed, cm/s.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Speed = 3000.f;

	// Size of the projectile, cm.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Radius = 10.f;

	// Mass, kg. Impact impulse = mass x velocity at the moment of impact.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Mass = 5.f;

	// Seconds before it fizzles out if it hits nothing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Lifetime = 3.f;
};
