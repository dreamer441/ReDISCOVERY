> **Note (2026-10-19):** the project now lives at `D:\REDISCOVERY-MAIN\Rediscovery` with descriptor `REDISCOVERY.uproject`; paths below that mention `D:\NEWMAGICwCLAUDE\Rediscovery` / `NEWMAGICwClaude.uproject` are historical. Current paths: `Tools\rd_env.cmd`. Commits/pushes are now authorized (see CLAUDE.md).

# REDISCOVERY environment setup

Updated 2026-10-05. Environment setup only; no gameplay code, assets, commits, or pushes created.

## Detected project and engine

- Project root: `D:\NEWMAGICwCLAUDE\Rediscovery`
- Descriptor: `NEWMAGICwClaude.uproject`
- Actual editor target: `NEWMAGICwClaudeEditor` (do not assume RediscoveryEditor or rename automatically)
- Engine: Unreal Engine **5.8.2**, changelist 56702186
- Registry association: descriptor EngineAssociation `5.8`; matching installation verified through Build.version and editor startup
- UnrealEditor.exe: `D:\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe`
- UnrealEditor-Cmd.exe: `D:\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe`
- Build.bat: `D:\UE_5.8\Engine\Build\BatchFiles\Build.bat`
- RunUAT.bat: `D:\UE_5.8\Engine\Build\BatchFiles\RunUAT.bat`

## Git safety

An independent repository was initialized here with explicit user permission because Git previously resolved to an unrelated repository at D:\. That parent repository was not modified. Origin is `https://github.com/dreamer441/ReDISCOVERY.git`; branch is `master`, with no commits. Remote HEAD query returned no refs; no history was fetched, merged, or rewritten.

Git LFS 3.7.1 is configured locally with a pre-push hook and attributes for uasset, umap, fbx, and blend. Filters and attributes were checked. Generated Unreal/plugin output, IDE state, solution files, and temporary logs are ignored; source, config, plugin source, CLAUDE.md, and project Claude configuration remain trackable. The user account has a Git safe.directory entry for only this exact root because sandbox-created Git metadata has a different Windows owner.

All project files are currently untracked. Review the complete initial file set before proposing a first commit; git diff alone omits untracked files. No commit/push is authorized.

## Local AI and Claude

- Claude Code 2.1.289: `C:\Users\sopda\.local\bin\claude.exe`
- Ollama 0.35.1: `C:\Users\sopda\AppData\Local\Programs\Ollama\ollama.exe`
- Model `qwen3-coder:30b` was already present. A read-only prompt returned READY.
- Tested context: 32768; observed processor split: 33% CPU / 67% GPU. Mixed execution is valid.
- Model's prior parameters did not explicitly set num_ctx. Its local definition now sets num_ctx=32768 using the already existing weight layers, with no model download. Previous definition is preserved under ignored `Saved/Setup/qwen3-coder-original.Modelfile`.
- Git Bash: `D:\Git\bin\bash.exe`, version 5.2.37. WSL was not configured. Project settings and launcher supply CLAUDE_CODE_GIT_BASH_PATH.
- Node.js v24.15.0, uv/uvx 0.11.24 are already installed.
- Epic `unreal-engine-skills-for-claude-code@claude-plugins-official` 3.1.1: installed and enabled for this project.
- `ralph-loop@claude-plugins-official` 1.0.0: installed and enabled. Its command file was verified; no loop started. See `.claude/rules/ralph-safety.md`.
- `claude-hud@claude-hud` 0.10.0: installed and enabled. Official setup helper installed the status line and preserved a backup of global Claude settings. Sample stdin rendering passed. It applies to Claude sessions using this user config.
- Desktop resolves to OneDrive: `C:\Users\sopda\OneDrive\Desktop\REDISCOVERY-Claude-Local.cmd`. Launcher opens this root and uses `ollama launch claude --model qwen3-coder:30b`, detected paths, and a Claude context cap of 32768. One noninteractive, tools-disabled Claude response through Ollama returned READY. No extra interactive session was launched.
- CLAUDE.md, four project skills, and project rules are present; all four custom skills passed the skill-creator validator.

## Native Unreal MCP

Verified native plugin identifiers and enabled them in the .uproject: `ModelContextProtocol` (Unreal MCP), `ToolsetRegistry`, `EditorToolset`, `AllToolsets`. Engine files were only read, not changed.

`Config/DefaultEditorPerProjectUserSettings.ini` sets the verified settings class `/Script/ModelContextProtocolEngine.ModelContextProtocolSettings`: bAutoStartServer=True and bEnableToolSearch=True.

The actual editor target built successfully for Win64 Development. Log: ignored `Saved/Logs/environment-build.log`.

A hidden unattended editor for this project generated `.mcp.json` using **Unreal's own** `ModelContextProtocol.GenerateClientConfig ClaudeCode` command, preserving the Blender entry. Native endpoint: `http://127.0.0.1:8000/mcp`. Tool Search was confirmed with 52 discoverable toolsets. HTTP MCP initialize, tools/list, list_toolsets, describe_toolset, get_current_level, and find_actors all succeeded. Current map in that test session was `/Temp/Untitled_1`, returning 145 actors. No actor, level, or asset mutation tool was called and no map was saved.

The temporary editor is closed before handoff. The native endpoint is available only while this project is running in Unreal.

**Human step pending:** Claude reports both project MCP servers as pending approval. Launch the Desktop shortcut, accept project trust if prompted, approve `unreal-mcp` and `blender`, then use `/mcp`. Do not bypass permissions. Reopen this .uproject in Unreal for the live native connection.

## VibeUE

Compatibility confirmed by the current upstream `5-8` branch documentation: VibeUE extends UE 5.8 native ToolsetRegistry/ModelContextProtocol, including landscape, foliage, profiling, and Python services. Not installed yet: upstream requires native MCP setup first; Claude approval/live session verification is pending. After the human step, select a project-local installation and build/validate it against the detected target. Do not replace native MCP or create terrain/gameplay for validation.

Source: https://github.com/kevinpbuckley/VibeUE/blob/5-8/README.md

## Blender

Detected Blender 5.2: `C:\Program Files\Blender Foundation\Blender 5.2\blender.exe`. Blender was not reinstalled.

Current `mcp-for-blender` package was prepared through uvx. The official installer copied its addon to `C:\Users\sopda\AppData\Roaming\Blender Foundation\Blender\5.2\scripts\addons\blender_mcp.py`. An initial console encoding failure was repaired using UTF-8 and the installer rerun successfully. Existing files are preserved by the installer's backups. Project `.mcp.json` registers the detected absolute uvx path, `mcp-for-blender`, and UTF-8 output.

**Human step pending:** Open Blender. Edit > Preferences > Add-ons; search MCP for Blender and enable Interface: MCP for Blender. If absent, use Install from Disk to select the addon file above. In the 3D View press N, open MCP for Blender, and click Start MCP Server (or Connect to Claude, depending on addon UI). Keep optional paid asset generators off. Live read-only scene inspection remains pending.

Source: https://github.com/ahujasid/mcp-for-blender

## Selected Blender skills

Copied only these 14 skills under `.claude/skills`, together with their referenced shared supporting files and MIT license:

blender-director, environment-artist, vegetation-artist, lowpoly-style, stylized-style, **painterly-style** (actual upstream identifier), procedural-modeling, materials, texture-workflow, asset-optimization, collision-proxy, lod-pipeline, unreal-export, qa-review.

Source: https://github.com/arjun988/blender-skills
Pinned source commit: `8f778d2405a214b508d4c7d80742be8e43acdd52`.
Character-specific skills were not installed. Style skills provide optional guidance; their installation does not choose an art direction. Referenced support-file paths were checked; no missing files found.

## Deferred

Serena intentionally not installed. Reconsider when the C++ codebase is large enough to benefit from semantic symbol navigation.

Remaining sequence: human Claude MCP approvals and Blender activation; verify live Claude/native Unreal and Blender read-only queries; install/build/enable compatible VibeUE, with a restart if needed; repeat final verification and update this document. No gameplay work is authorized.
