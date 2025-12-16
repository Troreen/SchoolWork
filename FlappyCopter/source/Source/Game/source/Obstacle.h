#pragma once
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>
#include "Helicopter.h"

class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	void Init(Tga::Engine& anEngine, const Tga::Vector2f& aPosition);
	void Update(float aDeltaTime);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);

	bool CheckCollision(const Helicopter& aHelicopter) const;
	bool IsOffScreen() const;

	const Tga::Vector2f& GetPosition() const;
	const Tga::Vector2f& GetSize() const;

	void SetSize(const Tga::Vector2f& aSize);
	void SetSpeed(float aSpeed);

private:
	Tga::Sprite2DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySharedData;
	Tga::Vector2f myPosition;
	Tga::Vector2f myScreenResolution;
	float mySpeed;
};
