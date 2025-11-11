#pragma once
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>

class InputHandler;

enum class PaddleSide
{
	Left,
	Right
};

class Paddle
{
public:
	Paddle();
	~Paddle();

	void Init(Tga::Engine& anEngine, PaddleSide aSide);
	void Update(float aDeltaTime);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);

	void SetInput(InputHandler* aInput) { myInput = aInput; }
	void SetAiFollowY(float aTargetY) { myAiTargetY = aTargetY; }
	void SetIsAI(bool aIsAI) { myIsAI = aIsAI; }

	const Tga::Vector2f& GetPosition() const { return myPosition; }
	const Tga::Vector2f& GetSize() const { return mySpriteInstance.mySize; }

	void ResetPosition();

private:
	void MovePlayer(float aDeltaTime);
	void MoveAI(float aDeltaTime);
	void ClampToScreen();

	Tga::Sprite2DInstanceData mySpriteInstance = {};
	Tga::SpriteSharedData mySharedData = {};

	Tga::Vector2f myPosition;
	Tga::Vector2f myScreenResolution;
	float myMoveSpeed = 1200.0f; // increased paddle speed (pixels per second)
	PaddleSide mySide = PaddleSide::Left;
	bool myIsAI = false;
	float myAiTargetY = 0.0f;
	InputHandler* myInput = nullptr;
};