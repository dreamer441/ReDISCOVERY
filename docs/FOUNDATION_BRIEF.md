# REDISCOVERY — Foundation Brief (owner's vision)

Condensed from the owner's October 2026 briefing. **These are directions being explored, not a fixed specification.** When something here conflicts with a later owner decision in `docs/PROJECT_STATE.md`, the later decision wins.

## Identity
- Working concept: *Theory of Everything*. 3D stylized fantasy sandbox / systemic magic RPG in a world shaped by a lost, highly advanced magical civilization.
- Core loop: **Explore → Observe → Investigate → Understand → Experiment → Construct → Apply → Discover.**
- Feeling: wonder, curiosity, freedom, experimentation, intellectual mastery. Progression = growing understanding and capability, not just bigger numbers.
- Identity rule: *we are not building a conventional magic game with a physics system attached; magic is a way of understanding, programming and manipulating the world's underlying rules.*

## Magic as a functional language
- Spells are composed from elements, functions (operations), properties, attributes (e.g. shape), modifiers (mass, speed, direction…), targeting and activation rules. Results are calculated from the composition and the world state.
- The same mass/velocity/shape/material rules apply whether a stone falls naturally or is propelled by magic.
- Casting: **both** live construction with keys **and** prepared spells (owner decision, 19 Oct 2026).
- Authoring directions: node-based editor and direct 3D construction/sculpting; magic circles and runes with a visual language (inspiration: Witch Hat Atelier).
- **Copy / Duplicate:** players record the structure of objects, materials, runes and constructs; reproducing requires mana, materials, control, precision and knowledge. You can hold a circle far beyond your power to cast.
- Barriers: rule-based constructs with targets, conditions and vulnerabilities; understanding beats raw damage.

## The four player states (core gameplay principle)
First Person, Third Person, Top Down, Free Roam. Each is a different way of perceiving and controlling the same world (embodied exploration and hands-on interaction; bodily movement and prepared spells; keyboard/control space for live construction; detached observation and body commands). Default perspective for the final game is not decided; the four states are confirmed as a principle.

## World
- Hybrid handcrafted + generated/assembled, expandable through modular zones (central hub with directional regions, modular cube spaces, vertical floors, floating islands are all candidate topologies; none final).
- Simulated with shared rules, fidelity tiers: full near the player, simplified far away, event/state-transition off-screen.
- Main systems: universal materials & properties; universal functions; elemental grammars (Earth: structure/shape/resistance; Water: flow/volume/merging; Fire: temperature/combustion; Air: pressure/expansion/force); world physics; impact/deformation/destruction derived from mass, speed, shape, contact area, material; spell construction; environmental simulation (weather, wetness, temperature, darkness; properties drive appearance); copy/discovery; barriers; AI/NPCs (AMADEUS direction, kept modular).
- Persistence of terrain, object, material and construction changes is a major goal.
- Narrative: environmental storytelling and mystery. Players arrive on an ancient platform that is secretly part of an enormous summoning/teleport circle of the vanished civilization (idea, not canon).

## First vertical slice (target, built incrementally)
Gentle, cohesive stylized grass field with real underlying earth (thin vegetation over ~10–20 m of diggable dirt/geological layers in the test area); painterly, low-poly-friendly, vibrant art; player movement/camera/inspection/targeting across the four states; a few materials with inspectable properties; Earth magic (creation, shape retention, movement, impact); a small spell composer; at least one interaction where shape, mass, speed and material visibly matter; property-driven visuals; save/load of meaningful changes; debug inspection of real system state; a Copy/Duplicate proof of concept. **A small simulation that genuinely composes beats a beautiful set of hard-coded spells.**

## Later (preserve architecturally, not promised)
Water, Air, Fire grammars; rituals and complex circles; advanced spell programming; barriers/counter-magic; liquids; construction/architecture; advanced destructible terrain; persistent intelligent NPCs (AMADEUS); biomes and world stages; floating/aerial civilizations; weather and dynamic music; multiplayer co-op/combat (local first, server-authoritative-friendly code); research progression; modding (new materials/properties/functions/elements without rewriting core).

## Technology
UE 5.8 C++ for architecture and performance-critical systems; Blueprints/assets where advantageous; Blender pipeline; Git/GitHub. PC (Windows) first. Pressure points: many dynamic objects, editable terrain, physics-driven magic, liquids, system interactions, networking, AI. Impact frames / hit-stop are intentional presentation, never real stalls.

## Owner and constraints
Solo student developer driving vision and systems design; not a strong C++ reader; uses AI agents for implementation. Minimal budget: free tools first; a paid tool only if cheap and a large, proven win. No timeline; build in modular, playable milestones.

## The owner's rules (verbatim intent)
1. Build systems, not collections of exceptions.
2. Physical reality and magical reality agree (exceptions must be explicit rules).
3. Complexity emerges from composition.
4. The architecture must be capable of changing; categories are not sacred.
5. Prioritize a real playable test over speculative completeness.
6. Every system needs an observable result (which functions, components, parameters, why).
7. Performance is part of the architecture (local, event-driven, cached, incremental, lower fidelity where it keeps the gameplay meaning).
8. Keep the player capable of surprising us.
9. Don't confuse long-term vision with immediate scope.
10. Preserve the central identity of REDISCOVERY.

## Still open (do not invent decisions)
Default camera perspective; multiplayer in first release; world layout and size; progression model; combat structure; survival mechanics; editable-terrain implementation; budget/availability; release timeline; monetization.
