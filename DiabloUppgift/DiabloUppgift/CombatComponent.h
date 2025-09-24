#pragma once
#include "Player.h"
#include "Enemy.h"
#include <vector>

class CombatComponent
{
public:
	enum class Action
	{
		ATTACK,
		DEFEND,
		USE_ITEM,
	};
	enum class Result
	{
		ONGOING,
		PLAYER_WON,
		PLAYER_LOST
	};

	CombatComponent(Player& player, std::vector<Enemy>& enemies);

	Result GetResult() const;
	const Enemy* GetCurrentEnemy() const;
	void PerformPlayerAction(Action action);
	void PerformEnemyTurn();

private:
	Player& player;
	std::vector<Enemy>& enemies;
	size_t currentEnemyIndex = 0;
	Result result;
	void CheckCombatEnd();
};