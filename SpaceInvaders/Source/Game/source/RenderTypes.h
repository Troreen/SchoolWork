#pragma once
#include <vector>
#include <tge/sprite/sprite.h>

enum class EGameState;

struct RenderSprite
{
	Tga::SpriteSharedData* shared;
	Tga::Sprite2DInstanceData instance;
};

struct HUDRenderData
{
	int score = 0;
	int highScore = 0;
	int lives = 0;
	int level = 0; 
	bool isPaused = false;
	EGameState state; 
};

struct RenderWorld
{
	std::vector<RenderSprite> sprites;
	HUDRenderData hud;
};


