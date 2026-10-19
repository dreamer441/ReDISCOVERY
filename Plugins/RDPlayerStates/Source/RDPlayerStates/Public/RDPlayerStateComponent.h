// Lives on the pawn. Holds the ordered list of state definitions and which one is active.
// Switching is a REQUEST: the server validates it (known state? not already active? CanEnter?) and sets
// CurrentStateTag; that tag replicates and every machine then runs Exit(old) / Enter(new) locally.
// It also forwards player intents (move, look, jump...) to the active state.
// It must NOT contain per-state logic: no switch over states lives here.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RDRequestResult.h"
#include "RDPlayerStateComponent.generated.h"

class URDPlayerStateDefinition;
class IRDPlayerStateHost;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRDOnPlayerStateChanged, FGameplayTag, OldState, FGameplayTag, NewState);

UCLASS(ClassGroup = (Rediscovery), meta = (BlueprintSpawnableComponent))
class RDPLAYERSTATES_API URDPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URDPlayerStateComponent();

	// The states, in the order Up/Down cycles through them. Defaults to the four core states.
	UPROPERTY(EditDefaultsOnly, Category = "Player States")
	TArray<TSubclassOf<URDPlayerStateDefinition>> StateClasses;

	// State the pawn starts in.
	UPROPERTY(EditDefaultsOnly, Category = "Player States")
	FGameplayTag InitialStateTag;

	UPROPERTY(BlueprintAssignable, Category = "Player States")
	FRDOnPlayerStateChanged OnStateChanged;

	// --- Requests (safe to call on any machine; the server decides) ---
	UFUNCTION(BlueprintCallable, Category = "Player States")
	void RequestNextState();

	UFUNCTION(BlueprintCallable, Category = "Player States")
	void RequestPreviousState();

	UFUNCTION(BlueprintCallable, Category = "Player States")
	void RequestState(FGameplayTag NewState);

	// --- Intents from the controller, forwarded to the active state ---
	void HandleMove(const FVector2D& Input);
	void HandleLook(const FVector2D& Input);
	void HandleJump(bool bPressed);
	void HandleVertical(float Input);
	void HandlePointer(const FVector& WorldTarget);

	// Run Exit+Enter again for the current state (e.g. after the pawn gets a controller, so local-only
	// settings such as the mouse cursor are applied on the right machine).
	void ReapplyCurrentState();

	// --- Queries ---
	UFUNCTION(BlueprintPure, Category = "Player States")
	FGameplayTag GetCurrentStateTag() const { return CurrentStateTag; }

	const URDPlayerStateDefinition* GetCurrentDefinition() const { return ActiveDefinition; }
	const URDPlayerStateDefinition* FindDefinition(const FGameplayTag& Tag) const;
	const TArray<TObjectPtr<URDPlayerStateDefinition>>& GetDefinitions() const { return Definitions; }

	// The state "Step" places away from "From" in the ordered list, wrapping around at both ends.
	FGameplayTag GetNeighborStateTag(const FGameplayTag& From, int32 Step) const;

	// Pure wrap-around arithmetic, kept separate so it can be tested on its own.
	static int32 WrapIndex(int32 Index, int32 Count);

	// Creates the definition objects from StateClasses (called automatically; public for tests).
	void CreateDefinitions();

	// Server-side validation of a state change. Public so tests and debug tools can ask "would this be allowed?".
	FRDRequestResult ValidateStateRequest(const FGameplayTag& NewState) const;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(Server, Reliable)
	void ServerRequestState(FGameplayTag NewState);

	// Server only: validate, log, and set the replicated tag.
	void HandleStateRequestOnServer(const FGameplayTag& NewState);

	UFUNCTION()
	void OnRep_CurrentStateTag(FGameplayTag OldState);

	// Runs on every machine when the tag changes: Exit old definition, Enter new one, notify listeners.
	void ApplyState(const FGameplayTag& OldState, const FGameplayTag& NewState);

	IRDPlayerStateHost* GetHost() const;
	void UpdateTickNeed();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentStateTag)
	FGameplayTag CurrentStateTag;

	UPROPERTY(Transient)
	TArray<TObjectPtr<URDPlayerStateDefinition>> Definitions;

	UPROPERTY(Transient)
	TObjectPtr<URDPlayerStateDefinition> ActiveDefinition;
};
