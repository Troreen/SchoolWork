#pragma once

#include "Player.h"
#include <Timer.h>
#include <InputHandler.h>
#include <memory>

namespace Tga { class Text; }

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
private:
	Player myPlayer;

	int myPlayerScore = 0;
	int myHighScore = 20000;
	bool myIsGameOver = false;

	std::unique_ptr<Tga::Text> myPlayerScoreText;
	std::unique_ptr<Tga::Text> myHighScoreText;
	std::unique_ptr<Tga::Text> myGameOverText;

	CommonUtilities::Timer myTimer;
	InputHandler* myInput = nullptr;

	EGameState myState = EGameState::Title;
	bool myIsPaused = false;
	int myCurrentLevel = 1;

	void UpdateScoreText();
	void CheckGameOver();
	void ResetGame();
};