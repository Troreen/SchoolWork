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
    BattleAxe,
    LongBow,
    LeatherArmor,
    ChainmailArmor,
    PlateArmor,
    HealthPotion,
    FuryEnchant
};

struct ItemSpec
{
    ItemId id;
    const char* name;
    ItemType type;
    int maxStack;
    float weight;

    bool hasSlot;
    EquipmentSlot slot;
    int attackBonus;
    int defenseBonus;

    int strengthModifier;
    int dexterityModifier;
    int physiqueModifier;

    int healAmount;

    int durationTurns;
};

extern const ItemSpec gItemSpecs[];
extern const size_t gItemSpecCount;

const ItemSpec& GetItemSpec(ItemId id);

struct ItemInstance
{
    ItemId id;
    int count = 1;
    int durability = 0;
    int durationTurns = 0;
    bool equipped = false;
};

struct InventoryState
{
    float maxCarryWeight = 50.0f;
    std::vector<ItemInstance> items;
};

struct EquipmentState
{
    bool hasMainHand = false;
    ItemInstance mainHand{};

    bool hasChest = false;
    ItemInstance chest{};
};

struct ActiveEnchantment
{
    ItemId id;
    int durationTurns = 0;
};
