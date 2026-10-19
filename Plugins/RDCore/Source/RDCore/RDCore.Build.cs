using UnrealBuildTool;

// RDCore: the bottom layer every REDISCOVERY module may depend on.
// It holds contracts only (shapes of requests/results, tag roots, authority helpers), never gameplay.
public class RDCore : ModuleRules
{
	public RDCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameplayTags" });
	}
}
