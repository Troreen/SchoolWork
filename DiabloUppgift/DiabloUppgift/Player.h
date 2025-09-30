#pragma once
#include <string>
#include "DamagableComponent.h"
#include "InventoryTypes.h"

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

    InventoryState& Inventory();
    const InventoryState& Inventory() const;
    EquipmentState& Equipment();
    const EquipmentState& Equipment() const;

    int GetAttackBonusFromEquipment() const;
    int GetDefenseBonusFromEquipment() const;
    bool AddItem(const ItemInstance& anItem);
    bool UseHealthPotion();


    void TakeDamage(int anAmount);
    const DamagableComponent& GetDamagable() const;
    
    std::string PrintStats() const;

private:
    std::string myName;
    int myStrength;
    int myDexterity;
    int myPhysique;
    DamagableComponent myDamagable;

    InventoryState myInventory;
    EquipmentState myEquipment;
};
