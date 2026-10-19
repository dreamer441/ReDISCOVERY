// End-to-end check in Play-In-Editor: starts PIE in the default map, presses the real Up / Down keys on the
// player's controller and checks that the four states cycle in order with wrap-around, and that the camera
// is back on the body after leaving Free Roam. Run with: Tools\RD_Test.cmd Rediscovery.PlayerStates
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS && WITH_EDITOR

#include "Tests/AutomationEditorCommon.h"
#include "Tests/AutomationCommon.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "InputKeyEventArgs.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RDCharacter.h"
#include "RDPlayerController.h"
#include "RDPlayerStateComponent.h"
#include "RDPlayerStateTags.h"

namespace RDPIETest
{
	ARDPlayerController* FindLocalController()
	{
		UWorld* World = GEditor ? GEditor->PlayWorld.Get() : nullptr;
		if (!World)
		{
			return nullptr;
		}
		for (TActorIterator<ARDPlayerController> It(World); It; ++It)
		{
			if (It->IsLocalController())
			{
				return *It;
			}
		}
		return nullptr;
	}

	void SendKey(APlayerController* PC, const FKey& Key, EInputEvent Event)
	{
		const FInputDeviceId Device = IPlatformInputDeviceMapper::Get().GetDefaultInputDevice();
		PC->InputKey(FInputKeyEventArgs(nullptr, Device, Key, Event, 1.f, false, 0));
	}
}

// Presses one key (press this frame, release next frame), waits a few frames, then checks the state tag.
class FRDPressKeyAndExpectState : public IAutomationLatentCommand
{
public:
	FRDPressKeyAndExpectState(FAutomationTestBase* InTest, FKey InKey, FGameplayTag InExpected)
		: Test(InTest), Key(InKey), Expected(InExpected) {}

	virtual bool Update() override
	{
		ARDPlayerController* PC = RDPIETest::FindLocalController();
		ARDCharacter* Character = PC ? Cast<ARDCharacter>(PC->GetPawn()) : nullptr;
		if (!Character)
		{
			Test->AddError(TEXT("No local RDPlayerController with an RDCharacter in PIE"));
			return true;
		}
		switch (Frame++)
		{
		case 0: RDPIETest::SendKey(PC, Key, IE_Pressed); return false;
		case 1: RDPIETest::SendKey(PC, Key, IE_Released); return false;
		case 2: case 3: return false; // let the request and Enter/Exit run
		default: break;
		}
		const FGameplayTag Actual = Character->GetPlayerStateComponent()->GetCurrentStateTag();
		Test->TestEqual(FString::Printf(TEXT("After %s expected %s"), *Key.ToString(), *Expected.ToString()), Actual, Expected);

		// Outside Free Roam the camera must sit on the end of the boom (i.e. it came back to the body).
		if (Expected != RDPlayerStateTags::FreeRoam)
		{
			Test->TestTrue(TEXT("Camera attached to boom"), Character->GetCamera()->GetAttachParent() == Character->GetCameraBoom());
		}
		return true;
	}

private:
	FAutomationTestBase* Test;
	FKey Key;
	FGameplayTag Expected;
	int32 Frame = 0;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRDPlayerStatesPIECycleTest, "Rediscovery.PlayerStates.PIECycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRDPlayerStatesPIECycleTest::RunTest(const FString& Parameters)
{
	const FGameplayTag FP = RDPlayerStateTags::FirstPerson;
	const FGameplayTag TP = RDPlayerStateTags::ThirdPerson;
	const FGameplayTag TD = RDPlayerStateTags::TopDown;
	const FGameplayTag FR = RDPlayerStateTags::FreeRoam;

	AutomationOpenMap(TEXT("/Engine/Maps/Templates/OpenWorld")); // in the editor this also starts PIE
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.f));

	// Starts in Third Person. Up x4: TP -> TD -> FR -> FP -> TP (wraps). Down x4: back the other way.
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Up, TD));
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Up, FR));
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Up, FP));
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Up, TP));
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Down, FP));
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Down, FR));
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Down, TD));
	ADD_LATENT_AUTOMATION_COMMAND(FRDPressKeyAndExpectState(this, EKeys::Down, TP));

	ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());
	return true;
}

#endif
