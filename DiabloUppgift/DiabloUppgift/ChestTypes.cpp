#include "ChestTypes.h"
#include "Chest.h"
#include "InventoryTypes.h"

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
            { ItemId::HealthPotion, 1, 0.7 },
            { ItemId::ShortSword, 1, 0.5 }
        }
    },
    {
        ChestId::IronChest,
        "Iron Chest",
        "A sturdy iron chest. It looks more secure than the wooden one.",
        true,
        6,
        {
            { ItemId::HealthPotion, 2, 0.8 },
            { ItemId::ChainmailArmor, 1, 0.5 }
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

Chest ChestFactory::Make(ChestId id) const
{
    const ChestSpec* spec = nullptr;

    if (specs != nullptr && specCount > 0)
    {
        for (size_t i = 0; i < specCount; i++)
        {
            if (specs[i].id == id)
            {
                spec = &specs[i];
                break;
            }
        }
    }

    if (!spec)
    {
        spec = FindChestSpec(id);
    }

    assert(spec && "ChestFactory::Make called with unknown ChestId");

    if (!spec)
    {
        return Chest{};
    }

    Chest chest(spec->name, spec->description);
    chest.SetLocked(spec->locked); 
    chest.SetCapacity(spec->capacity); 
    
    const ItemFactory& ItemFactory = GetItemFactory();
    for (const ChestLootEntry& entry : spec->lootTable)
    {
        if (entry.probability <= 0)
        {
            continue;
        }

        // add item outright if probablity is 100
        if (entry.probability >= 1.0)
        {
            chest.AddItem(ItemFactory.Make(entry.itemId, entry.count));
        }
        else
        {
            //TODO: apply RNG rolls once FactoryContext exposes RNG
        }
    }
    
    return Chest();
}

namespace
{
    const ChestFactory gDefaultChestFactory{ gChestSpecs, gChestSpecCount };
}

const ChestFactory& GetChestFactory()
{
    return gDefaultChestFactory;
}