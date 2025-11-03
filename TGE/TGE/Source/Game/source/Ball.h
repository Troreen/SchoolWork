#pragma once
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>

class Ball
{
public:
	Ball();
	~Ball();

	void Init(Tga::Engine& anEngine);
	void Update(float aDeltaTime);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);

private:
	Tga::Sprite2DInstanceData mySpriteInstance = {};
	Tga::SpriteSharedData mySharedData = {};

	Tga::Vector2f myPosition;
	Tga::Vector2f mySpeed;
	Tga::Vector2f myScreenResolution;
};
