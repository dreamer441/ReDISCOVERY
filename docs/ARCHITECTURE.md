# REDISCOVERY — Architecture Rules

Status legend: **[C]** Confirmed by the owner · **[P]** Proposed default, change only with a logged decision.
These rules exist so that the game can grow from one character in a field to a large online world without rewrites. When a rule blocks a good idea, change the rule in `docs/decisions/` — don't quietly break it.

## 1. The shape of the code

```
            ┌──────────────────────────────┐
            │  Rediscovery (game module)   │  assembles: pawn, controller, game mode, defaults
            └──────────────┬───────────────┘
                           │ depends on
     ┌──────────────┬──────┴───────┬──────────────┐
     ▼              ▼              ▼              ▼
 RDPlayerStates  RDMaterials   RDFunctions    RDElementEarth ...   ← Node Box modules (plugins)
     │              │              │              │   (a submodule may depend on the box it extends)
     └──────────────┴──────┬───────┴──────────────┘
                           ▼
                 RDCore (contracts, IDs, authority, routing, trace)
                           ▼
                    Unreal Engine
```

1. **[C] Core routes and arbitrates. Modules execute. Submodules extend.** Core owns the *contracts* (request/result shapes, IDs, authority rules, the routing seam, tracing). It never contains gameplay logic for a specific feature.
2. **[C] Core decides *whether* something happens; the owning module decides *how*.** Every world-changing action is a **Request** → Core's authority gate validates it (is the caller allowed, is it legal now, can it be paid for) → it is routed to the owning module → the module executes and returns a **Result** → the result is applied, replicated and broadcast.
3. **[C] Dependencies point down only.** Game module → feature modules → Core → Engine. No cycles. A generic module never depends on a specific one (generic casting never includes Earth or Water headers).
4. **[C] One Node Box = one plugin** (`Plugins/RD<Name>`), with `Public/` contracts and `Private/` implementation. Element-specific grammar lives inside that element's plugin.
5. **[P] Nothing depends on the game module.** It only assembles. If two modules need to share something, it moves down into Core or a shared contract module.
6. **[C] The game must still compile and run if a feature plugin is disabled**, unless that plugin is declared foundational (Core, PlayerStates).

## 2. Extension without editing (data over switches)

7. **[C] No `switch` over "kinds of things".** States, elements, functions, materials are **definitions** (classes or data assets) identified by **Gameplay Tags** (`RD.State.FirstPerson`, `RD.Element.Earth`, …). Adding a new one = adding a definition, not editing a list in someone else's code.
8. **[P] Lookups go through small, local tables owned by the module** (e.g. the player-state component holds its own list of state definitions). Core may hold a routing table from a tag to an executor; no global "everything" registry.
9. **[C] Results are calculated** from the function, the components and current values; never stored as authored outcomes.

## 3. Built for online from day one

Single-player is simply a server with one player. Writing code this way costs little now and avoids the classic multiplayer rewrite.

10. **[C] Server-authoritative.** Only the authority (server) changes world state. Clients send requests (Server RPCs) and receive replicated state. Never trust a value sent by a client (costs, positions, damage).
11. **[C] Replicate decisions and descriptions, not every intermediate step.** Replicate "player is in Top Down state" or "spell X was cast with parameters Y", and let clients derive visuals locally.
12. **[C] No "first player" assumptions.** Code takes an explicit owner/instigator/controller. `GetFirstPlayerController()` is forbidden in gameplay code.
13. **[C] Gameplay outcomes don't rely on non-deterministic physics.** Rules compute authoritative results; Chaos physics is used for feel and presentation, or for objects whose exact state is not gameplay-critical.
14. **[P] Presentation is local.** Cameras, UI, VFX, sounds, hit-stop and other feedback run on the owning client and never block the simulation.
15. **[P] Scale honesty:** Unreal's built-in networking comfortably supports tens of players per server (around 100 with care). A "massive" world later means zone/shard servers, a persistence backend and interest management. These rules (authority, small replicated state, stable IDs, event-driven systems) keep that path open; they don't make it free.

## 4. Performance is architecture

16. **[C] Event-driven over Tick.** Tick only when something continuously changes, and switch it off when idle.
17. **[C] Fidelity tiers:** full simulation near players, simplified at distance, state-transition/event processing off-screen. Every simulating system declares how it degrades.
18. **[P] Budgets:** systems that can explode (fragments, projectiles, water bodies) have explicit caps and graceful fallbacks.

## 5. Input and the four states

19. **[C] Four player states are a core principle:** First Person, Third Person, Top Down, Free Roam. Each is a definition object (enter/exit, camera, movement rules, input contexts). New states can be added later as new definitions.
20. **[C] Devices produce intents; modules never poll keys.** Enhanced Input actions and mapping contexts belong to the module that owns the behaviour; the active state decides which contexts are active. Gameplay systems receive intents (e.g. "cast", "switch state"), so AI and network clients use the same path.
21. **[C] Live and prepared casting produce the same spell data** and go through the same request path.

## 6. Identity, persistence, observability

22. **[C] Types are Gameplay Tags; instances that persist get stable IDs** (GUIDs), never actor names.
23. **[P] Save changes as deltas** against the authored/generated world, keyed by stable IDs.
24. **[C] Every request and result is observable**: which function, which components, which parameters, why it was accepted or refused. Each module logs to its own `LogRD<Module>` category, and the debug inspector reads the same trace data players may eventually see as discovery tools.

## 7. Code style (for a developer who reads C++ lightly)

25. **[C] Small files, one responsibility each**, with a short header comment saying what the class is for and what it must *not* do.
26. **[P] Naming:** classes `URD…`/`ARD…`/`FRD…`/`IRD…`, modules `RD<Name>`, tags `RD.<Box>.<Name>`, tests `Rediscovery.<Module>.<Case>`.
27. **[C] C++ owns architecture; Blueprints configure and visualize.** Blueprint subclasses may tune values and art, not replace rules.
28. **[C] Every increment ships with:** a test, visible feedback in the default map, an entry in `docs/PROJECT_STATE.md`, and a clean build.

## 8. Changing these rules
Write a short note in `docs/decisions/NNNN-title.md` (context, decision, consequences), mark the rule here, and update `PROJECT_STATE.md`. Rejected ideas stay recorded so they aren't re-proposed by accident.
