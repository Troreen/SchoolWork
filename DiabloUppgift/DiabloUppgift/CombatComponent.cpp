#include "CombatComponent.h"
#include "InventoryTypes.h"
#include "Room.h"
#include <iostream>
#include <random>

namespace
{
    int RollPercent()
    {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_int_distribution<int> dist(0, 99);
        return dist(rng);
    }
}

CombatComponent::CombatComponent(Player& aPlayer, std::vector<Enemy>& someEnemies, Room& aRoom)
    : myPlayer(aPlayer),
      myEnemies(someEnemies),
      myRoom(aRoom),
      myCurrentEnemyIndex(0),
      myResult(Result::ResultOngoing)
{
    if (myEnemies.empty())
    {
        myResult = Result::ResultPlayerWon;
    }
}

CombatComponent::Result CombatComponent::GetResult() const
{
    return myResult;
}

const Enemy* CombatComponent::GetCurrentEnemy() const
{
    if (myEnemies.empty() || myCurrentEnemyIndex >= myEnemies.size())
    {
        return nullptr;
    }

    return &myEnemies[myCurrentEnemyIndex];
}

const std::vector<Enemy>& CombatComponent::GetEnemies() const
{
    return myEnemies;
}

bool CombatComponent::SelectEnemy(size_t anIndex)
{
    if (myResult != Result::ResultOngoing)
    {
        return false;
    }

    if (myEnemies.empty() || anIndex >= myEnemies.size())
    {
        return false;
    }

    myCurrentEnemyIndex = anIndex;
    return true;
}

bool CombatComponent::PerformPlayerAction(Action anAction)
{
    std::system("cls"); 
    if (myResult != Result::ResultOngoing || myEnemies.empty() || myCurrentEnemyIndex >= myEnemies.size())
    {
        return false;
    }

    Enemy& enemy = myEnemies[myCurrentEnemyIndex];
    bool actionConsumed = false;

    switch (anAction)
    {
    case Action::ActionAttack:
    {
        const std::string enemyName = enemy.GetName();
        const int enemyMaxHealth = enemy.GetMaxHealth();
        const int enemyHealthBefore = enemy.GetHealth();
        const int attackPower = myPlayer.GetStrength() * 2
            + myPlayer.GetAttackBonusFromEquipment()
            + myPlayer.GetAttackBonusFromEnchantments();
        enemy.TakeDamage(attackPower);
        const int enemyHealthAfter = enemy.GetHealth();
        const int damageDealt = enemyHealthBefore - enemyHealthAfter;
        std::cout << "You attack " << enemyName << " and deal " << damageDealt << " damage.\n";

        actionConsumed = true;

        if (enemyHealthAfter <= 0)
        {
            std::cout << "This kills the " << enemyName << ".\n";

            const std::vector<Enemy::LootDrop>& lootDrops = enemy.GetLootDrops();
            bool droppedAny = false;
            for (const Enemy::LootDrop& drop : lootDrops)
            {
                if (drop.probability <= 0)
                {
                    continue;
                }

                const int roll = RollPercent();
                if (roll < drop.probability)
                {
                    ItemInstance dropped = drop.item;
                    dropped.equipped = false;
                    myRoom.AddFloorItem(dropped);

                    const ItemSpec& spec = GetItemSpec(dropped.id);
                    if (!droppedAny)
                    {
                        std::cout << "It drops: ";
                        droppedAny = true;
                    }
                    else
                    {
                        std::cout << ", ";
                    }

                    std::cout << spec.name;
                    if (spec.maxStack > 1 && dropped.count > 1)
                    {
                        std::cout << " x" << dropped.count;
                    }
                }
            }
            if (droppedAny)
            {
                std::cout << "\n";
            }

            myEnemies.erase(myEnemies.begin() + static_cast<int>(myCurrentEnemyIndex));
            if (myEnemies.empty())
            {
                myResult = Result::ResultPlayerWon;
                return true;
            }

            if (myCurrentEnemyIndex >= myEnemies.size())
            {
                myCurrentEnemyIndex = 0;
            }
        }
        else
        {
            std::cout << enemyName << " has " << enemyHealthAfter << "/" << enemyMaxHealth << " health left.\n";
        }
        break;
    }
    case Action::ActionDefend:
        std::cout << "You brace for the incoming attacks.\n";
        actionConsumed = true;
        break;
    case Action::ActionUseItem:
    {
        if (myPlayer.UseHealthPotion())
        {
            std::cout << "You drink a health potion.\n";
            std::cout << "Your health: " << myPlayer.GetHealth() << "/" << myPlayer.GetMaxHealth() << "\n";
            actionConsumed = true;
        }
        else
        {
            std::cout << "You have no health potions.\n";
            return false;
        }
        break;
    }
    case Action::ActionCount:
        return false;
    }

    if (!actionConsumed)
    {
        return false;
    }

    CheckCombatEnd();
    return true;
}

void CombatComponent::PerformEnemyTurn()
{
    if (myResult != Result::ResultOngoing || myEnemies.empty())
    {
        return;
    }

    for (Enemy& enemy : myEnemies)
    {
        const std::string enemyName = enemy.GetName();
        const int attackPower = enemy.GetStrength() * 2;
        const int playerHealthBefore = myPlayer.GetHealth();
        myPlayer.TakeDamage(attackPower);
        const int playerHealthAfter = myPlayer.GetHealth();
        const int damageTaken = playerHealthBefore - playerHealthAfter;

        if (damageTaken > 0)
        {
            std::cout << enemyName << " attacks you and deals " << damageTaken << " damage.\n";
        }
        else
        {
            std::cout << enemyName << " attacks you but fails to hurt you.\n";
        }

        std::cout << "Your health: " << playerHealthAfter << "/" << myPlayer.GetMaxHealth() << "\n";

        if (playerHealthAfter <= 0)
        {
            myResult = Result::ResultPlayerLost;
            std::cout << "You collapse from your wounds.\n";
            return;
        }
    }

    CheckCombatEnd();
}

void CombatComponent::CheckCombatEnd()
{
    if (myPlayer.GetHealth() <= 0)
    {
        myResult = Result::ResultPlayerLost;
    }
    else if (myEnemies.empty())
    {
        myResult = Result::ResultPlayerWon;
    }
}
