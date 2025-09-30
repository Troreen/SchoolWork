#pragma once
#include "Player.h"
#include "Enemy.h"
#include <vector>

class CombatComponent
{
public:
    enum class Action
    {
        ActionAttack,
        ActionDefend,
        ActionUseItem,
        ActionCount
    };

    enum class Result
    {
        ResultOngoing,
        ResultPlayerWon,
        ResultPlayerLost,
        ResultCount
    };

    CombatComponent(Player& aPlayer, std::vector<Enemy>& someEnemies);

    Result GetResult() const;
    const Enemy* GetCurrentEnemy() const;
    const std::vector<Enemy>& GetEnemies() const;
    bool SelectEnemy(size_t anIndex);
    bool PerformPlayerAction(Action anAction);
    void PerformEnemyTurn();

private:
    Player& myPlayer;
    std::vector<Enemy>& myEnemies;
    size_t myCurrentEnemyIndex;
    Result myResult;

    void CheckCombatEnd();
};
