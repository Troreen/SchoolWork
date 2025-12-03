#pragma once

#include "Player.h"
#include "GameTypes.h"
#include "EnemyFormation.h"
#include "ProjectileManager.h"
#include "ShieldManager.h"
#include "HighScoreManager.h"
#include "ConfigLoader.h"
#include "Explosion.h"
#include "GameObjectFactory.h"
#include <Timer.h>
#include <InputHandler.h>
#include <memory>
#include <vector>

namespace Tga { class Text; }
class MysteryShip;

enum class EGameState
{
	Title,
	Playing,
	GameOver,
	HighScoreEntry,
	Exiting
};

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Init();
	void Update(float aTimeDelta);
	void Render();

	void SetInputHandler(InputHandler* anInput);

	Player& GetPlayer();
	const Player& GetPlayer() const;

	EnemyFormation& GetEnemyFormation();
	ProjectileManager& GetProjectileManager();
	ShieldManager& GetShieldManager();
	HighScoreManager& GetHighScoreManager();

	int GetScore() const;
	void AddScore(int aScore);
	void RegisterEnemyKill(const CommonUtilities::Vector2<float>& pos);
	void OnPlayerHit();

	EGameState GetState() const;
	void SetState(EGameState aState);
	int GetLives() const;

	// New: expose mystery ship for collision system
	MysteryShip* GetMysteryShip() { return myMysteryShip.get(); }

private:
	Player myPlayer;
	GameTypes myTypes;
	std::unique_ptr<GameObjectFactory> myFactory;
	EnemyFormation myEnemyFormation;
	ProjectileManager myProjectileManager;
	ShieldManager myShieldManager;
	HighScoreManager myHighScoreManager;
	std::vector<std::unique_ptr<Explosion>> myExplosions;
	std::unique_ptr<MysteryShip> myMysteryShip;
	float myMysterySpawnTimer = 0.0f;
	float myMysterySpawnIntervalMin = 12.0f;
	float myMysterySpawnIntervalMax = 24.0f;
	GameConfig myConfig{};

	int myPlayerScore = 0;
	int myHighScore = 20000;
	bool myIsGameOver = false;
	int myEnemiesKilled = 0;
	int myLives = 3;
	float myEnemyFireTimer = 0.0f;
	float myEnemyFireInterval = 2.0f;

	std::unique_ptr<Tga::Text> myPlayerScoreText;
	std::unique_ptr<Tga::Text> myHighScoreText;
	std::unique_ptr<Tga::Text> myGameOverText;
	std::unique_ptr<Tga::Text> myTitleText;
	std::unique_ptr<Tga::Text> mySubtitleText;
	std::unique_ptr<Tga::Text> myInstructionsText;
	std::unique_ptr<Tga::Text> myStatsText;

	CommonUtilities::Timer myTimer;
	InputHandler* myInput = nullptr;

	EGameState myState = EGameState::Title;
	bool myIsPaused = false;
	int myCurrentLevel = 1;

	void UpdateScoreText();
	void CheckGameOver();
	void ResetGame();
	void UpdatePlaying(float aDeltaTime);
	void RenderHUD();
	void RenderTitleScreen();
	void RenderGameOverScreen();
};
