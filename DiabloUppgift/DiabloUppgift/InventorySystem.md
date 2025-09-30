# Inventory System — Step 1 (No Inheritance)

This document is a fill‑in worksheet to define your item and inventory data. It stays data‑only (no behavior), matches your current code, and avoids inheritance.

Sections you should customize are marked with TODO.

## How It Fits Your Code
- Combat damage uses `STR * 2` in `DiabloUppgift/CombatComponent.cpp:52`.
- Health/Defense live in `DamagableComponent` used by `Player` and `Enemy`.
- “Use item” exists but is not implemented in `DiabloUppgift/GameManager.cpp:StartCombat`.
- Rooms have a placeholder `AddItem()` in `DiabloUppgift/Room.cpp:44` where floor loot can live.

You will introduce simple data (enums + structs) that these places can read later.

---

## 1) Item Types (Spec‑level categories)
Define the set of item categories you’ll support. These become an `ItemType` enum later.

- Weapon — provides attack bonus
- Armor — provides defense bonus
- HealthPotion — restores HP
- EnergyPotion — restores energy (TODO: decide if you add energy now or later)
- Enchantment — temporary bonuses for X actions

Optional/future: Key, QuestItem, Misc.

---

## 2) Equipment Slots
Keep this list short to start. These become an `EquipmentSlot` enum later.

- mainHand
- offHand (optional for v1)
- head (optional for v1)
- chest

TODO: Confirm which slots you want for v1. Recommended: `mainHand`, `chest`.

---

## 3) ItemSpec (Data table row for each item kind)
Each item kind has one spec row. All fields are data only.

Common fields (for all items):
- id: unique identifier (e.g., ShortSword, LeatherArmor, HealthPotion)
- name: display name
- type: one of ItemType (Weapon/Armor/HealthPotion/EnergyPotion/Enchantment)
- maxStack: how many per inventory slot (equipment usually 1)
- slot: which `EquipmentSlot` it uses (only for equipment; ignored otherwise)

Weapon fields:
- attackBonus: integer added to player attack

Armor fields:
- defenseBonus: integer added to player defense

Consumable fields:
- healAmount: integer HP restored (HealthPotion)
- energyAmount: integer energy restored (EnergyPotion) — TODO if using energy

Enchantment fields:
- actionsDuration: how many player actions before it expires
- attackBonus: optional
- defenseBonus: optional

Example spec rows (edit these, add more as needed):

- id: ShortSword
  - name: "Short Sword"
  - type: Weapon
  - maxStack: 1
  - slot: mainHand
  - attackBonus: +4

- id: LeatherArmor
  - name: "Leather Armor"
  - type: Armor
  - maxStack: 1
  - slot: chest
  - defenseBonus: +2

- id: HealthPotion
  - name: "Health Potion"
  - type: HealthPotion
  - maxStack: 5
  - healAmount: +20

- id: EnergyPotion (optional)
  - name: "Energy Potion"
  - type: EnergyPotion
  - maxStack: 5
  - energyAmount: +20  (TODO: only if you add energy)

- id: FuryEnchant (optional for v1)
  - name: "Fury"
  - type: Enchantment
  - maxStack: 1
  - actionsDuration: 5
  - attackBonus: +2

TODO: Add at least 1 weapon, 1 armor, 1 potion you want in the first playable version.

---

## 4) ItemInstance (What you actually carry)
One struct shape covers all items; some fields are unused for certain types.

- id: the ItemSpec id
- count: stack size (1 for equipment)
- durability: for items that wear out (0 if unused)
- remainingActions: used by enchantments (0 if unused)
- equippedFlag: true if currently occupying a slot

Example instances the player might hold:
- { id=HealthPotion, count=3, durability=0, remainingActions=0, equippedFlag=false }
- { id=ShortSword, count=1, durability=0, remainingActions=0, equippedFlag=true }

---

## 5) Player‑Owned Containers

InventoryState (lives in Player):
- maxSlots: e.g., 10 (TODO: pick a number)
- items: vector of ItemInstance
- Stacking rule: if same id and `count < maxStack`, merge; else if free slot, add; else fail.

EquipmentState (lives in Player):
- mainHand: empty or ItemInstance (equippedFlag=true)
- chest: empty or ItemInstance (equippedFlag=true)
- Optional: offHand, head

ActiveEnchantments (optional for v1):
- list of { id, remainingActions, target: player or equipment slot }

---

## 6) Simple Rules (Decide these now)
- Capacity: `maxSlots = 10` (TODO) and equipment doesn’t consume slots while equipped.
- Equip: can only equip items with `type=Weapon/Armor` and proper `slot`. Equipping moves from inventory to equipment.
- Unequip: returns to inventory if space; fail if full.
- Potions: using a potion reduces `count` and restores HP (and energy if implemented). Remove item when `count == 0`.
- Enchantments: activating creates an entry in ActiveEnchantments; each relevant player action decrements `remainingActions`; when it hits 0, remove and undo bonuses.
- Stacking: only items with `maxStack > 1` can stack (e.g., potions). Equipment does not stack.

---

## 7) Integration Points (Later)
- Show items on the floor: add `std::vector<ItemInstance> myFloorItems;` inside Room (replaces placeholder `AddItem()` in `DiabloUppgift/Room.cpp:44`).
- Inspect/loot UI: when inspecting a room, list `myFloorItems` and allow pickup if inventory has space.
- Player commands: extend `GameManager::PlayerCommand` to include Inventory/Pickup/Drop/Equip/Unequip. Wire into `ExecuteCommand`.
- Use item in combat: in `GameManager::StartCombat`, when choosing “Use item,” list inventory consumables, apply effects, and decrement stacks.
- Apply equipment bonuses: when calculating damage/defense, include equipment bonuses from the equipped items. This affects the values used in `DiabloUppgift/CombatComponent.cpp:52` and `PerformEnemyTurn` defense checks.
- Healing API: add a `Heal(int)` or `RestoreHealth(int)` function to `DamagableComponent` so potions can restore HP safely.

---

## 8) Minimal v1 Scope (Recommended)
- Slots: `mainHand`, `chest` only.
- Items: Short Sword (+4 attack), Leather Armor (+2 defense), Health Potion (+20 HP, stack 5).
- Inventory: 10 slots, stacking only for potions.
- No energy and no enchantments in v1 (define now, implement later).

---

## 9) Open Questions / TODO
- [ ] Finalize v1 equipment slots (mainHand, chest?): mainhand, chest
- [ ] Choose `maxSlots` for inventory: 5 slots for inventory this includes scrolls which are for enchantments
- [ ] Confirm base item list for v1, current list is good, but no energy potions wont implement an energy system
- [ ] Decide if energy exists in v1; if yes, define max and recovery, no energy
- [ ] Which actions decrement enchantments? (Attack, Defend, UseItem, Move, Pickup?): a whole round should be one decrement meaning, the player does an action, the enemy does an action = one decrement
- [ ] Can the player equip/unequip during combat?: no
- [ ] Does equipping/unequipping consume a turn during combat?: cant equip unequip


