#include "InventoryTypes.h"
#include <cassert>

const ItemSpec gItemSpecs[] =
{
    // ID                     Name               Type                     MaxStack   Weight  HasSlot   Slot                      Atk Def Str Dex Phy Heal Dur
    { ItemId::ShortSword,     "Short Sword",     ItemType::Weapon,        1,         5.0f,   true,     EquipmentSlot::MainHand,  4,  0,  2,  0,  0,   0,  0 },
    { ItemId::BattleAxe,      "Battle Axe",      ItemType::Weapon,        1,         7.0f,   true,     EquipmentSlot::MainHand,  6,  0,  4, -1,  1,   0,  0 },
    { ItemId::LongBow,        "Long Bow",        ItemType::Weapon,        1,        40.0f,   true,     EquipmentSlot::MainHand,  5,  0,  0,  3,  0,   0,  0 },
    { ItemId::LeatherArmor,   "Leather Armor",   ItemType::Armor,         1,         6.0f,   true,     EquipmentSlot::Chest,     0,  2,  0,  0,  1,   0,  0 },
    { ItemId::ChainmailArmor, "Chainmail Armor", ItemType::Armor,         1,        20.0f,   true,     EquipmentSlot::Chest,     0,  3,  0, -1,  2,   0,  0 },
    { ItemId::PlateArmor,     "Plate Armor",     ItemType::Armor,         1,        15.0f,   true,     EquipmentSlot::Chest,     0,  5,  0, -2,  3,   0,  0 },
    { ItemId::HealthPotion,   "Health Potion",   ItemType::HealthPotion,  5,        0.5f,   false,     EquipmentSlot::MainHand,  0,  0,  0,  0,  0,  20,  0 },
    { ItemId::FuryEnchant,    "Fury Scroll",     ItemType::Enchantment,   1,        0.0f,   false,     EquipmentSlot::MainHand,  2,  0,  2,  1,  0,   0,  5 }
};

const size_t gItemSpecCount = sizeof(gItemSpecs) / sizeof(gItemSpecs[0]);

const ItemSpec* FindItemSpec(ItemId id)
{
    for (size_t index = 0; index < gItemSpecCount; ++index)
    {
        if (gItemSpecs[index].id == id)
        {
            return &gItemSpecs[index];
        }
    }

    return nullptr;
}

const ItemSpec& GetItemSpec(ItemId id)
{
    const ItemSpec* spec = FindItemSpec(id);
    assert(spec && "GetItemSpec called with unknown ItemId");

    if (!spec)
    {
        return gItemSpecs[0];
    }

    return *spec;
}

ItemInstance ItemFactory::Make(ItemId id, int count) const
{
    const ItemSpec* spec = nullptr;

    if (specs != nullptr && specCount > 0)
    {
        for (size_t index = 0; index < specCount; ++index)
        {
            if (specs[index].id == id)
            {
                spec = &specs[index];
                break;
            }
        }
    }

    if (spec == nullptr)
    {
        spec = FindItemSpec(id);
    }

    if (count <= 0)
    {
        count = 1;
    }

    assert(spec && "ItemFactory::Make called with unknown ItemId");
    if (!spec)
    {
        ItemInstance fallback{};
        fallback.id = id;
        fallback.count = count;
        return fallback;
    }

    ItemInstance instance{};
    instance.id = id;

    int clampedCount = count;
    if (spec->maxStack > 0 && clampedCount > spec->maxStack)
    {
        clampedCount = spec->maxStack;
    }
    else if (clampedCount < 1)
    {
        clampedCount = 1;
    }

    instance.count = clampedCount;

    if (spec->hasSlot)
    {
        instance.durability = 100;
    }

    if (spec->durationTurns > 0)
    {
        instance.durationTurns = spec->durationTurns;
    }

    instance.equipped = false;
    return instance;
}

namespace
{
    const ItemFactory gDefaultItemFactory{ gItemSpecs, gItemSpecCount };
}

const ItemFactory& GetItemFactory()
{
    return gDefaultItemFactory;
}


