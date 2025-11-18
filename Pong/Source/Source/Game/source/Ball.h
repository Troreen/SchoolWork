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

	const Tga::Vector2f& GetPosition() const;
	const Tga::Vector2f& GetScreenResolution() const;
	float GetRadius() const;

	void ReflectX();
	void Reset();
	void SetPosition(const Tga::Vector2f& aPos);

private:
	Tga::Sprite2DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySharedData;

	Tga::Vector2f myPosition;
	Tga::Vector2f mySpeed;
	Tga::Vector2f myScreenResolution;

	float myBaseSpeed;
};