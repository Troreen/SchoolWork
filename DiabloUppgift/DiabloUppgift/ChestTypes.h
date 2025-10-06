#pragma once

#include <vector>
#include <cstddef>
#include "InventoryTypes.h"

enum class ChestId
{
    WoodenChest,
    IronChest,
    GoldenChest
};

struct ChestLootEntry
{
    ItemId itemId;
    int count;
    float probability;
};

struct ChestSpec
{
    ChestId id;
    const char* name;
    const char* description;
    bool locked;
    int capacity;
    std::vector<ChestLootEntry> lootTable;
};

extern const ChestSpec gChestSpecs[];
extern const size_t gChestSpecCount;

const ChestSpec* FindChestSpec(ChestId id);
const ChestSpec& GetChestSpec(ChestId id);
