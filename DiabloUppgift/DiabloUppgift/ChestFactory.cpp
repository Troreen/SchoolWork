#include "ChestFactory.h"

#include "Chest.h"
#include "ItemFactory.h"

#include <assert.h>

Chest ChestFactory::Make(ChestId id) const
{
    const ChestSpec* spec = nullptr;

    if (specs != nullptr && specCount > 0)
    {
        for (size_t i = 0; i < specCount; ++i)
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

    Chest chest(*spec);

    const ItemFactory& itemFactory = GetItemFactory();
    for (const ChestLootEntry& entry : spec->lootTable)
    {
        if (entry.probability <= 0)
        {
            continue;
        }

        if (entry.probability >= 1.0f)
        {
            chest.AddItem(itemFactory.Make(entry.itemId, entry.count));
        }
    }

    return chest;
}

namespace
{
    const ChestFactory gDefaultChestFactory{ gChestSpecs, gChestSpecCount };
}

const ChestFactory& GetChestFactory()
{
    return gDefaultChestFactory;
}


