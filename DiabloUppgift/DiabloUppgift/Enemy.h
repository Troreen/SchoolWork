#pragma once
#include <string>
#include "DamagableComponent.h"

class Enemy
{
public:
    Enemy(const std::string& aName, int aStrength, int aDexterity, int aPhysique);
    const std::string& GetName() const;
    int GetStrength() const;
    int GetDexterity() const;
    int GetBody() const;
    const DamagableComponent& GetDamagable() const;
    std::string PrintStats() const;
    void TakeDamage(int anAmount);

private:
    std::string myName;
    int myStrength;
    int myDexterity;
    int myBody;
    DamagableComponent myDamagable;
};
