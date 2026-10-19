#include "RDPlayerController.h"
#include "RDPlayerStateComponent.h"
#include "RDPlayerStateDefinition.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"

namespace
{
	UInputAction* MakeAction(UObject* Outer, const TCHAR* Name, EInputActionValueType Type)
	{
		UInputAction* Action = NewObject<UInputAction>(Outer, Name);
		Action->ValueType = Type;
		return Action;
	}

	// Map a key and optionally swap X/Y (so a key can drive the "forward" axis) and/or flip its sign.
	void MapKey(UInputMappingContext* Context, UInputAction* Action, const FKey& Key, bool bSwizzleYX = false, bool bNegate = false)
	{
		FEnhancedActionKeyMapping& Mapping = Context->MapKey(Action, Key);
		if (bSwizzleYX)
		{
			Mapping.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>(Context)); // default order YXZ
		}
		if (bNegate)
		{
			Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(Context));
		}
	}
}

void ARDPlayerController::CreateInputAssets()
{
	if (BaseContext)
	{
		return;
	}
	MoveAction = MakeAction(this, TEXT("IA_Move"), EInputActionValueType::Axis2D);
	LookAction = MakeAction(this, TEXT("IA_Look"), EInputActionValueType::Axis2D);
	JumpAction = MakeAction(this, TEXT("IA_Jump"), EInputActionValueType::Boolean);
	VerticalAction = MakeAction(this, TEXT("IA_Vertical"), EInputActionValueType::Axis1D);
	PointerAction = MakeAction(this, TEXT("IA_Pointer"), EInputActionValueType::Boolean);
	StateNextAction = MakeAction(this, TEXT("IA_StateNext"), EInputActionValueType::Boolean);
	StatePrevAction = MakeAction(this, TEXT("IA_StatePrev"), EInputActionValueType::Boolean);

	BaseContext = NewObject<UInputMappingContext>(this, TEXT("IMC_Base"));
	// Move: X = right/left, Y = forward/back.
	MapKey(BaseContext, MoveAction, EKeys::D);
	MapKey(BaseContext, MoveAction, EKeys::A, false, true);
	MapKey(BaseContext, MoveAction, EKeys::W, true);
	MapKey(BaseContext, MoveAction, EKeys::S, true, true);
	MapKey(BaseContext, LookAction, EKeys::Mouse2D);
	MapKey(BaseContext, JumpAction, EKeys::SpaceBar);
	// Vertical: only Free Roam uses it (fly up/down). Space also jumps; each state picks what it listens to.
	MapKey(BaseContext, VerticalAction, EKeys::E);
	MapKey(BaseContext, VerticalAction, EKeys::SpaceBar);
	MapKey(BaseContext, VerticalAction, EKeys::Q, false, true);
	MapKey(BaseContext, VerticalAction, EKeys::LeftControl, false, true);
	MapKey(BaseContext, PointerAction, EKeys::LeftMouseButton);
	MapKey(BaseContext, StateNextAction, EKeys::Up);
	MapKey(BaseContext, StatePrevAction, EKeys::Down);
}

void ARDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	CreateInputAssets();

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARDPlayerController::OnMove);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARDPlayerController::OnLook);
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ARDPlayerController::OnJumpPressed);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARDPlayerController::OnJumpReleased);
		Input->BindAction(VerticalAction, ETriggerEvent::Triggered, this, &ARDPlayerController::OnVertical);
		Input->BindAction(PointerAction, ETriggerEvent::Triggered, this, &ARDPlayerController::OnPointer);
		Input->BindAction(StateNextAction, ETriggerEvent::Started, this, &ARDPlayerController::OnStateNext);
		Input->BindAction(StatePrevAction, ETriggerEvent::Started, this, &ARDPlayerController::OnStatePrev);
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
	// Runs on the machine of the player who now controls P: listen for state changes to show feedback.
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

// ---------- Device -> intent ----------

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

void ARDPlayerController::OnVertical(const FInputActionValue& Value)
{
	if (URDPlayerStateComponent* States = GetStateComponent()) { States->HandleVertical(Value.Get<float>()); }
}

void ARDPlayerController::OnPointer()
{
	URDPlayerStateComponent* States = GetStateComponent();
	FHitResult Hit;
	if (States && bShowMouseCursor && GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		States->HandlePointer(Hit.ImpactPoint);
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

// ---------- Feedback ----------

void ARDPlayerController::HandleStateChanged(FGameplayTag OldState, FGameplayTag NewState)
{
	URDPlayerStateComponent* States = GetStateComponent();
	if (!States)
	{
		return;
	}
	// Swap the state's own extra input context (none of the current states has one yet).
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer() ? GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr)
	{
		const URDPlayerStateDefinition* OldDef = States->FindDefinition(OldState);
		const URDPlayerStateDefinition* NewDef = States->FindDefinition(NewState);
		if (OldDef && OldDef->GetInputContext()) { Subsystem->RemoveMappingContext(OldDef->GetInputContext()); }
		if (NewDef && NewDef->GetInputContext()) { Subsystem->AddMappingContext(NewDef->GetInputContext(), 1); }
	}

	const URDPlayerStateDefinition* Def = States->FindDefinition(NewState);
	if (GEngine && Def)
	{
		const FString Text = FString::Printf(TEXT("State: %s   (Up/Down to switch)"), *Def->DisplayName.ToString());
		GEngine->AddOnScreenDebugMessage(/*Key*/ 4401, 3.f, FColor::Cyan, Text, true, FVector2D(1.6f, 1.6f));
	}
}
