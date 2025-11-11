#include "Paddle.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>
#include <InputHandler.h>

Paddle::Paddle()
{
	myPosition = { 0.0f,0.0f };
	myScreenResolution = { 1920.0f,1080.0f };
}

Paddle::~Paddle() {}

void Paddle::Init(Tga::Engine& anEngine, PaddleSide aSide)
{
	mySide = aSide;
	mySharedData.myTexture = anEngine.GetTextureManager().GetTexture(("Sprites/paddle.dds"));

	Tga::Vector2ui intResolution = anEngine.GetRenderSize();
	myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y) };

	mySpriteInstance.myPivot = { 0.5f, 0.5f };
	if (mySharedData.myTexture)
		mySpriteInstance.mySize = mySharedData.myTexture->CalculateTextureSize();
	else
		mySpriteInstance.mySize = { 20.0f, 120.0f };

	mySpriteInstance.myColor = Tga::Color(1, 1, 1, 1);

	ResetPosition();
}

void Paddle::ResetPosition()
{
	const float xMargin = 40.0f;
	if (mySide == PaddleSide::Left)
	{
		myPosition = { xMargin, myScreenResolution.y * 0.5f };
	}
	else
	{
		myPosition = { myScreenResolution.x - xMargin, myScreenResolution.y * 0.5f };
	}
	mySpriteInstance.myPosition = myPosition;
}

void Paddle::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}

void Paddle::Update(float aDeltaTime)
{
	if (myIsAI) MoveAI(aDeltaTime);
	else MovePlayer(aDeltaTime);

	ClampToScreen();
	mySpriteInstance.myPosition = myPosition;
}

void Paddle::MovePlayer(float aDeltaTime)
{
	if (!myInput) return;
	float dir = 0.0f;
	// Up should move visually up; adjust mapping
	if (myInput->IsKeyDown(VK_UP)) dir += 1.0f;   
	if (myInput->IsKeyDown(VK_DOWN)) dir -= 1.0f; 
	myPosition.y += dir * myMoveSpeed * aDeltaTime;
}

void Paddle::MoveAI(float aDeltaTime)
{
	float dir = 0.0f;
	if (myAiTargetY < myPosition.y - 5.0f) dir = -1.0f;
	else if (myAiTargetY > myPosition.y + 5.0f) dir = 1.0f;
	myPosition.y += dir * myMoveSpeed * aDeltaTime;
}

void Paddle::ClampToScreen()
{
	const float halfH = 0.5f * mySpriteInstance.mySize.y;
	if (myPosition.y < halfH) myPosition.y = halfH;
	if (myPosition.y > myScreenResolution.y - halfH) myPosition.y = myScreenResolution.y - halfH;
}