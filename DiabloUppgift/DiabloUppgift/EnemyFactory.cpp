#include "EnemyFactory.h"

#include "Enemy.h"
#include "ItemFactory.h"

#include <assert.h>

Enemy EnemyFactory::Make(EnemyId id) const
{
    const EnemySpec* spec = nullptr;

    if (specs != nullptr && specCount > 0)
    {
        for (size_t i = 0; i < specCount; ++i)
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
    const EnemyFactory gDefaultEnemyFactory{ gEnemySpecs, gEnemySpecCount };
}

const EnemyFactory& GetEnemyFactory()
{
    return gDefaultEnemyFactory;
}
