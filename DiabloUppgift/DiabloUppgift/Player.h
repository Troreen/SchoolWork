#pragma once
#include <string>
#include "DamagableComponent.h"

class Player
{
public:
    Player(const std::string& aName, int aStrength, int aDexterity, int aPhysique);
    const std::string& GetName() const;
    int GetStrength() const;
    int GetDexterity() const;
    int GetPhysique() const;
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDefense() const;

    void TakeDamage(int anAmount);
    const DamagableComponent& GetDamagable() const;
    
    std::string PrintStats() const;

private:
    std::string myName;
    int myStrength;
    int myDexterity;
    int myPhysique;
    DamagableComponent myDamagable;
};
