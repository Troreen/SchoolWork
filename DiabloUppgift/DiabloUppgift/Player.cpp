#include "Player.h"

Player::Player(const std::string& aName, int aStrength, int aDexterity, int aPhysique)
    : myName(aName),
      myStrength(aStrength),
      myDexterity(aDexterity),
      myBody(aPhysique),
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

int Player::GetBody() const
{
    return myBody;
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
    stats += "PHY: " + std::to_string(myBody) + "\n";
    stats += "DEF: " + std::to_string(myDamagable.GetDefense()) + "\n";
    stats += "HP: " + std::to_string(myDamagable.GetHealth()) + "/" + std::to_string(myDamagable.GetMaxHealth()) + "\n";
    return stats;
}

void Player::TakeDamage(int anAmount)
{
    myDamagable.TakeDamage(anAmount);
}
