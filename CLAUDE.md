# REDISCOVERY

Systemic fantasy sandbox / magic-construction game. Unreal Engine 5.8.2 (`D:\UE_5.8`), C++ first, Windows PC first.
Project root: `D:\REDISCOVERY-MAIN\Rediscovery`. Descriptor: `REDISCOVERY.uproject`. Code module: `Rediscovery`; editor target: `RediscoveryEditor` (renamed from NEWMAGICwClaude on 2026-10-19).
Canonical repository: https://github.com/dreamer441/ReDISCOVERY.git (branch `master`, Git LFS for uasset/umap/fbx/blend).

## Read first
1. `docs/PROJECT_STATE.md` — what exists, what is in progress, decisions, and the next task. Always keep it current.
2. `.claude/rules/REDISCOVERY_CORE_CONTEXT.md` and `docs/REDISCOVERY_SYSTEM_BIBLE.md` — the architectural laws.
3. `docs/FOUNDATION_BRIEF.md` — the owner's vision and scope (directions, not a fixed spec).

## Non-negotiable design principles
- **Core routes. Modules execute. Submodules extend.** No god managers, no central gameplay registries without a proven need, no Core that owns every category.
- Everything that happens is a Function acting on Components; Results are calculated, never hard-coded spell outcomes.
- **The four player states are a core gameplay principle:** First Person, Third Person, Top Down, Free Roam. They are different ways of perceiving and controlling the same world, not camera presets. Every input, casting and interaction feature must state how it behaves in each state.
- Casting supports both **live construction by keys** and **prepared spells**; both produce the same spell data.
- Always playable and presentable: every increment lands end-to-end (logic + visible feedback + test + status entry) in the default map. No invisible features, no broken main map.
- New code takes an explicit caster/instigator; never assume "the first player controller".

## Working rules
- The owner is not a strong C++ reader: explain changes in plain language, keep files small and commented at the "why" level, prefer clear names over cleverness.
- C++ owns architecture; Blueprints configure, expose, assemble and visualize.
- MAGICPHS1 (`D:\UNREALPHS1MAGIC\MAGICPHS1`) is an older prototype. Read it for lessons only when useful; never copy-migrate its code or assets wholesale.
- Stop and ask when an architecture decision is genuinely ambiguous. Mark ideas as Confirmed / Experimental / Speculative / Rejected.
- Verify: build after C++ changes, run tests, read logs, review the diff. Written code is not proof of success.
- Never modify Engine source, delete assets to hide reference failures, or hide compile failures.

## Git
- Committing and pushing to the canonical repo is authorized for reviewed, in-scope work. Small, focused commits with clear messages.
- Never reset hard, clean destructively, force push, or rewrite history. Do not commit generated Unreal output (see .gitignore).
- Use Windows Git (LFS-enabled): `Tools\RD_GitSync.cmd` with the message in `Saved\rd_commit_message.txt`.
- **Usage-limit handoff rule:** when a session is close to its usage limit, the last actions must be (1) update `docs/PROJECT_STATE.md`, (2) commit and push the latest *working* state, (3) output a ready-to-paste prompt describing exactly what we were working on and the next step.

## Build and test (Windows)
- Build: `Tools\RD_Build.cmd` → `Saved\Logs\rd\build.log` and `build.status`.
- Tests: `Tools\RD_Test.cmd [filter]` (default filter `Rediscovery`) → `Saved\Logs\rd\test.log`. Automation test names start with `Rediscovery.`.
- Paths live in `Tools\rd_env.cmd`.

Reports: concise — result, changed files, verification, remaining issues, decisions required.
Ralph loops only for explicitly requested bounded tasks with `--max-iterations` and verifiable completion criteria; loops never commit/push or rewrite history.
