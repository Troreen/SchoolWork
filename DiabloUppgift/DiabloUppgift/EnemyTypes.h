#pragma once

#include <vector>
#include <cstddef>
#include "InventoryTypes.h"

class Enemy;

enum class EnemyId
{
    Goblin,
    Orc,
    Troll
};

struct EnemyLootEntry
{
    ItemId itemId;
    int count;
    float probability; 
};

struct EnemySpec
{
    EnemyId id;
    const char* name;
    int strength;
    int dexterity;
    int physique;
    std::vector<EnemyLootEntry> lootTable;
};

struct EnemyFactory 
{
    const EnemySpec* specs = nullptr;
    size_t specCount = 0;

    Enemy Make(EnemyId id) const;   
};

const EnemyFactory& GetEnemyFactory();

extern const EnemySpec gEnemySpecs[];
extern const size_t gEnemySpecCount;

const EnemySpec* FindEnemySpec(EnemyId id);
const EnemySpec& GetEnemySpec(EnemyId id);