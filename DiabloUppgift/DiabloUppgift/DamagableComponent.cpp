#include "DamagableComponent.h"

DamagableComponent::DamagableComponent(int maxHealth, int defense)
    : myHealth(maxHealth), myMaxHealth(maxHealth), myDefense(defense)
{
}

void DamagableComponent::TakeDamage(int anIncomingDamage)
{
    int damageTaken = anIncomingDamage - myDefense;
    if (damageTaken < 0) 
    {
        damageTaken = 0;
    }

    myHealth -= damageTaken;
    if (myHealth < 0) 
    {
        myHealth = 0;
    }
}

int DamagableComponent::GetHealth() const { return myHealth; }
int DamagableComponent::GetMaxHealth() const { return myMaxHealth; }
int DamagableComponent::GetDefense() const { return myDefense; }
