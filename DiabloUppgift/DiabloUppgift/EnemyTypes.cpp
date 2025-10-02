#include "EnemyTypes.h"
#include "Enemy.h"
#include "InventoryTypes.h"

#include <assert.h>

const EnemySpec gEnemySpecs[] = 
{
    {
        EnemyId::Goblin,
        "Goblin",
        5,  // strength
        10, // dexterity
        15, // physique
        {
            { ItemId::ShortSword, 1, 0.5 },
            { ItemId::HealthPotion, 1, 0.3 }
        }
    },
    {
        EnemyId::Orc,
        "Orc",
        10, // strength
        8,  // dexterity
        20, // physique
        {
            { ItemId::BattleAxe, 1, 0.4 },
            { ItemId::HealthPotion, 1, 0.4 },
            { ItemId::LeatherArmor, 1, 0.2 }
        }
    },
    {
        EnemyId::Troll,
        "Troll",
        15, // strength
        5,  // dexterity
        25, // physique
        {
            { ItemId::LongBow, 1, 0.3 },
            { ItemId::HealthPotion, 2, 0.5 },
            { ItemId::ChainmailArmor, 1, 0.3 },
            { ItemId::FuryEnchant, 1, 0.1 }
        }
    }
};

const size_t gEnemySpecCount = sizeof(gEnemySpecs) / sizeof(gEnemySpecs[0]);


const EnemySpec* FindEnemySpec(EnemyId id)
{
    for (size_t i = 0; i < gEnemySpecCount; i++)
    {
        if (gEnemySpecs[i].id == id)
        {
            return &gEnemySpecs[i];
        }
    }
    
    return nullptr;
}

const EnemySpec& GetEnemySpec(EnemyId id)
{
    const EnemySpec* spec = FindEnemySpec(id);
    assert(spec && "GetEnemySpec called with unknown EnemyId");

    if (!spec)
    {
        return gEnemySpecs[0];
    }

    return *spec;
    
}

Enemy EnemyFactory::Make(EnemyId id) const
{
    const EnemySpec* spec = nullptr;

    if (specs != nullptr && specCount > 0)
    {
        for (size_t i = 0; i < specCount; i++)
        {
            if (specs[i].id == id)
            {
                spec = &specs[i];
                break;
            }   
        }
    }
    
    if (!spec)
    {
        spec = FindEnemySpec(id);
    }

    assert(spec && "EnemyFactory::Make called with unknown EnemyId");
    
    if (!spec)
    {
        return Enemy{};
    }
    
    Enemy enemy(spec->name, spec->strength, spec->dexterity, spec->physique);

    const ItemFactory& itemFactory = GetItemFactory();
    for (const EnemyLootEntry& entry : spec->lootTable)
    {
        enemy.AddLoot(itemFactory.Make(entry.itemId, entry.count), entry.probability);
    }

    return enemy;
}

namespace
{
    const EnemyFactory gDefaultEnemyFactory = { gEnemySpecs, gEnemySpecCount };
}

const EnemyFactory& GetEnemyFactory()
{
    return gDefaultEnemyFactory;
}