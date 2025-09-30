#include "DamagableComponent.h"

DamagableComponent::DamagableComponent(int aMaxHealth, int aDefense)
    : myHealth(aMaxHealth),
      myMaxHealth(aMaxHealth),
      myDefense(aDefense)
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

void DamagableComponent::Heal(int anAmount)
{

    if (anAmount < 0)
    {
        return;
    }
    myHealth += anAmount;
    if (myHealth > myMaxHealth)
    {
        myHealth = myMaxHealth;
    }
}


int DamagableComponent::GetHealth() const
{
    return myHealth;
}

int DamagableComponent::GetMaxHealth() const
{
    return myMaxHealth;
}

int DamagableComponent::GetDefense() const
{
    return myDefense;
}
