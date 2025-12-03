#include "CollisionSystem.h"

#include "EnemyFormation.h"
#include "ProjectileManager.h"
#include "ShieldManager.h"
#include "Player.h"
#include "MysteryShip.h"
#include <cmath>

namespace
{
	bool Intersects(const CommonUtilities::Vector2<float>& a, const CommonUtilities::Vector2<float>& b, float radius)
	{
		const float dx = a.x - b.x;
		const float dy = a.y - b.y;
		return (dx * dx + dy * dy) <= (radius * radius);
	}
}

void CollisionSystem::HandleCollisions(GameWorld& aWorld)
{
	auto& projectiles = aWorld.GetProjectileManager().GetProjectiles();
	const float projectileRadius = 12.0f;
	const float enemyRadius = 20.0f;
	const float shieldRadius = 18.0f;
	const float mysteryRadius = 22.0f;

	Player& player = aWorld.GetPlayer();

	// Player shots vs enemies, mystery ship and shields
	for (const auto& proj : projectiles)
	{
		if (!proj->IsAlive())
		{
			continue;
		}

		if (proj->IsPlayerShot())
		{
			// Mystery ship
			if (auto* ms = aWorld.GetMysteryShip())
			{
				if (Intersects(proj->GetPosition(), ms->GetPosition(), projectileRadius + mysteryRadius))
				{
					ms->OnCollision(proj.get());
					proj->OnCollision(ms);
					if (ms->IsDead())
					{
						aWorld.AddScore(ms->GetScoreValue());
					}
					continue;
				}
			}

			for (const auto& enemy : aWorld.GetEnemyFormation().GetEnemies())
			{
				if (!enemy->IsAlive())
				{
					continue;
				}
				if (Intersects(proj->GetPosition(), enemy->GetPosition(), projectileRadius + enemyRadius))
				{
					enemy->Hit();
					proj->OnCollision(enemy.get());
					if (enemy->IsDead())
					{
						aWorld.AddScore(enemy->GetScoreValue());
						aWorld.RegisterEnemyKill(enemy->GetPosition());
					}
					break;
				}
			}

			for (const auto& block : aWorld.GetShieldManager().GetBlocks())
			{
				if (!block->IsAlive())
				{
					continue;
				}
				if (Intersects(proj->GetPosition(), block->GetPosition(), projectileRadius + shieldRadius))
				{
					block->OnCollision(proj.get());
					proj->OnCollision(block.get());
					break;
				}
			}
		}
		else // enemy shot
		{
			// Enemy shots vs player
			CommonUtilities::Vector2<float> playerPos{ player.GetPosition().x, player.GetPosition().y };
			if (Intersects(proj->GetPosition(), playerPos, projectileRadius + enemyRadius))
			{
				proj->OnCollision(&player);
				player.OnCollision(proj.get());
				aWorld.OnPlayerHit();
				continue; // projectile is destroyed
			}

			// Enemy shots vs shields
			for (const auto& block : aWorld.GetShieldManager().GetBlocks())
			{
				if (!block->IsAlive())
				{
					continue;
				}
				if (Intersects(proj->GetPosition(), block->GetPosition(), projectileRadius + shieldRadius))
				{
					block->OnCollision(proj.get());
					proj->OnCollision(block.get());
					break;
				}
			}
		}
	}
}
