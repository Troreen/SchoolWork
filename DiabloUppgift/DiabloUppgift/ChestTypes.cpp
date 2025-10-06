#include "ChestTypes.h"

#include <assert.h>

const ChestSpec gChestSpecs[] =
{
    {
        ChestId::WoodenChest,
        "Wooden Chest",
        "A simple wooden chest. It looks old but sturdy.",
        false,
        4,
        {
            { ItemId::HealthPotion, 1, 0.7f },
            { ItemId::ShortSword, 1, 0.5f }
        }
    },
    {
        ChestId::IronChest,
        "Iron Chest",
        "A sturdy iron chest. It looks more secure than the wooden one.",
        true,
        6,
        {
            { ItemId::HealthPotion, 2, 0.8f },
            { ItemId::ChainmailArmor, 1, 0.5f }
        }
    }
};

const size_t gChestSpecCount = sizeof(gChestSpecs) / sizeof(gChestSpecs[0]);

const ChestSpec* FindChestSpec(ChestId id)
{
    for (size_t i = 0; i < gChestSpecCount; ++i)
    {
        if (gChestSpecs[i].id == id)
        {
            return &gChestSpecs[i];
        }
    }

    return nullptr;
}

const ChestSpec& GetChestSpec(ChestId id)
{
    const ChestSpec* spec = FindChestSpec(id);
    assert(spec && "GetChestSpec called with unknown ChestId");

    if (!spec)
    {
        return gChestSpecs[0];
    }

    return *spec;
}
