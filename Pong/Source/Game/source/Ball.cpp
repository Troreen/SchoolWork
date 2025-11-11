#include "Ball.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>
#include <cmath>

Ball::Ball()
{
	mySpeed = { myBaseSpeed, myBaseSpeed };
	myPosition = { 0.0f,0.0f };
	myScreenResolution = { 1920.0f,1080.0f };
}

Ball::~Ball() = default;

void Ball::Init(Tga::Engine& anEngine)
{
	mySharedData.myTexture = anEngine.GetTextureManager().GetTexture(("Sprites/ball.dds"));

	Tga::Vector2ui intResolution = anEngine.GetRenderSize();
	myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y) };
	Reset();
	mySpriteInstance.myPivot = { 0.5f, 0.5f };
	if (mySharedData.myTexture)
		mySpriteInstance.mySize = mySharedData.myTexture->CalculateTextureSize();
	else
		mySpriteInstance.mySize = { 16.0f,16.0f };
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

	float radius = GetRadius();
	if (myPosition.y > (myScreenResolution.y - radius))
	{
		myPosition.y = myScreenResolution.y - radius;
		mySpeed.y = -std::abs(mySpeed.y);
	}
	else if (myPosition.y < radius)
	{
		myPosition.y = radius;
		mySpeed.y = std::abs(mySpeed.y);
	}

	mySpriteInstance.myPosition = myPosition;
}

void Ball::ReflectX()
{
	mySpeed.x = -mySpeed.x;
}

void Ball::AddVerticalInfluence(float /*aNormalizedOffset*/)
{
	// Intentionally disabled for fixed angle behavior
}

void Ball::Reset()
{
	myPosition = Tga::Vector2f{ 0.5f, 0.5f } * myScreenResolution;
	int dirX = (rand() % 2) ? 1 : -1;
	int dirY = (rand() % 2) ? 1 : -1;
	// Normalize components so total speed magnitude remains myBaseSpeed
	const float diagComponent = myBaseSpeed / std::sqrt(2.0f);
	mySpeed = { diagComponent * dirX, diagComponent * dirY };
	mySpriteInstance.myPosition = myPosition;
}

void Ball::SetPosition(const Tga::Vector2f& aPos)
{
	myPosition = aPos;
	mySpriteInstance.myPosition = myPosition;
}