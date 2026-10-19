#include "RDPlayerIntents.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"

namespace
{
	UInputAction* MakeAction(UObject* Outer, const TCHAR* Name, EInputActionValueType Type)
	{
		UInputAction* Action = NewObject<UInputAction>(Outer, Name);
		Action->ValueType = Type;
		return Action;
	}
}

URDPlayerIntents* URDPlayerIntents::Create(UObject* Outer)
{
	URDPlayerIntents* I = NewObject<URDPlayerIntents>(Outer);
	I->Move = MakeAction(I, TEXT("IA_Move"), EInputActionValueType::Axis2D);
	I->Look = MakeAction(I, TEXT("IA_Look"), EInputActionValueType::Axis2D);
	I->Jump = MakeAction(I, TEXT("IA_Jump"), EInputActionValueType::Boolean);
	I->Sprint = MakeAction(I, TEXT("IA_Sprint"), EInputActionValueType::Boolean);
	I->Aim = MakeAction(I, TEXT("IA_Aim"), EInputActionValueType::Boolean);
	I->Cast = MakeAction(I, TEXT("IA_Cast"), EInputActionValueType::Boolean);
	I->Point = MakeAction(I, TEXT("IA_Point"), EInputActionValueType::Boolean);
	I->CameraDrag = MakeAction(I, TEXT("IA_CameraDrag"), EInputActionValueType::Boolean);
	I->Vertical = MakeAction(I, TEXT("IA_Vertical"), EInputActionValueType::Axis1D);
	I->StateNext = MakeAction(I, TEXT("IA_StateNext"), EInputActionValueType::Boolean);
	I->StatePrev = MakeAction(I, TEXT("IA_StatePrev"), EInputActionValueType::Boolean);
	return I;
}

void URDPlayerIntents::MapKey(UInputMappingContext* Context, const UInputAction* Action, const FKey& Key, bool bSwizzleYX, bool bNegate)
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

void URDPlayerIntents::MapWASD(UInputMappingContext* Context, const UInputAction* Action)
{
	MapKey(Context, Action, EKeys::D);
	MapKey(Context, Action, EKeys::A, false, true);
	MapKey(Context, Action, EKeys::W, true);
	MapKey(Context, Action, EKeys::S, true, true);
}
