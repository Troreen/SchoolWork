#pragma once

#include <vector>
#include <cstddef>
#include "InventoryTypes.h"

class Chest;

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

struct ChestFactory 
{
    const ChestSpec* specs = nullptr;
    size_t specCount = 0;

    Chest Make(ChestId id) const;
};

const ChestFactory& GetChestFactory();

extern const ChestSpec gChestSpecs[];
extern const size_t gChestSpecCount;

const ChestSpec* FindChestSpec(ChestId id);
const ChestSpec& GetChestSpec(ChestId id);



