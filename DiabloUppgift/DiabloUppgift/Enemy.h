#pragma once
#include <string>
#include <vector>
#include "DamagableComponent.h"
#include "InventoryTypes.h"

class Enemy
{
public:
    struct LootDrop
    {
        ItemInstance item;
        int probability;
    };

    Enemy(const std::string& aName, int aStrength, int aDexterity, int aPhysique);
    const std::string& GetName() const;
    int GetStrength() const;
    int GetDexterity() const;
    int GetPhysique() const;
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDefense() const;

    const DamagableComponent& GetDamagable() const;
    void TakeDamage(int anAmount);

    void AddLoot(const ItemInstance& anItem, int aProbability);
    const std::vector<LootDrop>& GetLootDrops() const;
    void ClearLoot();

    std::string PrintStats() const;

    private:
    std::string myName;
    int myStrength;
    int myDexterity;
    int myPhysique;
    DamagableComponent myDamagable;
    std::vector<LootDrop> myLoot;
};
