#include "Enemy.h"
#include "EnemyTypes.h"

Enemy::Enemy()
    : myType(nullptr)
    , myDamagable(0, 0)
    , myLoot()
    , myCustomName("Unknown")
    , myCustomStrength(0)
    , myCustomDexterity(0)
    , myCustomPhysique(0)
{
}

Enemy::Enemy(const std::string& aName, int aStrength, int aDexterity, int aPhysique)
    : myType(nullptr)
    , myDamagable(aPhysique * 3, aDexterity)
    , myLoot()
    , myCustomName(aName)
    , myCustomStrength(aStrength)
    , myCustomDexterity(aDexterity)
    , myCustomPhysique(aPhysique)
{
}

Enemy::Enemy(const EnemySpec& aType)
    : myType(&aType)
    , myDamagable(aType.physique * 3, aType.dexterity)
    , myLoot()
    , myCustomName()
    , myCustomStrength(0)
    , myCustomDexterity(0)
    , myCustomPhysique(0)
{
}

Enemy::~Enemy() = default;

std::string Enemy::GetName() const
{
    if (myType && myType->name)
    {
        return myType->name;
    }

    return myCustomName;
}

int Enemy::GetStrength() const
{
    if (myType)
    {
        return myType->strength;
    }

    return myCustomStrength;
}

int Enemy::GetDexterity() const
{
    if (myType)
    {
        return myType->dexterity;
    }

    return myCustomDexterity;
}

int Enemy::GetPhysique() const
{
    if (myType)
    {
        return myType->physique;
    }

    return myCustomPhysique;
}

int Enemy::GetHealth() const
{
    return myDamagable.GetHealth();
}

int Enemy::GetMaxHealth() const
{
    return myDamagable.GetMaxHealth();
}

int Enemy::GetDefense() const
{
    return myDamagable.GetDefense();
}

const DamagableComponent& Enemy::GetDamagable() const
{
    return myDamagable;
}

void Enemy::TakeDamage(int anAmount)
{
    myDamagable.TakeDamage(anAmount);
}

const EnemySpec& Enemy::GetType() const
{
    if (myType)
    {
        return *myType;
    }

    return GetEnemySpec(EnemyId::Goblin);
}

EnemyId Enemy::GetTypeId() const
{
    return GetType().id;
}

bool Enemy::HasType() const
{
    return myType != nullptr;
}

void Enemy::AddLoot(const ItemInstance& anItem, float aProbability)
{
    float clampedProbability = aProbability;
    if (clampedProbability < 0.0f)
    {
        clampedProbability = 0.0f;
    }
    else if (clampedProbability > 1.0f)
    {
        clampedProbability = 1.0f;
    }

    myLoot.push_back({ anItem, clampedProbability });
}

const std::vector<Enemy::LootDrop>& Enemy::GetLootDrops() const
{
    return myLoot;
}

void Enemy::ClearLoot()
{
    myLoot.clear();
}

std::string Enemy::PrintStats() const
{
    return GetName() + " (STR: " + std::to_string(GetStrength()) + ", DEX: " + std::to_string(GetDexterity()) + ", PHY: " + std::to_string(GetPhysique()) + ")";
}
