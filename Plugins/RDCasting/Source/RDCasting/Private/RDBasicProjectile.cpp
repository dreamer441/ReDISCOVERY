#include "RDBasicProjectile.h"
#include "RDCastingLog.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	const FLinearColor ProjectileColor(1.f, 0.45f, 0.1f); // warm orange
	constexpr float EngineSphereRadius = 50.f;           // /Engine/BasicShapes/Sphere is 100 cm across
}

ARDBasicProjectile::ARDBasicProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	// Don't block cameras or mouse/aim traces.
	Collision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Collision->SetNotifyRigidBodyCollision(true);
	RootComponent = Collision;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	Visual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
	Visual->SetupAttachment(Collision);
	Visual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Visual->SetCastShadow(false);
	if (SphereMesh.Succeeded())
	{
		Visual->SetStaticMesh(SphereMesh.Object);
	}

	Glow = CreateDefaultSubobject<UPointLightComponent>(TEXT("Glow"));
	Glow->SetupAttachment(Collision);
	Glow->SetLightColor(ProjectileColor);
	Glow->SetIntensity(8000.f);
	Glow->SetAttenuationRadius(400.f);
	Glow->SetCastShadows(false);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->UpdatedComponent = Collision;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 0.f; // flies straight for now
	Movement->bShouldBounce = false;
	Movement->bInitialVelocityInLocalSpace = false; // InitProjectile gives a world-space direction
}

void ARDBasicProjectile::InitProjectile(const FRDProjectileModifiers& InModifiers, const FVector& Direction)
{
	Modifiers = InModifiers;
	// Size and launch velocity are calculated from the Modifiers.
	Collision->SetSphereRadius(Modifiers.Radius);
	Visual->SetRelativeScale3D(FVector(Modifiers.Radius / EngineSphereRadius));
	Movement->InitialSpeed = Modifiers.Speed;
	Movement->MaxSpeed = Modifiers.Speed;
	Movement->Velocity = Direction.GetSafeNormal() * Modifiers.Speed;
	InitialLifeSpan = Modifiers.Lifetime;
}

void ARDBasicProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (APawn* Caster = GetInstigator())
	{
		Collision->IgnoreActorWhenMoving(Caster, true); // never hit our own caster
	}
	Collision->OnComponentHit.AddDynamic(this, &ARDBasicProjectile::OnProjectileHit);

	// Tint the sphere (the engine's basic shape material exposes a "Color" parameter).
	if (UMaterialInstanceDynamic* MID = Visual->CreateDynamicMaterialInstance(0))
	{
		MID->SetVectorParameterValue(TEXT("Color"), ProjectileColor);
	}
}

void ARDBasicProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
	{
		return; // clients just wait for the server's destroy to replicate
	}
	// Result: momentum transferred to whatever physics object we hit.
	const FVector Impulse = Movement->Velocity * Modifiers.Mass;
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
	}
	UE_LOG(LogRDCasting, Log, TEXT("[%s] hit %s at %s, impulse %.0f"), *GetName(), *GetNameSafe(OtherActor),
		*Hit.ImpactPoint.ToCompactString(), Impulse.Size());
	Destroy();
}

void ARDBasicProjectile::Destroyed()
{
	// Visible feedback where it ended (debug drawing for now; a proper effect comes with real magic).
	if (UWorld* World = GetWorld(); World && World->GetNetMode() != NM_DedicatedServer)
	{
		DrawDebugSphere(World, GetActorLocation(), 40.f, 12, ProjectileColor.ToFColor(true), false, 0.6f, 0, 2.f);
	}
	Super::Destroyed();
}
