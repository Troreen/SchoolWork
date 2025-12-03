#pragma once
#include <tge/Engine.h>


namespace Tga
{
	struct SpriteSharedData;
}

struct GameConfig;

struct PlayerType
{
	float moveSpeed;
	int	startLives;
	Tga::SpriteSharedData* spriteData = nullptr;
};

struct EnemyType
{
	const char* id;
	int	health;
	int score;
	float baseSpeed;
	float baseFireRate;
	Tga::SpriteSharedData* spriteData = nullptr;
};

struct ProjectileType
{
	float speed;
	bool goesDown;
	Tga::SpriteSharedData* spriteData = nullptr;
};

struct ShieldType
{
	int maxHits;
	Tga::SpriteSharedData* damageSprites[4] = { nullptr, nullptr, nullptr, nullptr };
};

struct ExplosionType
{
	float duration;
	Tga::SpriteSharedData* frames[2] = { nullptr, nullptr };

};

struct MysteryShipType
{
	float speed;
	int health;
	int score;
	Tga::SpriteSharedData* spriteData = nullptr;
};

// Central Registry for all types
struct GameTypes
{
	PlayerType player;
	ProjectileType playerShot;
	ProjectileType enemyShot;
	ShieldType shield;
	ExplosionType explosion;
	EnemyType enemyTypes[3];
	MysteryShipType mystery;
};

class GameTypesInitializer
{
public:
	static void Init(GameTypes& types, Tga::Engine& engine, const GameConfig& cfg);
};









