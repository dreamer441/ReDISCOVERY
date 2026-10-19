# Plan 001 — Playable character + four player states (Up/Down switching)

Follows `docs/ARCHITECTURE.md`. Scope: a playable default character in the default map (`/Engine/Maps/Templates/OpenWorld`) that cycles First Person → Third Person → Top Down → Free Roam with **Up/Down arrow keys**. State details (hands, cursor casting, body commands) come later.

## Module layout
```
Plugins/
  RDCore/                                  foundational, no gameplay
    Source/RDCore/
      RDCore.Build.cs                      deps: Core, CoreUObject, Engine, GameplayTags
      Public/RDCoreLog.h                   DECLARE_LOG_CATEGORY_EXTERN(LogRDCore)
      Public/RDCoreTags.h                  UE_DECLARE_GAMEPLAY_TAG_EXTERN roots: RD.State, RD.Element, RD.Function
      Private/RDCoreTags.cpp               UE_DEFINE_GAMEPLAY_TAG(...)
      Public/RDAuthority.h                 small helpers: HasWorldAuthority(Actor), IsLocallyControlled(Pawn)
      Public/RDRequestResult.h             FRDRequestResult { bool bAccepted; FGameplayTag RefusalReason; FString DebugText; }
  RDPlayerStates/                          foundational; depends only on RDCore + engine
    Source/RDPlayerStates/
      RDPlayerStates.Build.cs              deps: RDCore, EnhancedInput, GameplayTags
      Public/RDPlayerStateTags.h           RD.State.FirstPerson / ThirdPerson / TopDown / FreeRoam
      Public/RDPlayerStateDefinition.h     abstract UObject: StateTag, DisplayName, CanEnter(), OnEnter(), OnExit(), GetInputContext()
      Public/IRDPlayerStateHost.h          interface the pawn implements: GetCameraBoom(), GetCamera(), GetBodyMesh(), GetMovement()
      Public/RDPlayerStateComponent.h      on the pawn: ordered TArray of definitions, replicated CurrentStateTag (OnRep),
                                           RequestNextState()/RequestPreviousState() → Server RPC → validate (CanEnter) → set tag
                                           → OnRep applies Exit/Enter on every machine; OnStateChanged delegate
      Private/RDPlayerStateComponent.cpp
      Private/States/RDState_FirstPerson.(h|cpp)   camera at head, owner hides body, pawn yaw follows controller
      Private/States/RDState_ThirdPerson.(h|cpp)   spring arm ~350 behind shoulder, mouse look
      Private/States/RDState_TopDown.(h|cpp)       spring arm ~1400 at −60°, cursor visible, camera-relative WASD
      Private/States/RDState_FreeRoam.(h|cpp)      camera detaches and flies (WASD, Space/Ctrl), body stays in place
      Private/Tests/RDPlayerStateTests.cpp         Rediscovery.PlayerStates.Cycle (wrap order), .Definitions (4 unique tags)
Source/Rediscovery/                        game module = assembly only (nothing depends on it)
  Rediscovery.Build.cs                     deps: RDCore, RDPlayerStates, EnhancedInput
  Public/RDGameMode.h                      DefaultPawn = ARDCharacter, PlayerController = ARDPlayerController
  Public/RDCharacter.h                     ACharacter + IRDPlayerStateHost; spring arm, camera, URDPlayerStateComponent;
                                           placeholder mage body from /Engine/BasicShapes (cylinder robe, sphere head, cone hat)
  Public/RDPlayerController.h              creates Enhanced Input actions/mapping at runtime (no assets yet):
                                           Move (WASD), Look (mouse), Jump (Space), StateNext (Up), StatePrev (Down)
                                           and forwards them as intents to the pawn/state component
Config/DefaultEngine.ini                   GlobalDefaultGameMode=/Script/Rediscovery.RDGameMode
```

## Rules this plan must respect
- State switching is a **request** validated on the server; the result replicates. Works the same in single player.
- No `switch` over states: each state is its own definition class; adding a fifth state = one new class + adding it to the list.
- No `GetFirstPlayerController()`; the controller/pawn passes itself explicitly.
- Each state owns its camera/movement setup in Enter/Exit; Exit must fully undo Enter.
- Log every transition to `LogRDPlayerStates` (from, to, accepted/refused, why).
- On-screen feedback: current state name shown briefly (debug text is fine for now).

## Done when
1. `Tools\RD_Build.cmd` succeeds (`Saved\Logs\rd\build.status` = EXIT=0).
2. `Tools\RD_Test.cmd` passes `Rediscovery.PlayerStates.*`.
3. PIE in the default map: you can walk, look, jump; Up/Down cycles all four states in order with wrap-around; each feels distinct; Free Roam returns the camera to the body on exit.
4. `docs/PROJECT_STATE.md` updated; committed and pushed.
