#include "CombatComponent.h"
#include <iostream>

CombatComponent::CombatComponent(Player& aPlayer, std::vector<Enemy>& someEnemies)
    : myPlayer(aPlayer),
      myEnemies(someEnemies),
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

void CombatComponent::PerformPlayerAction(Action anAction)
{
    std::system("cls"); 
    if (myResult != Result::ResultOngoing || myEnemies.empty() || myCurrentEnemyIndex >= myEnemies.size())
    {
        return;
    }

    Enemy& enemy = myEnemies[myCurrentEnemyIndex];

    switch (anAction)
    {
    case Action::ActionAttack:
    {
        const std::string enemyName = enemy.GetName();
        const int enemyMaxHealth = enemy.GetMaxHealth();
        const int enemyHealthBefore = enemy.GetHealth();
        const int attackPower = myPlayer.GetStrength() * 2;
        enemy.TakeDamage(attackPower);
        const int enemyHealthAfter = enemy.GetHealth();
        const int damageDealt = enemyHealthBefore - enemyHealthAfter;
        std::cout << "You attack " << enemyName << " and deal " << damageDealt << " damage.\n";

        if (enemyHealthAfter <= 0)
        {
            std::cout << "This kills the " << enemyName << ".\n";
            myEnemies.erase(myEnemies.begin() + static_cast<int>(myCurrentEnemyIndex));
            if (myEnemies.empty())
            {
                myResult = Result::ResultPlayerWon;
                return;
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
        break;
    case Action::ActionUseItem:
        std::cout << "You search for an item, but nothing happens yet.\n";
        break;
    case Action::ActionCount:
        break;
    }

    CheckCombatEnd();
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
