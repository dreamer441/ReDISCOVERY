#include "RDPlayerStateComponent.h"
#include "RDPlayerStateDefinition.h"
#include "RDPlayerStateTags.h"
#include "RDPlayerStatesLog.h"
#include "IRDPlayerStateHost.h"
#include "RDAuthority.h"
#include "RDCoreTags.h"
#include "States/RDState_FirstPerson.h"
#include "States/RDState_ThirdPerson.h"
#include "States/RDState_TopDown.h"
#include "States/RDState_FreeRoam.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

URDPlayerStateComponent::URDPlayerStateComponent()
{
	// Tick is off by default; it is switched on only while the active state asks for it.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetIsReplicatedByDefault(true);

	// The four core states, in cycling order. A game can replace or extend this list without touching this file.
	StateClasses = {
		URDState_FirstPerson::StaticClass(),
		URDState_ThirdPerson::StaticClass(),
		URDState_TopDown::StaticClass(),
		URDState_FreeRoam::StaticClass(),
	};
	InitialStateTag = RDPlayerStateTags::ThirdPerson;
}

void URDPlayerStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URDPlayerStateComponent, CurrentStateTag);
}

void URDPlayerStateComponent::CreateDefinitions()
{
	if (Definitions.Num() > 0)
	{
		return; // already created
	}
	for (const TSubclassOf<URDPlayerStateDefinition>& StateClass : StateClasses)
	{
		if (*StateClass)
		{
			Definitions.Add(NewObject<URDPlayerStateDefinition>(this, StateClass));
		}
	}
}

void URDPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateDefinitions();

	if (RDAuthority::HasWorldAuthority(GetOwner()))
	{
		HandleStateRequestOnServer(InitialStateTag);
	}
	else if (CurrentStateTag.IsValid() && !ActiveDefinition)
	{
		// The replicated state arrived before BeginPlay; apply it now.
		ApplyState(FGameplayTag(), CurrentStateTag);
	}
}

// ---------- Requests ----------

void URDPlayerStateComponent::RequestNextState()
{
	RequestState(GetNeighborStateTag(CurrentStateTag, +1));
}

void URDPlayerStateComponent::RequestPreviousState()
{
	RequestState(GetNeighborStateTag(CurrentStateTag, -1));
}

void URDPlayerStateComponent::RequestState(FGameplayTag NewState)
{
	if (RDAuthority::HasWorldAuthority(GetOwner()))
	{
		HandleStateRequestOnServer(NewState);
	}
	else
	{
		ServerRequestState(NewState);
	}
}

void URDPlayerStateComponent::ServerRequestState_Implementation(FGameplayTag NewState)
{
	HandleStateRequestOnServer(NewState);
}

FRDRequestResult URDPlayerStateComponent::ValidateStateRequest(const FGameplayTag& NewState) const
{
	const URDPlayerStateDefinition* Definition = FindDefinition(NewState);
	if (!Definition)
	{
		return FRDRequestResult::Refuse(RDCoreTags::Refusal_Unknown, TEXT("no such state in this pawn's list"));
	}
	if (NewState == CurrentStateTag)
	{
		return FRDRequestResult::Refuse(RDCoreTags::Refusal_AlreadyActive, TEXT("already in that state"));
	}
	if (const IRDPlayerStateHost* Host = GetHost())
	{
		FString WhyNot;
		if (!Definition->CanEnter(*Host, WhyNot))
		{
			return FRDRequestResult::Refuse(RDCoreTags::Refusal_Blocked, WhyNot);
		}
	}
	return FRDRequestResult::Accept();
}

void URDPlayerStateComponent::HandleStateRequestOnServer(const FGameplayTag& NewState)
{
	const FRDRequestResult Result = ValidateStateRequest(NewState);
	UE_LOG(LogRDPlayerStates, Log, TEXT("[%s] state %s -> %s : %s %s %s"),
		*GetNameSafe(GetOwner()), *CurrentStateTag.ToString(), *NewState.ToString(),
		Result.bAccepted ? TEXT("ACCEPTED") : TEXT("REFUSED"),
		*Result.RefusalReason.ToString(), *Result.DebugText);

	if (!Result.bAccepted)
	{
		return;
	}
	const FGameplayTag OldState = CurrentStateTag;
	CurrentStateTag = NewState;
	ApplyState(OldState, NewState); // OnRep does not run on the server itself, so apply here.
}

void URDPlayerStateComponent::OnRep_CurrentStateTag(FGameplayTag OldState)
{
	CreateDefinitions();
	ApplyState(OldState, CurrentStateTag);
}

// ---------- Applying ----------

void URDPlayerStateComponent::ApplyState(const FGameplayTag& OldState, const FGameplayTag& NewState)
{
	IRDPlayerStateHost* Host = GetHost();
	if (!Host)
	{
		UE_LOG(LogRDPlayerStates, Warning, TEXT("[%s] owner does not implement IRDPlayerStateHost; state not applied"), *GetNameSafe(GetOwner()));
		return;
	}
	ApplySprint(false); // a sprint never carries over into another state
	if (ActiveDefinition)
	{
		ActiveDefinition->Exit(*Host);
	}
	ActiveDefinition =const_cast<URDPlayerStateDefinition*>(FindDefinition(NewState));
	if (ActiveDefinition)
	{
		ActiveDefinition->Enter(*Host);
	}
	UpdateTickNeed();
	OnStateChanged.Broadcast(OldState, NewState);
}

void URDPlayerStateComponent::ReapplyCurrentState()
{
	IRDPlayerStateHost* Host = GetHost();
	if (ActiveDefinition && Host)
	{
		ActiveDefinition->Exit(*Host);
		ActiveDefinition->Enter(*Host);
		UpdateTickNeed();
	}
}

// ---------- Intents ----------

void URDPlayerStateComponent::HandleMove(const FVector2D& Input)
{
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition) { ActiveDefinition->HandleMove(*Host, Input); }
}

void URDPlayerStateComponent::HandleLook(const FVector2D& Input)
{
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition) { ActiveDefinition->HandleLook(*Host, Input); }
}

void URDPlayerStateComponent::HandleJump(bool bPressed)
{
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition) { ActiveDefinition->HandleJump(*Host, bPressed); }
}

void URDPlayerStateComponent::HandleVertical(float Input)
{
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition) { ActiveDefinition->HandleVertical(*Host, Input); }
}

void URDPlayerStateComponent::HandlePointer(const FVector& WorldTarget)
{
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition)
	{
		ActiveDefinition->HandlePointer(*Host, WorldTarget);
		UpdateTickNeed(); // e.g. Top Down now has somewhere to walk to
	}
}

void URDPlayerStateComponent::HandleAim(bool bPressed)
{
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition)
	{
		ActiveDefinition->HandleAim(*Host, bPressed);
		if (bPressed && !ActiveDefinition->AllowsSprint())
		{
			HandleSprint(false); // e.g. aiming cancels a sprint
		}
		UpdateTickNeed(); // camera blends into / out of aim
	}
}

void URDPlayerStateComponent::HandleCameraDrag(bool bPressed)
{
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition) { ActiveDefinition->HandleCameraDrag(*Host, bPressed); }
}

void URDPlayerStateComponent::HandleSprint(bool bPressed)
{
	const bool bWants = bPressed && ActiveDefinition && ActiveDefinition->AllowsSprint();
	if (bWants == bSprinting)
	{
		return;
	}
	ApplySprint(bWants);
	if (!RDAuthority::HasWorldAuthority(GetOwner()))
	{
		ServerSetSprinting(bWants);
	}
}

void URDPlayerStateComponent::ServerSetSprinting_Implementation(bool bWantsSprint)
{
	// Never trust the client: the server checks its own copy of the active state.
	const bool bAllowed = bWantsSprint && ActiveDefinition && ActiveDefinition->AllowsSprint();
	if (bWantsSprint && !bAllowed)
	{
		UE_LOG(LogRDPlayerStates, Log, TEXT("[%s] sprint REFUSED in %s"), *GetNameSafe(GetOwner()), *CurrentStateTag.ToString());
	}
	ApplySprint(bAllowed);
}

void URDPlayerStateComponent::ApplySprint(bool bWantsSprint)
{
	IRDPlayerStateHost* Host = GetHost();
	UCharacterMovementComponent* Move = Host ? Host->GetMovement() : nullptr;
	if (!Move || bWantsSprint == bSprinting)
	{
		return;
	}
	if (bWantsSprint)
	{
		WalkSpeedBeforeSprint = Move->MaxWalkSpeed;
		Move->MaxWalkSpeed = WalkSpeedBeforeSprint * SprintSpeedMultiplier;
	}
	else
	{
		Move->MaxWalkSpeed = WalkSpeedBeforeSprint;
	}
	bSprinting = bWantsSprint;
}

bool URDPlayerStateComponent::ComputeAimTarget(FVector& OutTarget) const
{
	const IRDPlayerStateHost* Host = GetHost();
	return Host && ActiveDefinition && ActiveDefinition->ComputeAimTarget(*Host, OutTarget);
}

void URDPlayerStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IRDPlayerStateHost* Host = GetHost(); Host && ActiveDefinition)
	{
		ActiveDefinition->TickState(*Host, DeltaTime);
	}
	UpdateTickNeed();
}

void URDPlayerStateComponent::UpdateTickNeed()
{
	const bool bWants = ActiveDefinition && ActiveDefinition->WantsTick();
	if (IsComponentTickEnabled() != bWants)
	{
		SetComponentTickEnabled(bWants);
	}
}

// ---------- Queries ----------

const URDPlayerStateDefinition* URDPlayerStateComponent::FindDefinition(const FGameplayTag& Tag) const
{
	for (const URDPlayerStateDefinition* Definition : Definitions)
	{
		if (Definition && Definition->StateTag == Tag)
		{
			return Definition;
		}
	}
	return nullptr;
}

int32 URDPlayerStateComponent::WrapIndex(int32 Index, int32 Count)
{
	if (Count <= 0)
	{
		return INDEX_NONE;
	}
	return ((Index % Count) + Count) % Count; // works for negative steps too
}

FGameplayTag URDPlayerStateComponent::GetNeighborStateTag(const FGameplayTag& From, int32 Step) const
{
	const int32 Count = Definitions.Num();
	if (Count == 0)
	{
		return FGameplayTag();
	}
	int32 FromIndex = Definitions.IndexOfByPredicate([&From](const URDPlayerStateDefinition* D) { return D && D->StateTag == From; });
	if (FromIndex == INDEX_NONE)
	{
		FromIndex = 0; // unknown/empty current state: count from the first state
		Step = Step > 0 ? 0 : Step;
	}
	return Definitions[WrapIndex(FromIndex + Step, Count)]->StateTag;
}

IRDPlayerStateHost* URDPlayerStateComponent::GetHost() const
{
	return Cast<IRDPlayerStateHost>(GetOwner());
}
