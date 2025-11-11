#pragma once
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>
#include <tge/engine.h>

enum class PaddleSide { Left, Right };

class Paddle
{
public:
	Paddle(PaddleSide aSide = PaddleSide::Left);
	~Paddle();

	void Init(Tga::Engine& anEngine);
	// direction: -1 up, 1 down, 0 idle
	void Update(float aDeltaTime, int aDirection);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);

	const Tga::Vector2f& GetPosition() const { return myPosition; }
	const Tga::Vector2f& GetSize() const { return mySpriteInstance.mySize; }

private:
	PaddleSide mySide;
	Tga::Sprite2DInstanceData mySpriteInstance = {};
	Tga::SpriteSharedData mySharedData = {};

	Tga::Vector2f myPosition;
	float myMoveSpeed;
	Tga::Vector2f myScreenResolution;
};