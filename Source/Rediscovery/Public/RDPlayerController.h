// Turns devices into player INTENTS and hands them on. It never decides what an intent means.
// - Keys -> intents: each player state supplies its own key mapping; the controller adds the active state's
//   mapping (plus a small base mapping: mouse look, Up/Down state switching) and swaps it on state change.
// - Intents -> systems: movement/camera intents go to the pawn's player-state component; the Cast intent asks the
//   active state where it aims and passes that to the pawn's caster component.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "RDPlayerController.generated.h"

class UInputMappingContext;
class URDPlayerIntents;
class URDPlayerStateComponent;
class URDCasterComponent;
struct FInputActionValue;

UCLASS()
class REDISCOVERY_API ARDPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	virtual void AcknowledgePossession(APawn* P) override;

private:
	URDPlayerStateComponent* GetStateComponent() const;
	URDCasterComponent* GetCasterComponent() const;

	// Intent handlers
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJumpPressed();
	void OnJumpReleased();
	void OnSprintPressed();
	void OnSprintReleased();
	void OnAimPressed();
	void OnAimReleased();
	void OnCameraDragPressed();
	void OnCameraDragReleased();
	void OnVertical(const FInputActionValue& Value);
	void OnPoint();
	void OnCast();
	void OnStateNext();
	void OnStatePrev();

	// Shows the state name on screen and swaps in the new state's key mapping.
	UFUNCTION()
	void HandleStateChanged(FGameplayTag OldState, FGameplayTag NewState);

	UPROPERTY(Transient) TObjectPtr<URDPlayerIntents> Intents;
	UPROPERTY(Transient) TObjectPtr<UInputMappingContext> BaseContext;
	UPROPERTY(Transient) TObjectPtr<UInputMappingContext> ActiveStateContext;
};
