#include "EnemyTypes.h"

EnemyType::EnemyType()
    : myId(EnemyId::Goblin)
    , myName(nullptr)
    , myStrength(0)
    , myDexterity(0)
    , myPhysique(0)
    , myLootTable()
{
}

EnemyType::~EnemyType() = default;

void EnemyType::SetId(EnemyId anId)
{
    myId = anId;
}

void EnemyType::SetName(const char* aName)
{
    myName = aName;
}

void EnemyType::SetStrength(int aStrength)
{
    myStrength = aStrength;
}

void EnemyType::SetDexterity(int aDexterity)
{
    myDexterity = aDexterity;
}

void EnemyType::SetPhysique(int aPhysique)
{
    myPhysique = aPhysique;
}

void EnemyType::AddLoot(ItemId anItemId, int aCount, float aProbability)
{
    if (aProbability < 0.0f) aProbability = 0.0f;
    if (aProbability > 1.0f) aProbability = 1.0f;
    myLootTable.push_back({ anItemId, aCount, aProbability });
}

EnemyId EnemyType::GetId() const
{
    return myId;
}

const char* EnemyType::GetName() const
{
    return myName;
}

int EnemyType::GetStrength() const
{
    return myStrength;
}

int EnemyType::GetDexterity() const
{
    return myDexterity;
}

int EnemyType::GetPhysique() const
{
    return myPhysique;
}

const std::vector<EnemyLootEntry>& EnemyType::GetLootTable() const
{
    return myLootTable;
}
