# REDISCOVERY — Project State

_Last updated: 2026-10-19 (session: Plan 001 + controls patch 1 — sprint, aim, basic cast, Top Down orbit, Free Roam rework)._

## Where we are
Plan 001 plus the owner's first controls patch are implemented. Pressing Play in the default map (`/Engine/Maps/Templates/OpenWorld`) gives you the default Unreal mannequin (Manny). Up/Down arrows cycle **First Person → Third Person → Top Down → Free Roam** (wrapping around), with the state name shown on screen for 3 s.

| State | Camera | Controls |
|---|---|---|
| 1 First Person | at the head, own body hidden (shadow kept), centre dot always shown | WASD walk, mouse look, Space jump, Shift sprint, **LMB cast** at the dot |
| 2 Third Person (start state) | spring arm 350 behind the shoulder | WASD walk, mouse orbits, Space jump, Shift sprint, **RMB hold = zoom over the shoulder + centre dot** (body faces camera, no sprint), **LMB cast** at the dot |
| 3 Top Down | invisible orbit 1400 above the body, pitch −60° (player range −80°…−35°), cursor visible | LMB click/hold = walk there, **RMB drag = move camera around the orbit**, **Space = cast at cursor**. WASD and jump disabled. |
| 4 Free Roam | camera leaves the body; does **not** follow the mouse unless RMB held; cursor visible | WASD fly, E up, Q down, RMB drag = turn camera, LMB click = body walks there (placeholder, same as Top Down), **Space = cast at cursor**; camera snaps back to the body on exit |

## What exists
- **`Plugins/RDCore`** — contracts only: `LogRDCore`, root tags `RD.State` / `RD.Element` / `RD.Function`, generic refusal tags `RD.Refusal.*`, `RDAuthority` helpers, `FRDRequestResult`.
- **`Plugins/RDPlayerStates`** — `URDPlayerStateComponent` (ordered state list, replicated `CurrentStateTag`, server-validated requests, intent forwarding, sprint applied locally + on the server, tick only while a state needs it), `URDPlayerStateDefinition` base (each state supplies its own key map via `MapKeys`, its aim point, crosshair wish, sprint permission), `URDPlayerIntents` (the intent vocabulary as Enhanced Input actions built in code), `IRDPlayerStateHost`, `FRDPlayerRigSnapshot` (captured before Enter, restored after Exit, incl. FOV), `FRDClickToMove` helper, the four state classes, `LogRDPlayerStates`.
- **`Plugins/RDCasting`** (new, Experimental placeholder until real magic) — `URDCasterComponent`: client sends only the aim point; server computes the origin from its own body, checks cooldown/sane aim, spawns `ARDBasicProjectile` (glowing orange ball, flies straight, impulse on physics objects = mass × impact velocity, debug-sphere flash where it ends). Modifiers: speed 3000, radius 10, mass 5, lifetime 3 s. Logs to `LogRDCasting`. Knows nothing about states or keys.
- **`Source/Rediscovery`** (game module, assembly only) — `ARDGameMode`, `ARDCharacter` (mannequin + boom + camera + state component + caster), `ARDPlayerController` (base keys: mouse look + Up/Down; swaps in the active state's key map; Cast intent = ask state for aim → caster), `ARDHUD` (centre dot when the state wants it).
- **`Content/Characters/Mannequins`** — the default UE mannequin (meshes, materials, textures, rigs, unarmed animations, `ABP_Unarmed`), copied from the engine's template resources. Pistol/Rifle/Death animations left out (nothing referenced them).
- `Config/DefaultEngine.ini`: `GlobalDefaultGameMode=/Script/Rediscovery.RDGameMode`, editor startup map = default map.
- Desktop shortcut `REDISCOVERY.lnk` opens the project in UE 5.8.

## Tests (`Tools\RD_Test.cmd Rediscovery`) — all pass
- `Rediscovery.PlayerStates.Cycle` — wrap-around order, request validation.
- `Rediscovery.PlayerStates.Definitions` — exactly four unique state tags with names.
- `Rediscovery.PlayerStates.KeyMaps` — each state maps the agreed keys (Space = jump in 1/2, cast in 3/4; LMB = cast in 1/2, walk-to in 3/4; RMB = aim in 2, camera drag in 3/4; no WASD walking in 3).
- `Rediscovery.PlayerStates.PIECycle` — starts PIE in the default map, injects real Up/Down key presses, checks all four transitions both ways and that the camera is back on the boom after Free Roam.
- `Rediscovery.PlayerStates.PIEControls` — PIE with injected keys: Shift sprint (1, 2), RMB aim zoom + dot (2), LMB spawns a projectile (1, 2), WASD/Space don't move the body in 3, mouse alone doesn't orbit but RMB-drag does (3), Free Roam camera ignores mouse without RMB and E flies up (4).
- Not covered by automation: cursor-aimed casting with Space in 3/4 (headless PIE has no mouse cursor) — needs a human check.

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
| 2026-10-19 | Use the default Unreal mannequin as the player body (owner request; replaces Plan 001's basic-shapes placeholder). Top Down = mouse click-to-move. |
| 2026-10-19 | Controls patch 1 (owner): Shift sprint in 1/2; RMB over-shoulder zoom + dot in 2; cast = LMB in 1/2, Space in 3/4 (basic projectile until magic exists); Top Down camera on an orbit moved by RMB drag with limited pitch; no WASD walking in 3/4; Free Roam camera only turns with RMB, Q/E vertical, body moved by mouse like Top Down for now (will be redesigned). |

## Choices made during Plan 001 (Experimental — change freely)
- Start state is Third Person so the character is visible on spawn.
- Refusal reasons are generic `RD.Refusal.*` tags in RDCore (Plan listed only the three roots).
- Top Down walks to clicked points by itself (no navmesh needed), so it does not path around obstacles yet.
- Free Roam camera moves locally on the owning client (presentation only, not replicated).
- First Person always shows the centre dot (owner only specified the dot for Third Person aiming).
- Key maps live in each state class (`MapKeys`); the intent vocabulary (incl. Cast) lives in RDPlayerStates for now. If casting grows its own input needs, move the Cast action to RDCasting.
- Sprint changes `MaxWalkSpeed` on client and server via RPC; fine for single player, may cause small corrections online → later a custom movement component flag.
- Cast cooldown 0.25 s; projectile impact flash is debug drawing (not in shipping builds).
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
> Continue REDISCOVERY in `D:\REDISCOVERY-MAIN\Rediscovery` (repo github.com/dreamer441/ReDISCOVERY). Read CLAUDE.md and docs/PROJECT_STATE.md first. Done: RDCore + RDPlayerStates + RDCasting plugins, mannequin character, four player states (Up/Down) with per-state key maps, sprint, Third Person aim zoom + dot, basic projectile cast (LMB in 1/2, Space in 3/4), Top Down RMB orbit camera, Free Roam rework; 5 tests pass (`Tools\RD_Test.cmd Rediscovery`). Next: apply the owner's next feel feedback, then run the Foundation design session (brainstorming skill) for real cast intents/spell data and the first Earth function. Present the design for approval before writing code.
