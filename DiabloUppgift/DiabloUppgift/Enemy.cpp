#include "Enemy.h"

Enemy::Enemy(const std::string& aName, int aStrength, int aDexterity, int aPhysique)
    : myName(aName)
    , myStrength(aStrength)
    , myDexterity(aDexterity)
    , myPhysique(aPhysique)
    , myDamagable(aPhysique * 3, aDexterity)
    , myLoot()
{
}
Enemy::Enemy()
    : myName("Unknown")
    , myStrength(0)
    , myDexterity(0)
    , myPhysique(0)
    , myDamagable(0, 0)
    , myLoot()
{
}


Enemy::~Enemy() = default;

const std::string& Enemy::GetName() const
{
    return myName;
}

int Enemy::GetStrength() const
{
    return myStrength;
}

int Enemy::GetDexterity() const
{
    return myDexterity;
}

int Enemy::GetPhysique() const
{
    return myPhysique;
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

void Enemy::AddLoot(const ItemInstance& anItem, float aProbability)
{
    if (aProbability < 0.0f)
    {
        aProbability = 0.0f;
    }
    else if (aProbability > 1.0f)
    {
        aProbability = 1.0f;
    }

    myLoot.push_back({ anItem, aProbability });
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
    return myName + " (STR: " + std::to_string(myStrength) + ", DEX: " + std::to_string(myDexterity) + ", PHY: " + std::to_string(myPhysique) + ")";
}
