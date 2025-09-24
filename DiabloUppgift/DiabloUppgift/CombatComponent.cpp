#include "CombatComponent.h"

CombatComponent::CombatComponent(Player& player, std::vector<Enemy>& enemies)
	: player(player), enemies(enemies),currentEnemyIndex(1), result(Result::ONGOING)
{
	if (enemies.empty())
	{
		result = Result::PLAYER_WON;
	}
}

CombatComponent::Result CombatComponent::GetResult() const
{
	return result;
}

const Enemy* CombatComponent::GetCurrentEnemy() const
{
	if (enemies.empty() || currentEnemyIndex >= enemies.size())
	{
		return nullptr;
	}
	return &enemies[currentEnemyIndex];
}

void CombatComponent::PerformPlayerAction(Action action)
{
	if (result != Result::ONGOING || enemies.empty())
	{
		return;
	}

	Enemy& enemy = enemies[currentEnemyIndex];

	switch (action)
	{
	case Action::ATTACK:
		enemy.TakeDamage(player.GetStrength() * 2); // TODO: Implement dmg calc with weapons
		if (enemy.GetDamagable().GetHealth() <= 0)
		{
			enemies.erase(enemies.begin() + currentEnemyIndex);
			if (enemies.empty())
			{
				result = Result::PLAYER_WON;
				return;
			}
		}
		break;

	case Action::DEFEND:
		// Implement defend logic in future
		break;
	case Action::USE_ITEM:
		// Implement item usage logic in future
		break;
	}
}
	void CombatComponent::PerformEnemyTurn()
	{
		if (result != Result::ONGOING || enemies.empty())
		{
			return;
		}

		Enemy& enemy = enemies[currentEnemyIndex];
		player.TakeDamage(enemy.GetStrength() * 2); // TODO: Implement dmg calc with weapons

		if (player.GetDamagable().GetHealth() <= 0)
		{
			result = Result::PLAYER_LOST;
		}


	}

	void CombatComponent::CheckCombatEnd()
	{
		if (player.GetDamagable().GetHealth() <= 0)
		{
			result = Result::PLAYER_LOST;
		}
		else if (enemies.empty())
		{
			result = Result::PLAYER_WON;
		}
	}
