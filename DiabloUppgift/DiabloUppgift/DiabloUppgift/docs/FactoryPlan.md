# Factory Plan

## Overview

This document captures the proposed factory architecture for the dungeon generation systems. Factories give us a single entry point for constructing gameplay objects, while type-object style specs keep data-driven definitions. For now all specs can stay hardcoded; we can externalize later if needed.

## Factory Responsibilities

- **ItemFactory** builds `ItemInstance` objects from `ItemId`, clamping stacks and applying defaults such as durability or enchantment duration.
- **ChestFactory** produces fully populated `Chest` instances using loot tables and delegates item creation to `ItemFactory`.
- **EnemyFactory** instantiates `Enemy` objects from `EnemySpec`, applies difficulty scaling, and seeds inventories/drops with `ItemFactory`.
- **DoorFactory** constructs `Door` objects with orientation, lock state, key requirements, and target room metadata.
- **RoomFactory** creates a `Room`, then composes doors, enemies, chests, and floor loot through their dedicated factories while enforcing room-level constraints.

## Data Specifications

- **ItemSpec** (already in place) remains the canonical item type definition.
- **EnemySpec**: id, name, stats, AI tag, XP value, drop table.
- **ChestSpec**: id, capacity limits, lock state, loot table, rarity modifiers.
- **DoorSpec**: orientation, locked flag, required key id, target room hint.
- **RoomSpec**: theme/biome id, size, desired counts (doors/chests/enemies/items), spawn tables, constraint rules.
- **SpawnTable / WeightedEntry** helpers support weighted random selection; **CountRange** expresses min/max rolls.
- **FactoryContext** aggregates spec registries, RNG handle, difficulty scaler, and shared services.

## Interfaces (sketch)

```cpp
struct ItemFactory {
    const ItemSpec* specs;
    size_t specCount;
    ItemInstance Make(ItemId id, int count = 1) const;
};

struct ChestFactory { Chest Make(const ChestSpec&, const FactoryContext&) const; };
struct EnemyFactory { Enemy Make(const EnemySpec&, const FactoryContext&) const; };
struct DoorFactory  { Door  Make(const DoorSpec&, const FactoryContext&) const; };
struct RoomFactory  { Room  Make(const RoomSpec&, const FactoryContext&) const; };
```

Lookup helpers should return pointers/optionals so we fail fast when presented with an unknown id.

## Composition Flow

1. `RoomFactory::Make` reads `RoomSpec`, decides how many of each element to spawn, and iterates through child factories.
2. For each door spec ? `DoorFactory::Make`.
3. For each enemy roll ? `EnemyFactory::Make`, including inventory/drops via `ItemFactory`.
4. For each chest roll ? `ChestFactory::Make`, which fills loot through `ItemFactory`.
5. Floor loot rolls ? `ItemFactory::Make` and attach directly to the room.
6. Weight, capacity, and difficulty rules live in the respective factory (room-level for totals, chest-level for capacity, enemy-level for stat scaling).

## Error Handling and Instrumentation

- Validate ids (assert in debug, log and skip in release).
- Guard against empty spawn tables.
- Clip overflow situations (stack, capacity, weight) and record diagnostics for tuning.

## Outstanding Decisions

- Determinism vs. entropy: still undecided. We can keep RNG pluggable via `FactoryContext` so the choice remains open.
- Data source: specs are hardcoded for now; we can migrate to external data once systems stabilize.

## TODO (incremental, testable steps)

1. Introduce `ChestSpec` + hardcoded loot tables, implement `ChestFactory`, and verify chest UI/interaction flows (including overweight cases).
2. Create `EnemySpec` data, `EnemyFactory`, and update enemy spawning to use it; test combat stats, drops, and XP awards.
3. Define `DoorSpec` and `DoorFactory`; integrate with room connections and test locked/unlocked interactions.
4. Implement `RoomSpec` and `RoomFactory`, composing earlier factories; test end-to-end room generation with multiple configurations.
5. Revisit RNG strategy (deterministic seeding vs. full randomness) once the pipeline is in place and we understand gameplay needs.
