# REDISCOVERY — System & Architecture Bible

## Purpose

This document defines the **conceptual laws of the REDISCOVERY game systems**.

It does **not** prescribe exact implementation, class names, algorithms, or final data structures. Those may change as the game grows.

The goal is to keep the project clean, modular, expandable, physics-based, and easy to restructure when future elements or magic types expose problems that Earth magic has not yet revealed.

---

# 1. Golden Rule

> **Everything that happens in REDISCOVERY is driven by Functions acting on Components.**
>
> **Components are the things that exist and can be injected into Functions.**
>
> **Modifiers describe scalable/changeable parameters of those Functions.**
>
> **The interaction produces live runtime Results.**

This applies broadly to the entire world, not only to spell casting.

A spell cast, an impact, an environmental change, deformation, movement, destruction, creation, heating, cooling, force, damage, material transformation, and similar events should all be understood as **Functions operating on world Components**.

Nothing important should exist only as a hard-coded final outcome when it can instead be derived from the Function and the Components involved.

---

# 2. The World Is Built From Materials and Properties

The physical world is created from **Materials**.

A Material is defined by a specific set and combination of **Properties**.

Examples of Properties may include:

- hardness
- density
- temperature
- moisture
- structure
- elasticity
- conductivity
- brittleness
- state
- other future physical or magical properties

These examples are not a final list.

The important rule is:

> **A Material is not only a name or visual asset. It is a data-defined thing whose Properties describe what it is and how it can behave.**

Objects and materials in the world should carry relevant Properties as metadata or runtime variables.

Those Properties may be read or changed by:

- magic
- physical actions
- other objects
- environmental processes
- future systems

For example, the same stone may become hotter, weaker, fractured, displaced, compressed, melted, or otherwise changed without requiring a separate hard-coded version for every possible state.

---

# 3. Functions Drive What Happens

A **Function** is an action, operation, or interaction.

Functions are the verbs of the world.

Examples:

- move
- accelerate
- heat
- cool
- compress
- expand
- deform
- cut
- fracture
- create
- remove
- transform
- propel
- rotate
- teleport
- bind
- combine
- separate

These examples do not define the final Function library.

A Function receives Components and parameters, performs an interaction, and produces live Results.

The same Function should be reusable wherever its meaning is valid.

A Function should not exist only for one pre-authored spell if the same operation can be expressed generally.

---

# 4. Components

Components are the pieces that make up the world and the magic language.

Current main categories are:

- Elements
- Materials
- Properties
- Modifiers
- Attributes
- Functions

Future development may introduce additional component categories.

The current classification is **provisional**.

The architecture must make it possible to split, merge, rename, or redesign categories later without rebuilding the whole engine.

---

# 5. Elements

**Elements are the primary domains or subjects through which Functions are applied.**

They are not limited to traditional physical elements.

Examples may include:

- Earth
- Water
- Air
- Fire
- Arcane
- Teleportation
- other future magical domains

An Element defines what kinds of Components and Functions make sense inside that domain.

Elements may share general rules, but individual Elements may also define their own **Function Grammar**.

Earth may allow combinations or operations that Fire does not.

Teleportation may require rules that have little relevance to physical Earth manipulation.

Therefore:

> **Every Element should be compatible with the common magic architecture without being forced to behave identically.**

---

# 6. Properties

**Properties are the data that determine what something actually is.**

Properties describe the state and physical or magical characteristics of a Material, object, or other Component.

A specific combination of Properties may define a unique Material.

Properties should be readable by Functions and, where appropriate, mutable at runtime.

Properties are not the same thing as spell crafting instructions.

Examples:

- hardness
- density
- temperature
- moisture
- structure

A Property describes the thing itself.

---

# 7. Modifiers

**Modifiers are scalable or changeable parameters that affect how a Function is performed.**

Their values can vary continuously or discretely.

Examples:

- speed
- mass
- amount
- magnitude
- distance
- orientation
- precision
- duration
- rotation
- scale

A Modifier is not a fixed spell value.

It is part of the live composition.

Changing a Modifier should influence the resulting behavior, cost, force, difficulty, or other calculated Results.

---

# 8. Attributes

**Attributes describe crafted characteristics of a Function or spell composition that are not simply general Properties of the target Material.**

A major example is:

- shape

Other future Attributes may describe constructed or authored characteristics of the spell itself.

The distinction is:

- **Property** = data describing what the thing is
- **Attribute** = crafted characteristic of the Function/composition
- **Modifier** = scalable/changeable value controlling how it happens

This classification may evolve as new magic systems are tested.

---

# 9. Magic Is a Functional Language

Magic is not a database of predefined spells.

> **No spell needs to exist as a hard-coded final object before it is cast.**

A cast is assembled at runtime from compatible Components and Functions.

A simplified conceptual form is:

`Element + Function + Properties + Attributes + Modifiers + Parameters`

The exact syntax may evolve.

The important rule is that a spell is a **temporary functional composition**, not a permanent hard-coded list entry.

A player is effectively constructing an instruction in the game's magic language.

---

# 10. Function Grammar

The system should theoretically support broad composition between Components, but not every possible combination has to be valid in practice.

There should be a distinction between:

## General Grammar

Rules that can be shared across many Elements.

Examples may include common handling for:

- magnitude
- distance
- orientation
- duration
- precision
- shape
- targeting

## Element-Specific Grammar

Rules that belong specifically to one Element or magical domain.

For example:

`Elements/Earth/...`

may define Earth-specific Functions, restrictions, valid combinations, or Element-specific relationships.

Those Earth-specific rules should remain under Earth or its owned submodules rather than being scattered through unrelated systems.

The same principle should apply to every future Element.

---

# 11. No Hard-Coded Spell Results

Damage, mana cost, deformation, force, temperature change, speed, range, duration, and other important Results should be calculated from the actual Function composition and current world state.

Examples:

- a larger amount of material may cost more mana
- higher speed may increase cost or impact
- greater mass may change force
- harder material may resist deformation
- higher precision may require greater control or mana
- longer distance may alter cost
- a heated material may react differently from the same material while cold

The exact formulas are implementation details and may change.

The architectural rule is:

> **Results come from the Function + Components + current values, not from a predefined spell table.**

---

# 12. Results

Results are what comes out of a Function interaction.

Examples:

- mana cost
- damage
- deformation
- displacement
- velocity change
- temperature change
- force
- structural damage
- material transformation
- created or removed matter
- other runtime consequences

Results should be calculated live from the state of the involved Components and the Function being performed.

Results may themselves change Components, causing further Functions or interactions.

---

# 13. Node Box Architecture

Every major category should live in its own clear architectural **Node Box**.

Examples:

- Elements contain Elements
- Materials contain Materials
- Properties contain Properties
- Modifiers contain Modifiers
- Attributes contain Attributes
- Functions contain Functions

Each Node Box should contain only its own domain and its owned submodules.

For example:

`Elements/Earth/`

may contain:

- Earth definition
- Earth-specific Function Grammar
- Earth-specific restrictions
- Earth-specific extensions

But it should not become the home for unrelated Materials, generic Modifiers, or global systems.

Likewise:

`Materials/`

should contain Material definitions and Material-owned submodules.

`Properties/`

should contain Property definitions and Property-owned logic.

The exact folder names or code layout may change, but the conceptual ownership must remain clear.

---

# 14. Independent but Fully Compatible

Node Boxes should be **independent but fully compatible**.

Adding, removing, replacing, or redesigning one Element, Property, Modifier, Attribute, Material, or Function should not require rewriting unrelated systems.

Systems should communicate through clear contracts, shared data formats, or defined interfaces rather than hidden assumptions.

A new Element should be able to join the system by implementing the expected contracts rather than modifying every existing Element.

A removed Modifier should not break Materials that never depended on it.

A new Material should not require rewriting the spell engine.

This is one of the most important reasons for the architecture.

---

# 15. Local Ownership of Specialized Logic

Specialized logic belongs under the system that owns it.

Examples:

- Earth-specific grammar belongs under Earth.
- A Material-specific rule belongs under that Material or Material system.
- A generic distance Modifier belongs in Modifiers, not inside Earth.
- Generic Function execution belongs with Functions/core execution, not duplicated in every Element.

Avoid scattering one system's rules across unrelated folders, classes, assets, or modules.

---

# 16. Functions and Components, Not Prebuilt Content

The engine should prefer reusable Functions and Components over prebuilt one-off behavior.

Instead of creating:

- `EarthSpikeSpell`
- `BigEarthSpikeSpell`
- `FastEarthSpikeSpell`
- `HeavyEarthSpikeSpell`

the system should aim to express the behavior through composition.

Conceptually:

- Element = Earth
- Function = create / move / propel
- Attribute = shape
- Modifier = amount
- Modifier = speed
- Modifier = mass
- Modifier = orientation
- relevant Material Properties

The exact internal implementation is not prescribed here.

The principle is that complexity should emerge from reusable pieces.

---

# 17. World and Magic Must Use the Same Reality

Magic should not operate in a separate fake ruleset when it interacts with the physical world.

If a Material has mass, density, hardness, temperature, structure, or other relevant Properties, magical Functions should interact with those same Properties.

The same world rule should apply whether change comes from:

- player magic
- environment
- another object
- physics
- future systems

Exceptions may exist, but they should be explicit rules rather than accidental inconsistencies.

---

# 18. The Current Classification Is Not Sacred. The Architecture Is.

Earth magic is only the first seriously tested case.

Future systems such as:

- Water
- Fire
- Air
- Arcane
- Teleportation
- rituals
- non-material magic
- unknown future mechanics

may reveal weaknesses in the current classification.

We may discover that:

- one category should split into two
- two categories should merge
- a Modifier is really an Attribute
- a Property needs another layer
- certain Elements need completely different grammar
- a new Component category is necessary

That is acceptable.

What must remain true is:

> **The architecture makes these changes local.**

We should be able to redesign one conceptual box without destroying the rest of the engine.

---

# 19. Expansion Rule

When adding a new system:

1. Determine which Node Box owns it.
2. Reuse existing Functions or Components when their meaning genuinely matches.
3. Add new Components only when the existing language cannot express the concept cleanly.
4. Keep Element-specific grammar inside that Element.
5. Do not create unrelated dependencies for convenience.
6. Do not hard-code final spell outcomes when they can be calculated.
7. Make the new addition removable or replaceable without rewriting unrelated systems.
8. If the existing taxonomy cannot represent the new concept cleanly, improve the taxonomy rather than forcing the feature into the wrong box.

---

# 20. Architectural Summary

The REDISCOVERY system can be reduced to this mental model:

> **WORLD = Components**
>
> **WHAT HAPPENS = Functions**
>
> **HOW IT CHANGES = Modifiers**
>
> **WHAT COMES OUT = live-calculated Results**
>
> **STRUCTURE = independent, compatible Node Boxes**

And the central law remains:

> **Everything that happens in REDISCOVERY is a Function acting on Components. Components define what exists and what is being acted on. Modifiers define scalable/changeable parameters of the action. The interaction produces live runtime Results.**
