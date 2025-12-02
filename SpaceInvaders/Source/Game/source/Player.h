#pragma once
#include <tge/graphics//GraphicsEngine.h>
#include <tge/sprite/sprite.h>
#include "GameObject.h"

class InputHandler;

class Player : public GameObject 
{
public:
	Player();
	~Player();

	void Init(Tga::Engine& anEngine);
	void Update(float aDeltaTime);
	void Render(Tga::SpriteDrawer& aSpriteDrawer);

	void SetInput(InputHandler* aInput);

	const Tga::Vector2f& GetPosition() const;
	const Tga::Vector2f& GetSize() const;
	const Tga::Vector2f& GetScreenResolution() const;

	void ResetPosition();

private:

	void Move(float aDeltaTime);
	void ClampToScreen();

	Tga::Sprite2DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySharedData;

	Tga::Vector2f myPosition;
	Tga::Vector2f myScreenResolution;
	float myMoveSpeed;
	
	float myVelocityX;
	float myAcceleration;

	InputHandler* myInput;
	

};