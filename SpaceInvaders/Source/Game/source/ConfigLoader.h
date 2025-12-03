#pragma once
#include <string>

struct GameConfig
{
	float playerMoveSpeed;
	int		playerLives;
	float	playerProjectileSpeed;
	float	enemyProjectileSpeed;
	float	enemyBaseSpeed;
	int		defaultHighScore;
};

class ConfigLoader
{
public:
	static GameConfig Load(const std::string& aPath);
};