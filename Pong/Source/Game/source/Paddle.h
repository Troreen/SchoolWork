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

	void SetInput(InputHandler* aInput);
	void SetAiFollowY(float aTargetY);
	void SetIsAI(bool aIsAI);

	const Tga::Vector2f& GetPosition() const;
	const Tga::Vector2f& GetSize() const;

	void ResetPosition();

private:
	void MovePlayer(float aDeltaTime);
	void MoveAI(float aDeltaTime);
	void ClampToScreen();

	Tga::Sprite2DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySharedData;

	Tga::Vector2f myPosition;
	Tga::Vector2f myScreenResolution;
	float myMoveSpeed;
	PaddleSide mySide;
	bool myIsAI;
	float myAiTargetY;
	InputHandler* myInput;
};