#pragma once
#include "Player.h"
#include "Enemy.h"
#include <vector>

class Room;

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

    CombatComponent() = delete;
    CombatComponent(Player& aPlayer, std::vector<Enemy>& someEnemies, Room& aRoom);
    ~CombatComponent();

    Result GetResult() const;
    const Enemy* GetCurrentEnemy() const;
    const std::vector<Enemy>& GetEnemies() const;
    bool SelectEnemy(size_t anIndex);
    bool PerformPlayerAction(Action anAction);
    void PerformEnemyTurn();

private:
    Player& myPlayer;
    std::vector<Enemy>& myEnemies;
    Room& myRoom;
    size_t myCurrentEnemyIndex;
    Result myResult;

    void CheckCombatEnd();
};
