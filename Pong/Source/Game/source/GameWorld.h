#pragma once
#include "Ball.h"
#include <Timer.h>
#include <InputHandler.h>
#include "Paddle.h"
#include <memory>

namespace Tga { class Text; }

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
	Ball* myBall;
	Paddle* myPlayerPaddle;
	Paddle* myAIPaddle;

	int myPlayerScore = 0;
	int myAIScore = 0;
	const int myMaxScore = 3;
	bool myIsGameOver = false;

	std::unique_ptr<Tga::Text> myPlayerScoreText;
	std::unique_ptr<Tga::Text> myAIScoreText;
	std::unique_ptr<Tga::Text> myGameOverText;

	CommonUtilities::Timer myTimer;
	InputHandler* myInput = nullptr;

	void HandlePaddleBallCollision();
	void UpdateScoreText();
	void CheckGameOver();
	void ResetGame();
};