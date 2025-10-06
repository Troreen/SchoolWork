#include "ItemFactory.h"

#include <assert.h>

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
