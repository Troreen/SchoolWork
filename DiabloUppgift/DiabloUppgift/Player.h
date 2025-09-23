#pragma once
#include <string>
#include "DamagableComponent.h"

class Player
{
public:
    Player(const std::string& name, int strength, int dexterity, int body);
    const std::string& GetName() const;
    int GetStrength() const;
    int GetDexterity() const;
    int GetBody() const;
    const DamagableComponent& GetDamagable() const;
    std::string PrintStats() const;
    void TakeDamage(int amount);

private:
    std::string myName;
    int myStrength;
    int myDexterity;
    int myBody;
    DamagableComponent myDamagable;
};
