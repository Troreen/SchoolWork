#pragma once

#include <vector>
#include <cstddef>

enum class ItemType
{
    Weapon,
    Armor,
    HealthPotion,
    Enchantment
};

enum class EquipmentSlot
{
    MainHand,
    Chest
};

enum class ItemId
{
    ShortSword,
    LeatherArmor,
    HealthPotion,
    FuryEnchant
};

struct ItemSpec
{
    ItemId id;
    const char* name;
    ItemType type;
    int maxStack;

    bool hasSlot;
    EquipmentSlot slot;
    int attackBonus;
    int defenseBonus;

    int healAmount;

    int actionsDuration;
};

extern const ItemSpec gItemSpecs[];
extern const size_t gItemSpecCount;

const ItemSpec& GetItemSpec(ItemId id);

struct ItemInstance
{
    ItemId id;
    int count = 1;
    int durability = 0;
    int remainingActions = 0;
    bool equipped = false;
};

struct InventoryState
{
    int maxSlots = 5;
    std::vector<ItemInstance> items;
};

struct EquipmentState
{
    bool hasMainHand = false;
    ItemInstance mainHand{};

    bool hasChest = false;
    ItemInstance chest{};
};