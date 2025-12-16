#pragma once
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>

class InputHandler;
struct GameConfig;

class Helicopter
{
public:
	Helicopter();
	~Helicopter();

	void Init(Tga::Engine& anEngine);
	void Update(float aDeltaTime);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);

	void SetInput(InputHandler* anInput);

	void ApplyConfig(const GameConfig& cfg);

	const Tga::Vector2f& GetPosition() const;
	const Tga::Vector2f& GetSize() const;
	const Tga::Vector2f& GetScreenResolution() const;

	const Tga::Vector2f& GetHitboxSize() const;

	void ResetPosition();

private:
	void ClampToScreen();
	void ApplyGravity(float aDeltaTiem);

	Tga::Sprite2DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySharedData;

	Tga::Vector2f myPosition;
	Tga::Vector2f myScreenResolution;
	float myMoveSpeed;

	float myVelocityY;
	float myGravity;
	float myJumpVelocity;

	InputHandler* myInput;
};