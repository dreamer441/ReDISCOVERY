// Turns devices (keyboard, mouse) into player INTENTS and hands them to the pawn's player-state component.
// The controller never decides what an intent means; the active player state does.
// For now the input actions and key mapping are created in code (no input assets yet):
//   Move = WASD, Look = mouse, Jump = Space, Vertical = E/Q (and Space/Ctrl) for Free Roam,
//   Pointer = left mouse (Top Down click-to-move), StateNext = Up arrow, StatePrev = Down arrow.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "RDPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class URDPlayerStateComponent;
struct FInputActionValue;

UCLASS()
class REDISCOVERY_API ARDPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	virtual void AcknowledgePossession(APawn* P) override;

private:
	void CreateInputAssets();
	URDPlayerStateComponent* GetStateComponent() const;

	// Device -> intent handlers
	void OnMove(const FInputActionValue& Value);
	void OnLook(const FInputActionValue& Value);
	void OnJumpPressed();
	void OnJumpReleased();
	void OnVertical(const FInputActionValue& Value);
	void OnPointer();
	void OnStateNext();
	void OnStatePrev();

	// Shows the state name on screen and swaps the state's extra input context, if it has one.
	UFUNCTION()
	void HandleStateChanged(FGameplayTag OldState, FGameplayTag NewState);

	UPROPERTY(Transient) TObjectPtr<UInputMappingContext> BaseContext;
	UPROPERTY(Transient) TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> LookAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> VerticalAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PointerAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> StateNextAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> StatePrevAction;
};
