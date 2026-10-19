#include "RDPlayerController.h"
#include "RDPlayerIntents.h"
#include "RDPlayerStateComponent.h"
#include "RDPlayerStateDefinition.h"
#include "RDCasterComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"

void ARDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!Intents)
	{
		Intents = URDPlayerIntents::Create(this);
		// Keys that mean the same thing in every state.
		BaseContext = NewObject<UInputMappingContext>(this, TEXT("IMC_Base"));
		URDPlayerIntents::MapKey(BaseContext, Intents->Look, EKeys::Mouse2D);
		URDPlayerIntents::MapKey(BaseContext, Intents->StateNext, EKeys::Up);
		URDPlayerIntents::MapKey(BaseContext, Intents->StatePrev, EKeys::Down);
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(Intents->Move, ETriggerEvent::Triggered, this, &ARDPlayerController::OnMove);
		Input->BindAction(Intents->Look, ETriggerEvent::Triggered, this, &ARDPlayerController::OnLook);
		Input->BindAction(Intents->Jump, ETriggerEvent::Started, this, &ARDPlayerController::OnJumpPressed);
		Input->BindAction(Intents->Jump, ETriggerEvent::Completed, this, &ARDPlayerController::OnJumpReleased);
		Input->BindAction(Intents->Sprint, ETriggerEvent::Started, this, &ARDPlayerController::OnSprintPressed);
		Input->BindAction(Intents->Sprint, ETriggerEvent::Completed, this, &ARDPlayerController::OnSprintReleased);
		Input->BindAction(Intents->Aim, ETriggerEvent::Started, this, &ARDPlayerController::OnAimPressed);
		Input->BindAction(Intents->Aim, ETriggerEvent::Completed, this, &ARDPlayerController::OnAimReleased);
		Input->BindAction(Intents->CameraDrag, ETriggerEvent::Started, this, &ARDPlayerController::OnCameraDragPressed);
		Input->BindAction(Intents->CameraDrag, ETriggerEvent::Completed, this, &ARDPlayerController::OnCameraDragReleased);
		Input->BindAction(Intents->Vertical, ETriggerEvent::Triggered, this, &ARDPlayerController::OnVertical);
		Input->BindAction(Intents->Point, ETriggerEvent::Triggered, this, &ARDPlayerController::OnPoint);
		Input->BindAction(Intents->Cast, ETriggerEvent::Started, this, &ARDPlayerController::OnCast);
		Input->BindAction(Intents->StateNext, ETriggerEvent::Started, this, &ARDPlayerController::OnStateNext);
		Input->BindAction(Intents->StatePrev, ETriggerEvent::Started, this, &ARDPlayerController::OnStatePrev);
	}

	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(BaseContext, 0);
		}
	}
}

void ARDPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	// Runs on the machine of the player who now controls P: follow its state changes.
	if (URDPlayerStateComponent* States = GetStateComponent())
	{
		States->OnStateChanged.AddUniqueDynamic(this, &ARDPlayerController::HandleStateChanged);
		HandleStateChanged(FGameplayTag(), States->GetCurrentStateTag());
	}
}

URDPlayerStateComponent* ARDPlayerController::GetStateComponent() const
{
	const APawn* MyPawn = GetPawn();
	return MyPawn ? MyPawn->FindComponentByClass<URDPlayerStateComponent>() : nullptr;
}

URDCasterComponent* ARDPlayerController::GetCasterComponent() const
{
	const APawn* MyPawn = GetPawn();
	return MyPawn ? MyPawn->FindComponentByClass<URDCasterComponent>() : nullptr;
}

// ---------- Intents -> systems ----------

void ARDPlayerController::OnMove(const FInputActionValue& Value)
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleMove(Value.Get<FVector2D>()); }
}

void ARDPlayerController::OnLook(const FInputActionValue& Value)
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleLook(Value.Get<FVector2D>()); }
}

void ARDPlayerController::OnJumpPressed()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleJump(true); }
}

void ARDPlayerController::OnJumpReleased()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleJump(false); }
}

void ARDPlayerController::OnSprintPressed()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleSprint(true); }
}

void ARDPlayerController::OnSprintReleased()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleSprint(false); }
}

void ARDPlayerController::OnAimPressed()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleAim(true); }
}

void ARDPlayerController::OnAimReleased()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleAim(false); }
}

void ARDPlayerController::OnCameraDragPressed()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleCameraDrag(true); }
}

void ARDPlayerController::OnCameraDragReleased()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleCameraDrag(false); }
}

void ARDPlayerController::OnVertical(const FInputActionValue& Value)
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleVertical(Value.Get<float>()); }
}

void ARDPlayerController::OnPoint()
{
	URDPlayerStateComponent* States = GetStateComponent();
	FHitResult Hit;
	if (States && GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		States->HandlePointer(Hit.ImpactPoint);
	}
}

void ARDPlayerController::OnCast()
{
	// The active state decides where we aim (centre dot or mouse cursor); casting decides what happens.
	URDPlayerStateComponent* States = GetStateComponent();
	URDCasterComponent* Caster = GetCasterComponent();
	FVector AimTarget;
	if (States && Caster && States->ComputeAimTarget(AimTarget))
	{
		Caster->RequestCast(AimTarget);
	}
}

void ARDPlayerController::OnStateNext()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->RequestNextState(); }
}

void ARDPlayerController::OnStatePrev()
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->RequestPreviousState(); }
}

// ---------- State changes ----------

void ARDPlayerController::HandleStateChanged(FGameplayTag OldState, FGameplayTag NewState)
{
	URDPlayerStateComponent* States = GetStateComponent();
	if (!States || !Intents)
	{
		return;
	}
	URDPlayerStateDefinition* NewDef = const_cast<URDPlayerStateDefinition*>(States->FindDefinition(NewState));

	// Swap the keys: remove the previous state's mapping, add the new one's.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer() ? GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr)
	{
		if (ActiveStateContext)
		{
			Subsystem->RemoveMappingContext(ActiveStateContext);
		}
		ActiveStateContext = NewDef ? NewDef->GetInputContext(*Intents) : nullptr;
		if (ActiveStateContext)
		{
			Subsystem->AddMappingContext(ActiveStateContext, 1);
		}
	}

	if (GEngine && NewDef)
	{
		const FString Text = FString::Printf(TEXT("State: %s   (Up/Down to switch)"), *NewDef->DisplayName.ToString());
		GEngine->AddOnScreenDebugMessage(/*Key*/ 4401, 3.f, FColor::Cyan, Text, true, FVector2D(1.6f, 1.6f));
	}
}
