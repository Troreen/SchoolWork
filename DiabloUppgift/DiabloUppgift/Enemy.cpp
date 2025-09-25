#include "Enemy.h"

Enemy::Enemy(const std::string& aName, int aStrength, int aDexterity, int aPhysique)
    : myName(aName),
      myStrength(aStrength),
      myDexterity(aDexterity),
      myBody(aPhysique),
      myDamagable(aPhysique * 3, aDexterity)
{
}

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

int Enemy::GetBody() const
{
    return myBody;
}

const DamagableComponent& Enemy::GetDamagable() const
{
    return myDamagable;
}

std::string Enemy::PrintStats() const
{
    return myName + " (STR: " + std::to_string(myStrength) + ", DEX: " + std::to_string(myDexterity) + ", PHY: " + std::to_string(myBody) + ")";
}

void Enemy::TakeDamage(int anAmount)
{
    myDamagable.TakeDamage(anAmount);
}
