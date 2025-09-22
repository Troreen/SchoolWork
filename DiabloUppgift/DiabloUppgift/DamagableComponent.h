#pragma once

class DamagableComponent
{
public:
    DamagableComponent(int maxHealth, int defense);
    void TakeDamage(int anIncomingDamage);
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDefense() const;
private:
    int myHealth;
    int myMaxHealth;
    int myDefense;
};