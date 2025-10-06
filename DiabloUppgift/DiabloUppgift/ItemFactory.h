#pragma once

#include <cstddef>

#include "InventoryTypes.h"

struct ItemFactory
{
    const ItemSpec* specs = nullptr;
    size_t specCount = 0;

    ItemInstance Make(ItemId id, int count = 1) const;
};

const ItemFactory& GetItemFactory();
