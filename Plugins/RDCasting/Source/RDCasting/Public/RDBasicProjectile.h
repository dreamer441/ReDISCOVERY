// PLACEHOLDER Function (Experimental): a glowing ball that flies straight, pushes physics objects it hits
// (impulse = mass x impact velocity) and disappears. Spawned only by the server; replicated to clients.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDCastTypes.h"
#include "RDBasicProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class UProjectileMovementComponent;

UCLASS()
class RDCASTING_API ARDBasicProjectile : public AActor
{
	GENERATED_BODY()

public:
	ARDBasicProjectile();

	// Call between deferred spawn and FinishSpawning.
	void InitProjectile(const FRDProjectileModifiers& InModifiers, const FVector& Direction);

	const FRDProjectileModifiers& GetModifiers() const { return Modifiers; }

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> Visual;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<UPointLightComponent> Glow;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> Movement;

private:
	FRDProjectileModifiers Modifiers;
};
