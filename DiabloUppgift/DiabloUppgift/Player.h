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
    int GetAttackBonusFromEnchantments() const;
    int GetDefenseBonusFromEnchantments() const;
    bool HasActiveEnchantments() const;
    std::vector<ActiveEnchantment>& ActiveEnchantments();
    const std::vector<ActiveEnchantment>& ActiveEnchantments() const;
    void AddActiveEnchantment(const ActiveEnchantment& anEnchantment);
    std::string GetActiveEnchantmentsSummary() const;
    void AdvanceActiveEnchantments();
    void RecalculateDerivedStats();
    float GetCarryWeight() const;
    float GetRemainingCarryWeight() const;
    float GetMaxCarryWeight() const;
    bool CanCarryAdditionalWeight(float aWeight) const;
    bool CanCarryItem(const ItemInstance& anItem) const;
    float ComputeItemWeight(const ItemInstance& anItem) const;
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
    std::vector<ActiveEnchantment> myActiveEnchantments;

    int GetStrengthBonusFromEquipment() const;
    int GetStrengthBonusFromEnchantments() const;
    int GetDexterityBonusFromEquipment() const;
    int GetDexterityBonusFromEnchantments() const;
    int GetPhysiqueBonusFromEquipment() const;
    int GetPhysiqueBonusFromEnchantments() const;
    float CalculateCarryWeight() const;
    float GetItemWeight(const ItemInstance& anItem) const;
};
