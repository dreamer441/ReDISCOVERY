using UnrealBuildTool;

// RDPlayerStates: foundational Node Box for the four ways of perceiving/controlling the world.
// Depends only on RDCore and the engine; never on the game module or on magic modules.
public class RDPlayerStates : ModuleRules
{
	public RDPlayerStates(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameplayTags", "EnhancedInput", "RDCore" });
	}
}
