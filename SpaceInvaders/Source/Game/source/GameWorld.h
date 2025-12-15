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
#include "ScoreManager.h"
#include <Timer.h>
#include <InputHandler.h>
#include <memory>
#include <vector>

namespace Tga { class Text; }
namespace CommonUtilities { class InputHandler; }
class MysteryShip;

enum class EGameState
{
	Title,
	Playing,
	GameOver,
	LevelCleared,
	HighScoreEntry,
	Exiting,
	Paused 
};

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Init();
	void Update(float aTimeDelta);
	void Render();

	void SetInputHandler(CommonUtilities::InputHandler* anInput);

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

	MysteryShip* GetMysteryShip() { return myMysteryShip.get(); }

	CommonUtilities::Timer& GetTimer() { return myTimer; } 

private:
	Player myPlayer;
	GameTypes myTypes;
	std::unique_ptr<GameObjectFactory> myFactory;
	EnemyFormation myEnemyFormation;
	ProjectileManager myProjectileManager;
	ShieldManager myShieldManager;
	HighScoreManager myHighScoreManager;
	ScoreManager myScoreManager;
	std::vector<std::unique_ptr<Explosion>> myExplosions;
	std::unique_ptr<MysteryShip> myMysteryShip;
	float myMysterySpawnTimer = 0.0f;
	float myMysterySpawnIntervalMin = 12.0f;
	float myMysterySpawnIntervalMax = 24.0f;
	GameConfig myConfig{};

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
	CommonUtilities::InputHandler* myInput = nullptr;

	EGameState myState = EGameState::Title;
	bool myIsPaused = false;
	int myCurrentLevel = 1;

	std::vector<EnemyType> myLeveledTypes; 

	void UpdateScoreText();
	void CheckGameOver();
	void ResetGame();
	void UpdatePlaying(float aDeltaTime);
	void RenderHUD();
	void RenderTitleScreen();
	void RenderGameOverScreen();
	void RenderLevelClearedScreen();
	void RenderPauseScreen(); 
};
