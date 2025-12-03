#pragma once

#include <memory>
#include "GameTypes.h"

class Player;
class Enemy;
class MysteryShip;
class ShieldBlock;
class Projectile;
class Explosion;

namespace CommonUtilities { template<typename T> class Vector2; }
using CUVector2f = CommonUtilities::Vector2<float>;

class GameObjectFactory {
public: 
		GameObjectFactory(const GameTypes& someTypes);
		
		std::unique_ptr<Player> CreatePlayer();
		std::unique_ptr<Enemy> CreateEnemy(const EnemyType& aType, const CUVector2f& pos);

		std::unique_ptr<MysteryShip> CreateMysteryShip(const CUVector2f& pos);
		std::unique_ptr<ShieldBlock> CreateShieldBlock(const CUVector2f& pos);
		std::unique_ptr<Projectile> CreateProjectile(const ProjectileType& aType, const CUVector2f& pos);
		std::unique_ptr<Explosion> CreateExplosion(const ExplosionType& aType, const CUVector2f& pos);

private:
	const GameTypes& myTypes;

};
