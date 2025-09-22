#include "Player.h"

Player::Player(const std::string& name, int strength, int dexterity, int body)
    : myName(name), myStrength(strength), myDexterity(dexterity), myBody(body),
      myDamagable(body * 4 + strength * 6 + dexterity * 3, strength * 2 + body)
{
}

const std::string& Player::GetName() const { return myName; }
int Player::GetStrength() const { return myStrength; }
int Player::GetDexterity() const { return myDexterity; }
int Player::GetBody() const { return myBody; }
const DamagableComponent& Player::GetDamagable() const { return myDamagable; }

std::string Player::PrintStats() const
{
	std::string stats = "Name: " + myName + "\n";
	stats += "Strength: " + std::to_string(myStrength) + "\n";
	stats += "Dexterity: " + std::to_string(myDexterity) + "\n";
	stats += "Body: " + std::to_string(myBody) + "\n";
	stats += "Defense: " + std::to_string(myDamagable.GetDefense()) + "\n";
	stats += "Health: " + std::to_string(myDamagable.GetHealth()) + "/" + std::to_string(myDamagable.GetMaxHealth()) + "\n";
	return stats;
}

void Player::TakeDamage(int amount)
{
	myDamagable.TakeDamage(amount);
}
