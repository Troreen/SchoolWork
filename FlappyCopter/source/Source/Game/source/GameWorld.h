#pragma once

#include <memory>
#include <vector>

#include <tge/sprite/sprite.h>

#include "Helicopter.h"
#include "Obstacle.h"
#include <InputHandler.h>

namespace Tga
{
	class Text;
}

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(float aTimeDelta);
	void Render();

	void SetInputHandler(InputHandler* anInput);
	void ResetGame();

private:
	enum class GameState
	{
		Title,
		Playing,
		GameOver
	};
	
	void StartGame();
	void UpdateScoreText();
	void SpawnObstacle(float aSpeed);
	void TriggerGameOver();

	std::unique_ptr<Helicopter> myHelicopter;
	InputHandler* myInput = nullptr;

	int myPlayerScore;
	GameState myState;

	float myDistanceTravelled;
	float myDistanceSinceLastObstacle;

	std::vector<Obstacle> myObstacles;

	std::unique_ptr<Tga::Text> myPlayerScoreText;
	std::unique_ptr<Tga::Text> myGameOverText;

	std::vector<std::unique_ptr<Tga::Text>> myTitleTexts;
	struct TitleSprite
	{
		Tga::SpriteSharedData shared;
		Tga::Sprite2DInstanceData inst;
	};
	std::vector<TitleSprite> myTitleImages;

	std::vector<std::unique_ptr<Tga::Text>> myTitleStateTexts;
	std::vector<std::unique_ptr<Tga::Text>> myPlayingStateTexts;
	std::vector<std::unique_ptr<Tga::Text>> myGameOverStateTexts;
};