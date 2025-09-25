#include "Player.h"

Player::Player(const std::string& aName, int aStrength, int aDexterity, int aPhysique)
    : myName(aName),
      myStrength(aStrength),
      myDexterity(aDexterity),
      myPhysique(aPhysique),
      myDamagable(aPhysique * 4 + aStrength * 6 + aDexterity * 3, aStrength + aPhysique)
{
}

const std::string& Player::GetName() const
{
    return myName;
}

int Player::GetStrength() const
{
    return myStrength;
}

int Player::GetDexterity() const
{
    return myDexterity;
}

int Player::GetPhysique() const
{
    return myPhysique;
}

int Player::GetHealth() const
{
    return myDamagable.GetHealth();
}

int Player::GetMaxHealth() const
{
    return myDamagable.GetMaxHealth();
}

int Player::GetDefense() const
{
    return myDamagable.GetDefense();
}

void Player::TakeDamage(int anAmount)
{
    myDamagable.TakeDamage(anAmount);
}

const DamagableComponent& Player::GetDamagable() const
{
    return myDamagable;
}

std::string Player::PrintStats() const
{
    std::string stats = "Name: " + myName + "\n";
    stats += "STR: " + std::to_string(myStrength) + "\n";
    stats += "DXT: " + std::to_string(myDexterity) + "\n";
    stats += "PHY: " + std::to_string(myPhysique) + "\n";
    stats += "DEF: " + std::to_string(GetDefense()) + "\n";
    stats += "HP: " + std::to_string(GetHealth()) + "/" + std::to_string(GetMaxHealth()) + "\n";
    return stats;
}