#include "EnemyTypes.h"

#include <assert.h>

const EnemySpec gEnemySpecs[] =
{
    {
        EnemyId::Goblin,
        "Goblin",
        5,
        10,
        15,
        {
            { ItemId::ShortSword, 1, 0.5f },
            { ItemId::HealthPotion, 1, 0.3f }
        }
    },
    {
        EnemyId::Orc,
        "Orc",
        10,
        8,
        20,
        {
            { ItemId::BattleAxe, 1, 0.4f },
            { ItemId::HealthPotion, 1, 0.4f },
            { ItemId::LeatherArmor, 1, 0.2f }
        }
    },
    {
        EnemyId::Troll,
        "Troll",
        15,
        5,
        25,
        {
            { ItemId::LongBow, 1, 0.3f },
            { ItemId::HealthPotion, 2, 0.5f },
            { ItemId::ChainmailArmor, 1, 0.3f },
            { ItemId::FuryEnchant, 1, 0.1f }
        }
    }
};

const size_t gEnemySpecCount = sizeof(gEnemySpecs) / sizeof(gEnemySpecs[0]);

const EnemySpec* FindEnemySpec(EnemyId id)
{
    for (size_t i = 0; i < gEnemySpecCount; ++i)
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
