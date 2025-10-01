#pragma once

class DamagableComponent
{
public:
    DamagableComponent() = delete;
    DamagableComponent(int aMaxHealth, int aDefense);
    ~DamagableComponent();
    void TakeDamage(int anIncomingDamage);
    void Heal(int anAmount);
    void SetMaxHealth(int aMaxHealth);
    void SetDefense(int aDefense);
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDefense() const;

private:
    int myHealth;
    int myMaxHealth;
    int myDefense;
};
