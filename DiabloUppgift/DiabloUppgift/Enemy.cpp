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

Enemy::Enemy(const std::string& someName, int someStrength, int someDexterity, int somePhysique)
    : myType(nullptr)
    , myDamagable(somePhysique * 3, someDexterity)
    , myLoot()
    , myCustomName(someName)
    , myCustomStrength(someStrength)
    , myCustomDexterity(someDexterity)
    , myCustomPhysique(somePhysique)
{
}

Enemy::Enemy(const EnemyType& aType)
    : myType(&aType)
    , myDamagable(aType.GetPhysique() * 3, aType.GetDexterity())
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
    if (myType && myType->GetName())
    {
        return myType->GetName();
    }

    return myCustomName;
}

int Enemy::GetStrength() const
{
    if (myType)
    {
        return myType->GetStrength();
    }

    return myCustomStrength;
}

int Enemy::GetDexterity() const
{
    if (myType)
    {
        return myType->GetDexterity();
    }

    return myCustomDexterity;
}

int Enemy::GetPhysique() const
{
    if (myType)
    {
        return myType->GetPhysique();
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

EnemyId Enemy::GetTypeId() const
{
    return myType ? myType->GetId() : EnemyId::Goblin;
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
