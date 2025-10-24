#include "EnemyFactory.h"

#include "Enemy.h"
#include "ItemFactory.h"

#include <assert.h>

namespace
{
    constexpr size_t ToIndex(EnemyId anId)
    {
        return static_cast<size_t>(anId);
    }
}

EnemyFactory::EnemyFactory() = default;
EnemyFactory::~EnemyFactory() = default;

void EnemyFactory::Initialize()
{
    if (myInitialized)
    {
        return;
    }

    // Goblin
    {
        EnemyType& t = myTypes[ToIndex(EnemyId::Goblin)];
        t.SetId(EnemyId::Goblin);
        t.SetName("Goblin");
        t.SetStrength(5);
        t.SetDexterity(10);
        t.SetPhysique(15);
        t.AddLoot(ItemId::ShortSword, 1, 0.5f);
        t.AddLoot(ItemId::HealthPotion, 1, 0.3f);
    }

    // Orc
    {
        EnemyType& t = myTypes[ToIndex(EnemyId::Orc)];
        t.SetId(EnemyId::Orc);
        t.SetName("Orc");
        t.SetStrength(10);
        t.SetDexterity(8);
        t.SetPhysique(20);
        t.AddLoot(ItemId::BattleAxe, 1, 0.4f);
        t.AddLoot(ItemId::HealthPotion, 1, 0.4f);
        t.AddLoot(ItemId::LeatherArmor, 1, 0.2f);
    }

    // Troll
    {
        EnemyType& t = myTypes[ToIndex(EnemyId::Troll)];
        t.SetId(EnemyId::Troll);
        t.SetName("Troll");
        t.SetStrength(15);
        t.SetDexterity(5);
        t.SetPhysique(25);
        t.AddLoot(ItemId::LongBow, 1, 0.3f);
        t.AddLoot(ItemId::HealthPotion, 2, 0.5f);
        t.AddLoot(ItemId::ChainmailArmor, 1, 0.3f);
        t.AddLoot(ItemId::FuryEnchant, 1, 0.1f);
    }

    myInitialized = true;
}

const EnemyType& EnemyFactory::GetType(EnemyId anId) const
{
    const size_t index = ToIndex(anId);
    assert(index < myTypes.size());
    return myTypes[index];
}

Enemy EnemyFactory::Make(EnemyId anId) const
{
    assert(myInitialized && "EnemyFactory used before Initialize()");

    const EnemyType& type = GetType(anId);

    Enemy enemy(type);

    const ItemFactory& itemFactory = GetItemFactory();
    for (const EnemyLootEntry& entry : type.GetLootTable())
    {
        enemy.AddLoot(itemFactory.Make(entry.itemId, entry.count), entry.probability);
    }

    return enemy;
}

namespace
{
    EnemyFactory& GetMutableFactory()
    {
        static EnemyFactory factory;
        return factory;
    }
}

const EnemyFactory& GetEnemyFactory()
{
    EnemyFactory& factory = GetMutableFactory();
    factory.Initialize();
    return factory;
}

