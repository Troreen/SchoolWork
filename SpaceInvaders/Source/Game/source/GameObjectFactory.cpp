#include "GameObjectFactory.h"

#include "Player.h"
#include "Enemy.h"
#include "MysteryShip.h"
#include "ShieldBlock.h"
#include "Projectile.h"
#include "Explosion.h"

GameObjectFactory::GameObjectFactory(const GameTypes& someTypes)
	: myTypes(someTypes)
{}

std::unique_ptr<Player> GameObjectFactory::CreatePlayer()
{
	auto player = std::make_unique<Player>();

	return player;
}

std::unique_ptr<Enemy> GameObjectFactory::CreateEnemy(const EnemyType& aType, const CUVector2f& pos)
{
	return std::make_unique<Enemy>(aType, pos);
}

std::unique_ptr<MysteryShip> GameObjectFactory::CreateMysteryShip(const CUVector2f& pos)
{
	const float speed = myTypes.mystery.speed;
	const int health = myTypes.mystery.health;
	const int score = myTypes.mystery.score;
	Tga::SpriteSharedData* sprite = myTypes.mystery.spriteData;
	return std::make_unique<MysteryShip>(speed, health, score, pos, sprite);
}

std::unique_ptr<ShieldBlock> GameObjectFactory::CreateShieldBlock(const CUVector2f& pos)
{
	return std::make_unique<ShieldBlock>(myTypes.shield, pos);
}

std::unique_ptr<Projectile> GameObjectFactory::CreateProjectile(const ProjectileType& aType, const CUVector2f& pos)
{
	return std::make_unique<Projectile>(aType, pos);
}

std::unique_ptr<Explosion> GameObjectFactory::CreateExplosion(const ExplosionType& aType, const CUVector2f& pos)
{
	return std::make_unique<Explosion>(aType, pos);
}
