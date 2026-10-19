# REDISCOVERY — Project State

_Last updated: 2026-10-19 (session: Plan 001 — playable character + four player states)._

## Where we are
Plan 001 is implemented. Pressing Play in the default map (`/Engine/Maps/Templates/OpenWorld`) gives you the default Unreal mannequin (Manny). Up/Down arrows cycle **First Person → Third Person → Top Down → Free Roam** (wrapping around), with the state name shown on screen for 3 s.

| State | Camera | Controls |
|---|---|---|
| 1 First Person | at the head, own body hidden (shadow kept) | WASD walk, mouse look, Space jump |
| 2 Third Person (start state) | spring arm 350 behind the shoulder | WASD walk (camera-relative), mouse orbits, Space jump |
| 3 Top Down | spring arm 1400 at −60°, cursor visible | left-click / hold on the ground to walk there; WASD also walks (screen-relative) |
| 4 Free Roam | camera leaves the body and flies; body stays put | WASD fly, E/Space up, Q/Ctrl down, mouse look; camera snaps back to the body on exit |

## What exists
- **`Plugins/RDCore`** — contracts only: `LogRDCore`, root tags `RD.State` / `RD.Element` / `RD.Function`, generic refusal tags `RD.Refusal.*`, `RDAuthority` helpers, `FRDRequestResult`.
- **`Plugins/RDPlayerStates`** — `URDPlayerStateComponent` (ordered state list, replicated `CurrentStateTag`, server-validated requests, intent forwarding, tick only while a state needs it), `URDPlayerStateDefinition` base, `IRDPlayerStateHost`, `FRDPlayerRigSnapshot` (captured before Enter, restored after Exit, so Exit always fully undoes Enter), the four state classes, `LogRDPlayerStates`.
- **`Source/Rediscovery`** (game module, assembly only) — `ARDGameMode`, `ARDCharacter` (mannequin + boom + camera + state component), `ARDPlayerController` (builds Enhanced Input actions/mapping in code, sends intents to the state component, shows the state name).
- **`Content/Characters/Mannequins`** — the default UE mannequin (meshes, materials, textures, rigs, unarmed animations, `ABP_Unarmed`), copied from the engine's template resources. Pistol/Rifle/Death animations left out (nothing referenced them).
- `Config/DefaultEngine.ini`: `GlobalDefaultGameMode=/Script/Rediscovery.RDGameMode`, editor startup map = default map.
- Desktop shortcut `REDISCOVERY.lnk` opens the project in UE 5.8.

## Tests (`Tools\RD_Test.cmd Rediscovery`) — all pass
- `Rediscovery.PlayerStates.Cycle` — wrap-around order, request validation.
- `Rediscovery.PlayerStates.Definitions` — exactly four unique state tags with names.
- `Rediscovery.PlayerStates.PIECycle` — starts PIE in the default map, injects real Up/Down key presses, checks all four transitions both ways and that the camera is back on the boom after Free Roam.

## Confirmed owner decisions
| Date | Decision |
|---|---|
| 2026-10-19 | Start a clean project at `D:\REDISCOVERY-MAIN\Rediscovery`; canonical repo `github.com/dreamer441/ReDISCOVERY`. MAGICPHS1 is a reference for lessons only. |
| 2026-10-19 | Claude may commit and push reviewed work. Usage-limit handoff rule (see CLAUDE.md). |
| 2026-10-19 | Casting = live key construction **and** prepared spells. |
| 2026-10-19 | Four player states (First Person, Third Person, Top Down, Free Roam) are a core gameplay principle. |
| 2026-10-19 | Default map for now; expand later. |
| 2026-10-19 | Budget: free first; paid only if cheap and clearly worth it. |
| 2026-10-19 | Owner is not a strong C++ reader → plain-language explanations, small commented files. |
| 2026-10-19 | Code module renamed `NEWMAGICwClaude` → `Rediscovery` (targets `Rediscovery` / `RediscoveryEditor`). |
| 2026-10-19 | Use the default Unreal mannequin as the player body (owner request; replaces Plan 001's basic-shapes placeholder). Free Roam vertical = Q/E (Space/Ctrl also work). Top Down = mouse click-to-move. |

## Choices made during Plan 001 (Experimental — change freely)
- Start state is Third Person so the character is visible on spawn.
- Refusal reasons are generic `RD.Refusal.*` tags in RDCore (Plan listed only the three roots).
- Top Down walks to clicked points by itself (no navmesh needed), so it does not path around obstacles yet.
- Free Roam camera moves locally on the owning client (presentation only, not replicated).
- Fixed a bug in `Tools\*.cmd`: `echo EXIT=0> file` was read by cmd as a redirect, leaving `*.status` empty.

## Lessons carried from MAGICPHS1 (see `D:\UNREALPHS1MAGIC\MAGICPHS1\Docs\FOUNDATION_AUDIT_2026-10-19.md`)
- Keep: staged, pure impact solvers with inspectable results; material vs body split; separate orientation from motion; concept metadata with implementation state.
- Avoid: spell as a fixed parameter sheet + element `switch`; casting code polling raw keys; generic modules depending on specific elements; circular-area contact (round craters); mesh-boolean terrain; "first player controller" assumptions; large uncommitted work.

## Tooling
- Project `.claude/skills`: Unreal C++ knowledge (quodsoler/unreal-engine-skills), workflow skills, build-unreal, checkpoint, review-diff, verify-task, Blender art skills.
- `Tools\RD_Build.cmd`, `Tools\RD_Test.cmd [filter]`, `Tools\RD_GitSync.cmd` (logs in `Saved\Logs\rd`). Claude Code can run them directly from its shell.
- Unreal MCP (`unreal-mcp`) was not reachable this session (editor not running); not needed for builds/tests.

## Open decisions (owner)
1. `Content/AI_Tests` and `Content/Developers` are kept out of git via .gitignore; delete later?
2. Does the feel of each state match your intent? (Verified by tests and logs, not yet by a human playing.)

## Next task
Owner plays the four states and gives feel feedback (camera distances, speeds, Top Down click behaviour). Then: Foundation design session (brainstorming → spec → plan) for cast intents, function-step spell data shared by live and prepared casting, the first Earth function, and the debug inspector — each stating its behaviour in all four states.

## Handoff prompt (refresh at the end of every session)
> Continue REDISCOVERY in `D:\REDISCOVERY-MAIN\Rediscovery` (repo github.com/dreamer441/ReDISCOVERY). Read CLAUDE.md and docs/PROJECT_STATE.md first. Plan 001 is done: RDCore + RDPlayerStates plugins, mannequin character, Up/Down cycles four player states, 3 tests pass (`Tools\RD_Test.cmd Rediscovery`). Next: apply the owner's feel feedback on the states, then run the Foundation design session (brainstorming skill) for cast intents and the first Earth function. Present the design for approval before writing code.
