#pragma once
#include <string>
#include <vector>

struct TextElement
{
	std::string text;
	float x = 0.0f;
	float y = 0.0f;
	std::string state;
};

struct TitleText
{
	std::string text;
	float x = 0.0f;
	float y = 0.0f;
};

struct TitleImage
{
	std::string path;
	float x = 0.0f;
	float y = 0.0f;
};

struct GameConfig
{
	float playerMoveSpeed = 1200.0f;
	float playerGravity = 2000.0f;
	float playerJumpVelocity = 450.0f;

	float scrollSpeedScale = 1.0f;

	std::string playerSprite;
	std::string obstacleSprite;
	std::string backgroundSprite;

	std::vector <TextElement> textElements;
	std::vector <TitleText> titleTexts;
	std::vector <TitleImage> titleImages;

};
class ConfigLoader
{
public:
		
	static GameConfig Load(const std::string& aPath);
};