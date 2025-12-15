#include "ConfigLoader.h"

#include <fstream>
#include <sstream>

namespace
{
	GameConfig MakeDefaultConfig()
	{
		GameConfig cfg{};
		cfg.playerMoveSpeed = 600.0f;
		cfg.playerLives = 3;
		cfg.playerProjectileSpeed = 900.0f;
		cfg.enemyProjectileSpeed = 600.0f;
		cfg.enemyBaseSpeed = 500.0f;
		cfg.defaultHighScore = 0;
		return cfg;
	}

	bool ParseLine(const std::string& aLine, const std::string& aKey, float& aOutValue)
	{
		const auto pos = aLine.find(aKey);
		if (pos == std::string::npos)
		{
			return false;
		}

		const auto eq = aLine.find('=', pos + aKey.size());
		if (eq == std::string::npos)
		{
			return false;
		}

		try
		{
			aOutValue = std::stof(aLine.substr(eq + 1));
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	bool ParseLine(const std::string& aLine, const std::string& aKey, int& aOutValue)
	{
		const auto pos = aLine.find(aKey);
		if (pos == std::string::npos)
		{
			return false;
		}

		const auto eq = aLine.find('=', pos + aKey.size());
		if (eq == std::string::npos)
		{
			return false;
		}

		try
		{
			aOutValue = std::stoi(aLine.substr(eq + 1));
		}
		catch (...)
		{
			return false;
		}

		return true;
	}
}

GameConfig ConfigLoader::Load(const std::string& aPath)
{
	std::ifstream file(aPath);
	if (!file.is_open())
	{
		return MakeDefaultConfig();
	}

	GameConfig cfg = MakeDefaultConfig();
	std::string line;
	while (std::getline(file, line))
	{
		ParseLine(line, "player_move_speed", cfg.playerMoveSpeed);
		ParseLine(line, "player_lives", cfg.playerLives);
		ParseLine(line, "projectile_speed_player", cfg.playerProjectileSpeed);
		ParseLine(line, "projectile_speed_enemy", cfg.enemyProjectileSpeed);
		ParseLine(line, "enemy_move_speed", cfg.enemyBaseSpeed);
		ParseLine(line, "default_hiscore", cfg.defaultHighScore);
	}

	return cfg;
}
