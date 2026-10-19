#include "RDCasterComponent.h"
#include "RDBasicProjectile.h"
#include "RDCastingLog.h"
#include "RDCoreTags.h"
#include "RDAuthority.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

namespace
{
	constexpr float MaxAimDistance = 30000.f;  // farther aim points are pulled in along the same direction
	constexpr float OriginHeight = 40.f;       // chest height above the body's centre
	constexpr float OriginForward = 60.f;      // start outside the body
}

URDCasterComponent::URDCasterComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // purely event-driven
	SetIsReplicatedByDefault(true);            // needed for the Server RPC
	ProjectileClass = ARDBasicProjectile::StaticClass();
}

void URDCasterComponent::RequestCast(FVector AimTarget)
{
	if (RDAuthority::HasWorldAuthority(GetOwner()))
	{
		HandleCastOnServer(AimTarget);
	}
	else
	{
		ServerRequestCast(AimTarget);
	}
}

void URDCasterComponent::ServerRequestCast_Implementation(FVector AimTarget)
{
	HandleCastOnServer(AimTarget);
}

FVector URDCasterComponent::ComputeCastOrigin(const FVector& AimTarget) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return FVector::ZeroVector;
	}
	const FVector Centre = Owner->GetActorLocation() + FVector(0.f, 0.f, OriginHeight);
	FVector Flat = AimTarget - Centre;
	Flat.Z = 0.f;
	const FVector Forward = Flat.IsNearlyZero() ? Owner->GetActorForwardVector() : Flat.GetSafeNormal();
	return Centre + Forward * OriginForward;
}

FRDRequestResult URDCasterComponent::ValidateCast(const FVector& AimTarget) const
{
	if (!GetOwner() || !GetWorld())
	{
		return FRDRequestResult::Refuse(RDCoreTags::Refusal_Unknown, TEXT("no owner/world"));
	}
	if (AimTarget.ContainsNaN())
	{
		return FRDRequestResult::Refuse(RDCoreTags::Refusal_Blocked, TEXT("invalid aim"));
	}
	if ((AimTarget - ComputeCastOrigin(AimTarget)).IsNearlyZero())
	{
		return FRDRequestResult::Refuse(RDCoreTags::Refusal_Blocked, TEXT("aim is at the cast origin"));
	}
	if (GetWorld()->GetTimeSeconds() - LastCastTime < CooldownSeconds)
	{
		return FRDRequestResult::Refuse(RDCoreTags::Refusal_Blocked, TEXT("cooldown"));
	}
	if (!*ProjectileClass)
	{
		return FRDRequestResult::Refuse(RDCoreTags::Refusal_Unknown, TEXT("no projectile class"));
	}
	return FRDRequestResult::Accept();
}

void URDCasterComponent::HandleCastOnServer(const FVector& AimTarget)
{
	const FRDRequestResult Result = ValidateCast(AimTarget);
	if (!Result.bAccepted)
	{
		UE_LOG(LogRDCasting, Verbose, TEXT("[%s] cast %s REFUSED %s %s"), *GetNameSafe(GetOwner()),
			*RDCastingTags::BasicProjectile.GetTag().ToString(), *Result.RefusalReason.ToString(), *Result.DebugText);
		return;
	}

	// Server-side geometry: the client's aim point is used only as a direction hint.
	const FVector Origin = ComputeCastOrigin(AimTarget);
	const FVector Direction = (AimTarget - Origin).GetClampedToMaxSize(MaxAimDistance).GetSafeNormal();

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Params.Instigator = Cast<APawn>(GetOwner());
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.bDeferConstruction = true;

	ARDBasicProjectile* Projectile = GetWorld()->SpawnActor<ARDBasicProjectile>(ProjectileClass, Origin, Direction.Rotation(), Params);
	if (!Projectile)
	{
		UE_LOG(LogRDCasting, Warning, TEXT("[%s] cast accepted but projectile failed to spawn"), *GetNameSafe(GetOwner()));
		return;
	}
	Projectile->InitProjectile(ProjectileModifiers, Direction);
	Projectile->FinishSpawning(FTransform(Direction.Rotation(), Origin));
	LastCastTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogRDCasting, Log, TEXT("[%s] cast %s ACCEPTED: origin %s dir %s speed %.0f mass %.1f"), *GetNameSafe(GetOwner()),
		*RDCastingTags::BasicProjectile.GetTag().ToString(), *Origin.ToCompactString(), *Direction.ToCompactString(),
		ProjectileModifiers.Speed, ProjectileModifiers.Mass);
}
