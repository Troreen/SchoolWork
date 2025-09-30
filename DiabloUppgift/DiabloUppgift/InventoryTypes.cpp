#include "InventoryTypes.h"

const ItemSpec gItemSpecs[] =
{
    { ItemId::ShortSword,   "Short Sword",   ItemType::Weapon,      1, true,  EquipmentSlot::MainHand, 4, 0,  0, 0 },
    { ItemId::LeatherArmor, "Leather Armor", ItemType::Armor,       1, true,  EquipmentSlot::Chest,    0, 2,  0, 0 },
    { ItemId::HealthPotion, "Health Potion", ItemType::HealthPotion,5, false, EquipmentSlot::MainHand, 0, 0, 20, 0 },
    { ItemId::FuryEnchant,  "Fury Scroll",   ItemType::Enchantment, 1, false, EquipmentSlot::MainHand, 2, 0,  0, 5 }
};

const size_t gItemSpecCount = sizeof(gItemSpecs) / sizeof(gItemSpecs[0]);

const ItemSpec& GetItemSpec(ItemId id)
{
    for (size_t index = 0; index < gItemSpecCount; ++index)
    {
        if (gItemSpecs[index].id == id)
        {
            return gItemSpecs[index];
        }
    }

    return gItemSpecs[0];
}

