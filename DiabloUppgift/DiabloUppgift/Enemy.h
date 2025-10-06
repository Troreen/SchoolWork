#pragma once

#include <string>
#include <vector>
#include "DamagableComponent.h"
#include "InventoryTypes.h"

struct EnemySpec;
enum class EnemyId;

class Enemy
{
public:
    struct LootDrop
    {
        ItemInstance item;
        float probability;
    };

    Enemy();
    Enemy(const std::string& aName, int aStrength, int aDexterity, int aPhysique);
    explicit Enemy(const EnemySpec& aType);
    ~Enemy();

    std::string GetName() const;
    int GetStrength() const;
    int GetDexterity() const;
    int GetPhysique() const;
    int GetHealth() const;
    int GetMaxHealth() const;
    int GetDefense() const;

    const DamagableComponent& GetDamagable() const;
    void TakeDamage(int anAmount);

    const EnemySpec& GetType() const;
    EnemyId GetTypeId() const;
    bool HasType() const;

    void AddLoot(const ItemInstance& anItem, float aProbability);
    const std::vector<LootDrop>& GetLootDrops() const;
    void ClearLoot();

    std::string PrintStats() const;

private:
    const EnemySpec* myType;
    DamagableComponent myDamagable;
    std::vector<LootDrop> myLoot;

    std::string myCustomName;
    int myCustomStrength;
    int myCustomDexterity;
    int myCustomPhysique;
};

