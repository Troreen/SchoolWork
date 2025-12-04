#include "ProjectileManager.h"

#include <algorithm>

ProjectileManager::ProjectileManager() = default;
ProjectileManager::~ProjectileManager() = default;

void ProjectileManager::Update(float aDeltaTime)
{
	myReloadTimer -= aDeltaTime;
	if (myReloadTimer < 0.0f)
	{
		myReloadTimer = 0.0f;
	}

	for (auto& projectile : myProjectiles)
	{
		if (projectile->IsAlive())
		{
			projectile->Update(aDeltaTime);
		}
	}

	const float minY = -50.0f;
	const float maxY = 950.0f;

	myProjectiles.erase(std::remove_if(myProjectiles.begin(), myProjectiles.end(),
		[&](const std::unique_ptr<Projectile>& p)
		{
			const float y = p->GetPosition().y;
			const bool outOfBounds = (y < minY) || (y > maxY);
			if (outOfBounds || p->ShouldDestroy())
			{
				if (p->IsPlayerShot())
				{
					myActivePlayerShots = std::max(0, myActivePlayerShots - 1);
				}
				return true;
			}
			return false;
		}), myProjectiles.end());
}

void ProjectileManager::SpawnPlayerShot(const CommonUtilities::Vector2<float>& aPosition, const ProjectileType& aType)
{
	if (!CanPlayerShoot())
	{
		return;
	}

	myProjectiles.push_back(std::make_unique<Projectile>(aType, aPosition));
	++myActivePlayerShots;
	myReloadTimer = myReloadTime;
}

void ProjectileManager::SpawnEnemyShot(const CommonUtilities::Vector2<float>& aPosition, const ProjectileType& aType)
{
	myProjectiles.push_back(std::make_unique<Projectile>(aType, aPosition));
}

const std::vector<std::unique_ptr<Projectile>>& ProjectileManager::GetProjectiles() const
{
	return myProjectiles;
}

bool ProjectileManager::CanPlayerShoot() const
{
	return myActivePlayerShots < 3 && myReloadTimer <= 0.0f;
}

void ProjectileManager::Reset()
{
	myProjectiles.clear();
	myActivePlayerShots = 0;
	myReloadTimer = 0.0f;
}
