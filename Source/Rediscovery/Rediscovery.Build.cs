// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rediscovery : ModuleRules
{
	public Rediscovery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		// The game module only assembles: pawn, controller and game mode built from the Node Box plugins.
		// Nothing may depend on this module.
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags", "RDCore", "RDPlayerStates" });

		// Editor-only: the Play-In-Editor automation test needs editor helpers.
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "ApplicationCore" });
		}
	}
}
