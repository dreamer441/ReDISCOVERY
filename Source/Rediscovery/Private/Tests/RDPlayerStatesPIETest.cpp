// End-to-end checks in Play-In-Editor, driven by injected key presses on the real player controller.
//  PIECycle    - Up/Down cycles the four states in order with wrap-around; camera is back on the body after Free Roam.
//  PIEControls - per-state controls: sprint, aim zoom + centre dot, casting (LMB in 1/2, Space in 3/4),
//                WASD/jump disabled in Top Down, right-mouse orbit in Top Down, Free Roam flying and mouse rules.
// Run with: Tools\RD_Test.cmd Rediscovery.PlayerStates
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS && WITH_EDITOR

#include "Tests/AutomationEditorCommon.h"
#include "Tests/AutomationCommon.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "InputKeyEventArgs.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "RDCharacter.h"
#include "RDPlayerController.h"
#include "RDPlayerStateComponent.h"
#include "RDPlayerStateDefinition.h"
#include "RDPlayerStateTags.h"
#include "RDBasicProjectile.h"

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

	ARDCharacter* FindCharacter()
	{
		ARDPlayerController* PC = FindLocalController();
		return PC ? Cast<ARDCharacter>(PC->GetPawn()) : nullptr;
	}

	FGameplayTag CurrentState()
	{
		ARDCharacter* C = FindCharacter();
		return C ? C->GetPlayerStateComponent()->GetCurrentStateTag() : FGameplayTag();
	}

	int32 CountProjectiles()
	{
		int32 Count = 0;
		if (UWorld* World = GEditor ? GEditor->PlayWorld.Get() : nullptr)
		{
			for (TActorIterator<ARDBasicProjectile> It(World); It; ++It)
			{
				++Count;
			}
		}
		return Count;
	}

	void SendKey(const FKey& Key, EInputEvent Event)
	{
		if (APlayerController* PC = FindLocalController())
		{
			const FInputDeviceId Device = IPlatformInputDeviceMapper::Get().GetDefaultInputDevice();
			PC->InputKey(FInputKeyEventArgs(nullptr, Device, Key, Event, 1.f, false, 0));
		}
	}

	void SendMouseX(float Delta)
	{
		if (APlayerController* PC = FindLocalController())
		{
			const FInputDeviceId Device = IPlatformInputDeviceMapper::Get().GetDefaultInputDevice();
			PC->InputKey(FInputKeyEventArgs(nullptr, Device, EKeys::MouseX, Delta, 1.f / 60.f, 1, 0));
		}
	}

	// Values remembered between latent steps.
	struct FMemo
	{
		int32 Projectiles = 0;
		FVector Location = FVector::ZeroVector;
		float Yaw = 0.f;
		int32 Frames = 0;
	};
}

// Latent-step shorthands: DO runs a block once; WAIT lets the game run for a while.
#define RD_DO(Body) ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([=, this]() { Body; return true; }))
#define RD_WAIT(Seconds) ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(Seconds))
// Holds a key for a moment and releases it.
#define RD_TAP(Key) RD_DO(RDPIETest::SendKey(Key, IE_Pressed)); RD_WAIT(0.05f); RD_DO(RDPIETest::SendKey(Key, IE_Released)); RD_WAIT(0.1f)
// Sends a few frames of mouse movement to the right.
#define RD_MOUSE_RIGHT() ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand([Memo]() { RDPIETest::SendMouseX(10.f); return ++Memo->Frames % 10 == 0; })); RD_WAIT(0.1f)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRDPlayerStatesPIECycleTest, "Rediscovery.PlayerStates.PIECycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRDPlayerStatesPIECycleTest::RunTest(const FString& Parameters)
{
	TSharedRef<RDPIETest::FMemo> Memo = MakeShared<RDPIETest::FMemo>();
	const FGameplayTag Order[] = {
		RDPlayerStateTags::TopDown, RDPlayerStateTags::FreeRoam, RDPlayerStateTags::FirstPerson, RDPlayerStateTags::ThirdPerson, // Up x4
		RDPlayerStateTags::FirstPerson, RDPlayerStateTags::FreeRoam, RDPlayerStateTags::TopDown, RDPlayerStateTags::ThirdPerson, // Down x4
	};

	AutomationOpenMap(TEXT("/Engine/Maps/Templates/OpenWorld")); // in the editor this also starts PIE
	RD_WAIT(3.f);
	RD_DO(TestNotNull(TEXT("Local RDCharacter in PIE"), RDPIETest::FindCharacter()));
	RD_DO(TestEqual(TEXT("Starts in Third Person"), RDPIETest::CurrentState(), RDPlayerStateTags::ThirdPerson.GetTag()));

	for (int32 i = 0; i < UE_ARRAY_COUNT(Order); ++i)
	{
		const FKey Key = i < 4 ? EKeys::Up : EKeys::Down;
		const FGameplayTag Expected = Order[i];
		RD_TAP(Key);
		RD_DO(
			TestEqual(FString::Printf(TEXT("After %s expected %s"), *Key.ToString(), *Expected.ToString()), RDPIETest::CurrentState(), Expected);
			if (Expected != RDPlayerStateTags::FreeRoam)
			{
				ARDCharacter* C = RDPIETest::FindCharacter();
				TestTrue(TEXT("Camera attached to boom"), C && C->GetCamera()->GetAttachParent() == C->GetCameraBoom());
			});
	}
	ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRDPlayerStatesPIEControlsTest, "Rediscovery.PlayerStates.PIEControls",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRDPlayerStatesPIEControlsTest::RunTest(const FString& Parameters)
{
	TSharedRef<RDPIETest::FMemo> Memo = MakeShared<RDPIETest::FMemo>();

	AutomationOpenMap(TEXT("/Engine/Maps/Templates/OpenWorld"));
	RD_WAIT(3.f);
	RD_DO(TestEqual(TEXT("Starts in Third Person"), RDPIETest::CurrentState(), RDPlayerStateTags::ThirdPerson.GetTag()));

	// --- Third Person: sprint ---
	RD_DO(Memo->Yaw = RDPIETest::FindCharacter()->GetCharacterMovement()->MaxWalkSpeed; RDPIETest::SendKey(EKeys::LeftShift, IE_Pressed));
	RD_WAIT(0.1f);
	RD_DO(TestTrue(TEXT("Shift sprints in Third Person"), RDPIETest::FindCharacter()->GetCharacterMovement()->MaxWalkSpeed > Memo->Yaw * 1.5f));
	RD_DO(RDPIETest::SendKey(EKeys::LeftShift, IE_Released));
	RD_WAIT(0.1f);
	RD_DO(TestEqual(TEXT("Release Shift restores walk speed"), RDPIETest::FindCharacter()->GetCharacterMovement()->MaxWalkSpeed, Memo->Yaw));

	// --- Third Person: right mouse aims (zoom + centre dot) ---
	RD_DO(RDPIETest::SendKey(EKeys::RightMouseButton, IE_Pressed));
	RD_WAIT(0.6f);
	RD_DO(
		ARDCharacter* C = RDPIETest::FindCharacter();
		TestTrue(TEXT("Aim shows centre dot"), C->GetPlayerStateComponent()->GetCurrentDefinition()->WantsCrosshair());
		TestTrue(TEXT("Aim pulls camera in"), C->GetCameraBoom()->TargetArmLength < 200.f);
		TestTrue(TEXT("Aim narrows field of view"), C->GetCamera()->FieldOfView < 80.f));
	RD_DO(RDPIETest::SendKey(EKeys::RightMouseButton, IE_Released));
	RD_WAIT(0.6f);
	RD_DO(
		ARDCharacter* C = RDPIETest::FindCharacter();
		TestFalse(TEXT("No dot when not aiming"), C->GetPlayerStateComponent()->GetCurrentDefinition()->WantsCrosshair());
		TestTrue(TEXT("Camera back out after aim"), C->GetCameraBoom()->TargetArmLength > 300.f));

	// --- Third Person: left mouse casts ---
	RD_DO(Memo->Projectiles = RDPIETest::CountProjectiles());
	RD_TAP(EKeys::LeftMouseButton);
	RD_DO(TestTrue(TEXT("Left mouse casts in Third Person"), RDPIETest::CountProjectiles() > Memo->Projectiles));

	// --- First Person: centre dot, sprint, left mouse casts ---
	RD_TAP(EKeys::Down);
	RD_DO(TestEqual(TEXT("Now First Person"), RDPIETest::CurrentState(), RDPlayerStateTags::FirstPerson.GetTag()));
	RD_DO(TestTrue(TEXT("First Person always shows the dot"), RDPIETest::FindCharacter()->GetPlayerStateComponent()->GetCurrentDefinition()->WantsCrosshair()));
	RD_DO(Memo->Yaw = RDPIETest::FindCharacter()->GetCharacterMovement()->MaxWalkSpeed; RDPIETest::SendKey(EKeys::LeftShift, IE_Pressed));
	RD_WAIT(0.1f);
	RD_DO(TestTrue(TEXT("Shift sprints in First Person"), RDPIETest::FindCharacter()->GetCharacterMovement()->MaxWalkSpeed > Memo->Yaw * 1.5f));
	RD_DO(RDPIETest::SendKey(EKeys::LeftShift, IE_Released));
	RD_WAIT(0.4f); // let the cast cooldown pass
	RD_DO(Memo->Projectiles = RDPIETest::CountProjectiles());
	RD_TAP(EKeys::LeftMouseButton);
	RD_DO(TestTrue(TEXT("Left mouse casts in First Person"), RDPIETest::CountProjectiles() > Memo->Projectiles));

	// --- Top Down (Up x2 from First Person) ---
	RD_TAP(EKeys::Up);
	RD_TAP(EKeys::Up);
	RD_DO(TestEqual(TEXT("Now Top Down"), RDPIETest::CurrentState(), RDPlayerStateTags::TopDown.GetTag()));
	RD_WAIT(0.5f);
	// WASD and Space-jump do nothing to the body.
	RD_DO(Memo->Location = RDPIETest::FindCharacter()->GetActorLocation(); RDPIETest::SendKey(EKeys::W, IE_Pressed); RDPIETest::SendKey(EKeys::SpaceBar, IE_Pressed));
	RD_WAIT(0.4f);
	RD_DO(
		RDPIETest::SendKey(EKeys::W, IE_Released); RDPIETest::SendKey(EKeys::SpaceBar, IE_Released);
		const FVector Moved = RDPIETest::FindCharacter()->GetActorLocation() - Memo->Location;
		TestTrue(TEXT("WASD does not walk in Top Down"), Moved.Size2D() < 5.f);
		TestTrue(TEXT("Space does not jump in Top Down"), FMath::Abs(Moved.Z) < 5.f));
	// Mouse alone does not turn the camera; right mouse + mouse moves it around the orbit.
	RD_DO(Memo->Yaw = RDPIETest::FindCharacter()->GetCameraBoom()->GetComponentRotation().Yaw);
	RD_MOUSE_RIGHT();
	RD_DO(TestTrue(TEXT("Mouse alone does not orbit"), FMath::IsNearlyEqual(RDPIETest::FindCharacter()->GetCameraBoom()->GetComponentRotation().Yaw, Memo->Yaw, 0.01f)));
	RD_DO(RDPIETest::SendKey(EKeys::RightMouseButton, IE_Pressed));
	RD_MOUSE_RIGHT();
	RD_DO(
		RDPIETest::SendKey(EKeys::RightMouseButton, IE_Released);
		TestFalse(TEXT("Right mouse drag orbits the camera"), FMath::IsNearlyEqual(RDPIETest::FindCharacter()->GetCameraBoom()->GetComponentRotation().Yaw, Memo->Yaw, 1.f)));

	// --- Free Roam ---
	RD_TAP(EKeys::Up);
	RD_DO(TestEqual(TEXT("Now Free Roam"), RDPIETest::CurrentState(), RDPlayerStateTags::FreeRoam.GetTag()));
	RD_DO(Memo->Yaw = RDPIETest::FindCharacter()->GetCamera()->GetComponentRotation().Yaw);
	RD_MOUSE_RIGHT();
	RD_DO(TestTrue(TEXT("Free Roam camera ignores mouse without right button"), FMath::IsNearlyEqual(RDPIETest::FindCharacter()->GetCamera()->GetComponentRotation().Yaw, Memo->Yaw, 0.01f)));
	RD_DO(Memo->Location = RDPIETest::FindCharacter()->GetCamera()->GetComponentLocation(); RDPIETest::SendKey(EKeys::E, IE_Pressed));
	RD_WAIT(0.3f);
	RD_DO(
		RDPIETest::SendKey(EKeys::E, IE_Released);
		TestTrue(TEXT("E flies the camera up"), RDPIETest::FindCharacter()->GetCamera()->GetComponentLocation().Z > Memo->Location.Z + 50.f));

	ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand());
	return true;
}

#undef RD_DO
#undef RD_WAIT
#undef RD_TAP
#undef RD_MOUSE_RIGHT

#endif
