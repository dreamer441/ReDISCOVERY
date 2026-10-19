# Unreal

Detected engine: Unreal Engine 5.8.2. Detect paths and actual target names before building.
C++ first for architecture; Blueprints mainly configure content and interfaces.
Use narrow modules and narrow subsystem responsibilities. Avoid giant managers, unnecessary Tick, and circular module dependencies.
Use Unreal-native systems when appropriate. Build after C++ changes, inspect errors, repair, and rebuild before claiming success.
Never modify Engine source without explicit approval or delete assets to hide dependency failures.
Native ModelContextProtocol is the primary bridge; extensions must use it.
Environment setup does not authorize gameplay implementation or content creation.

## Unreal MCP efficiency

Unreal MCP Tool Search is enabled.

Do NOT call `list_toolsets` when the relevant toolset is already known.
The full toolset listing is extremely large and wastes local-model context.

Known toolsets:

- Level/map/scene:
  `editor_toolset.toolsets.scene.SceneTools`

- Actors:
  `editor_toolset.toolsets.actor.ActorTools`

- Assets:
  `editor_toolset.toolsets.asset.AssetTools`

- Blueprints:
  `editor_toolset.toolsets.blueprint.BlueprintTools`

- Object/class/property inspection:
  `editor_toolset.toolsets.object.ObjectTools`

- Materials:
  `editor_toolset.toolsets.material.MaterialTools`

Workflow:

1. Use `describe_toolset` on the relevant specific toolset.
2. Read the exact function names returned.
3. Use `call_tool` with an exact returned function name.

Never guess Unreal MCP tool names.

Only call `list_toolsets` when the relevant toolset truly cannot be determined.
