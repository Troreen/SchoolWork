#include "Ball.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>

Ball::Ball()
{
	mySpeed = { 500.0f, 500.0f };
	myPosition = { 0.0f,0.0f };
	myScreenResolution = { 1920.0f,1080.0f };
}

Ball::~Ball()
{
}

void Ball::Init(Tga::Engine& anEngine)
{
	// Load Sprite
	mySharedData.myTexture = anEngine.GetTextureManager().GetTexture(("Sprites/ball.dds"));

	// Center the ball
	Tga::Vector2ui intResolution = anEngine.GetRenderSize();
	myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y) };
	myPosition = Tga::Vector2f{ 0.5f, 0.5f } *myScreenResolution;
	mySpriteInstance.myPosition = myPosition;

	// Set pivot
	mySpriteInstance.myPivot = { 0.5f, 0.5f };

	// Change size of sprite
	mySpriteInstance.mySize = mySharedData.myTexture->CalculateTextureSize();

	// Change color (Red | Green | Blue| Alpha)
	mySpriteInstance.myColor = Tga::Color(1, 1, 1, 1);
}

void Ball::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}

void Ball::Update(float aDeltaTime)
{
	myPosition.x += mySpeed.x * aDeltaTime;
	myPosition.y += mySpeed.y * aDeltaTime;

	if (myPosition.x > (myScreenResolution.x - 10.0f))
	{
		mySpeed.x = -500.0f;
	}
	else if (myPosition.x < 10)
	{
		mySpeed.x = 500.0f;
	}

	if (myPosition.y > (myScreenResolution.y - 10.0f))
	{
		mySpeed.y = -500.0f;
	}
	else if (myPosition.y < 10)
	{
		mySpeed.y = 500.0f;
	}

	mySpriteInstance.myPosition = myPosition;

}