# REDISCOVERY — Permanent Core Context

## Golden Rule

> **Everything that happens in REDISCOVERY is driven by Functions acting on Components.**
>
> **Components are the things that exist and can be injected into Functions.**
>
> **Modifiers define scalable/changeable parameters of those Functions.**
>
> **The interaction produces live runtime Results.**

This applies to spells, impacts, environmental changes, deformation, movement, damage, creation, transformation, physics interactions, and other world outcomes.

## World

The world is created from **Materials and their Properties**.

A Material is defined by a specific set/combination of Properties. Objects and Materials carry relevant Properties as metadata/runtime variables, and those values may be read or changed by magic, actions, physics, or the environment.

Properties describe what something **is**: hardness, density, temperature, moisture, structure, etc.

## Magic

Magic is a **functional language**, not a database of predefined spells.

A spell does not need to exist before it is cast. A cast is assembled live from compatible Components, conceptually:

`Element + Function + Properties + Attributes + Modifiers + Parameters`

**No hard-coded spell outcomes.** Mana cost, damage, deformation, force, range, temperature change, speed, duration, and similar Results are calculated from the actual Function + Components + current values.

## Current Component Language

- **Elements** — the domains Functions are applied through. Not only Earth/Water/Air/Fire; later this may include Arcane, Teleportation, and other non-material magic.
- **Properties** — data that determine what a Material/object is.
- **Modifiers** — scalable/changeable values such as speed, mass, amount, distance, orientation, magnitude, precision, duration, rotation, etc.
- **Attributes** — crafted characteristics of the Function/spell that are not general Material Properties, e.g. shape.
- **Functions** — actions/operations performed using Components.
- **Results** — live-calculated consequences such as mana cost, damage, deformation, force, movement, temperature change, or transformation.

## Function Grammar

Everything should be broadly composable in theory, but not every combination must be valid.

There is a **general grammar** shared across the system and **Element-specific grammar** owned by each Element. Earth-specific rules belong under Earth; another Element may define different rules without rewriting Earth or unrelated systems.

## Node Box Architecture

Every category has its own independent **Node Box**:

`Elements | Materials | Properties | Modifiers | Attributes | Functions | ...`

Each box owns only its own domain and submodules.

Example: `Elements/Earth/...` may contain Earth-specific Function Grammar, but generic Materials, Modifiers, Properties, or unrelated logic must not be scattered inside Earth.

Node Boxes must be **independent but fully compatible**. Adding, removing, replacing, splitting, or redesigning an Element, Property, Modifier, Attribute, Material, Function, or future Component type must not require rebuilding unrelated systems.

## Most Important Flexibility Rule

> **The current classification is not sacred. The architecture is.**

Earth magic is only the first tested case. Future Elements and magic types may reveal that categories need to be split, merged, renamed, or redesigned. The system must make those changes local instead of forcing a rewrite of the whole engine.

## Mental Model

> **WORLD = Components**  
> **WHAT HAPPENS = Functions**  
> **HOW IT CHANGES = Modifiers**  
> **WHAT COMES OUT = live-calculated Results**  
> **STRUCTURE = independent compatible Node Boxes**
