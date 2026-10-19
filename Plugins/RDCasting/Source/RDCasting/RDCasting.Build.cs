using UnrealBuildTool;

// RDCasting: turns a "cast" intent into a server-validated request and executes it.
// Depends only on RDCore and the engine. It knows nothing about player states or input keys:
// whoever calls it supplies the aim point.
public class RDCasting : ModuleRules
{
	public RDCasting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "GameplayTags", "RDCore" });
	}
}
