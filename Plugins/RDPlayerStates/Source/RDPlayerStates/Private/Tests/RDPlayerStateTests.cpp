// Automation tests for the player-state Node Box. Run with: Tools\RD_Test.cmd Rediscovery.PlayerStates
#include "Misc/AutomationTest.h"
#include "RDPlayerStateComponent.h"
#include "RDPlayerStateDefinition.h"
#include "RDPlayerStateTags.h"

#if WITH_DEV_AUTOMATION_TESTS

// Up cycles FP -> TP -> TD -> FR -> FP, Down goes the other way, both wrap around.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRDPlayerStatesCycleTest, "Rediscovery.PlayerStates.Cycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRDPlayerStatesCycleTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("WrapIndex(4,4)"), URDPlayerStateComponent::WrapIndex(4, 4), 0);
	TestEqual(TEXT("WrapIndex(-1,4)"), URDPlayerStateComponent::WrapIndex(-1, 4), 3);
	TestEqual(TEXT("WrapIndex(x,0)"), URDPlayerStateComponent::WrapIndex(2, 0), static_cast<int32>(INDEX_NONE));

	URDPlayerStateComponent* Component = NewObject<URDPlayerStateComponent>();
	Component->CreateDefinitions();

	const FGameplayTag FP = RDPlayerStateTags::FirstPerson;
	const FGameplayTag TP = RDPlayerStateTags::ThirdPerson;
	const FGameplayTag TD = RDPlayerStateTags::TopDown;
	const FGameplayTag FR = RDPlayerStateTags::FreeRoam;

	TestEqual(TEXT("Up from FirstPerson"), Component->GetNeighborStateTag(FP, +1), TP);
	TestEqual(TEXT("Up from ThirdPerson"), Component->GetNeighborStateTag(TP, +1), TD);
	TestEqual(TEXT("Up from TopDown"), Component->GetNeighborStateTag(TD, +1), FR);
	TestEqual(TEXT("Up from FreeRoam wraps"), Component->GetNeighborStateTag(FR, +1), FP);

	TestEqual(TEXT("Down from FirstPerson wraps"), Component->GetNeighborStateTag(FP, -1), FR);
	TestEqual(TEXT("Down from FreeRoam"), Component->GetNeighborStateTag(FR, -1), TD);

	// Validation: unknown tag refused; known, different tag accepted.
	TestFalse(TEXT("Unknown state refused"), Component->ValidateStateRequest(FGameplayTag()).bAccepted);
	TestTrue(TEXT("Known state accepted"), Component->ValidateStateRequest(TD).bAccepted);
	return true;
}

// The default list holds exactly the four core states, each with a unique tag and a display name.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRDPlayerStatesDefinitionsTest, "Rediscovery.PlayerStates.Definitions",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRDPlayerStatesDefinitionsTest::RunTest(const FString& Parameters)
{
	URDPlayerStateComponent* Component = NewObject<URDPlayerStateComponent>();
	Component->CreateDefinitions();
	const TArray<TObjectPtr<URDPlayerStateDefinition>>& Defs = Component->GetDefinitions();

	TestEqual(TEXT("Four definitions"), Defs.Num(), 4);

	TSet<FGameplayTag> Seen;
	for (const URDPlayerStateDefinition* Def : Defs)
	{
		if (!TestNotNull(TEXT("Definition exists"), Def))
		{
			continue;
		}
		TestTrue(TEXT("Tag valid"), Def->StateTag.IsValid());
		TestFalse(TEXT("Display name set"), Def->DisplayName.IsEmpty());
		TestFalse(FString::Printf(TEXT("Tag %s unique"), *Def->StateTag.ToString()), Seen.Contains(Def->StateTag));
		Seen.Add(Def->StateTag);
	}
	TestTrue(TEXT("Has FirstPerson"), Seen.Contains(RDPlayerStateTags::FirstPerson));
	TestTrue(TEXT("Has ThirdPerson"), Seen.Contains(RDPlayerStateTags::ThirdPerson));
	TestTrue(TEXT("Has TopDown"), Seen.Contains(RDPlayerStateTags::TopDown));
	TestTrue(TEXT("Has FreeRoam"), Seen.Contains(RDPlayerStateTags::FreeRoam));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
