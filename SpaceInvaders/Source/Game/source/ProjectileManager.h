#pragma once
#include <Vector2.hpp>
#include <vector>
#include <memory>
#include "Projectile.h"


class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();
	
	void Update(float aDeltaTime);
	void SpawnPlayerShot(const CommonUtilities::Vector2<float>& aPosition, const ProjectileType& aType);
	void SpawnEnemyShot(const CommonUtilities::Vector2<float>& aPosition, const ProjectileType& aType);

	const std::vector<std::unique_ptr<Projectile>>& GetProjectiles() const;

	bool CanPlayerShoot() const;
private:
	std::vector<std::unique_ptr<Projectile>> myProjectiles;
	int myActivePlayerShots = 0;
	float myReloadTimer = 0.0f;
	float myReloadTime = 0.3f;

};