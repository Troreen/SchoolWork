#include "Enemy.h"

Enemy::Enemy(const std::string& name, int strength, int dexterity, int body)
	: myName(name), myStrength(strength), myDexterity(dexterity), myBody(body),
	myDamagable(body * 3, dexterity)
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
    return myName + " (STR: " + std::to_string(myStrength) + ", DEX: " + std::to_string(myDexterity) + ", BODY: " + std::to_string(myBody) + ")";
}

void Enemy::TakeDamage(int amount) 
{
    myDamagable.TakeDamage(amount);
}