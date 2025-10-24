#pragma once

#include <vector>
#include <cstddef>
#include "InventoryTypes.h"

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

class EnemyType
{
public:
    EnemyType();
    ~EnemyType();

    // Setters
    void SetId(EnemyId anId);
    void SetName(const char* aName); 
    void SetStrength(int aStrength);
    void SetDexterity(int aDexterity);
    void SetPhysique(int aPhysique);
    void AddLoot(ItemId anItemId, int aCount, float aProbability);

    // Getters
    EnemyId GetId() const;
    const char* GetName() const;
    int GetStrength() const;
    int GetDexterity() const;
    int GetPhysique() const;
    const std::vector<EnemyLootEntry>& GetLootTable() const;

private:
    EnemyId myId;
    const char* myName;
    int myStrength;
    int myDexterity;
    int myPhysique;
    std::vector<EnemyLootEntry> myLootTable;
};

