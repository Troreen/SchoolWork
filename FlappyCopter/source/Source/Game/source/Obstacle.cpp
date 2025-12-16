#include "Obstacle.h"
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>

Obstacle::Obstacle()
{
	mySpriteInstance = {};
	mySharedData = {};
	myPosition = { 0.0f, 0.0f };
	myScreenResolution = { 1920.0f, 1080.0f };
	mySpeed = 300.0f;
}

Obstacle::~Obstacle()
{
}
void Obstacle::Init(Tga::Engine& anEngine, const Tga::Vector2f& aPosition)
{
	mySharedData.myTexture = anEngine.GetTextureManager().GetTexture("Sprites/MetalBlock.png");
	Tga::Vector2ui intResolution = anEngine.GetRenderSize();
	myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y) };
	myPosition = aPosition;
	mySpriteInstance.myPivot = { 0.5f,0.5f };

	if (mySharedData.myTexture)
	{
		mySpriteInstance.mySize = mySharedData.myTexture->CalculateTextureSize();
	}
	else
	{
		mySpriteInstance.mySize = { 64.0f, 128.0f };
	}

	mySpriteInstance.myColor = Tga::Color(1, 1, 1, 1);
	mySpriteInstance.myPosition = myPosition;
}

void Obstacle::Update(float aDeltaTime)
{
	myPosition.x -= mySpeed * aDeltaTime;
	mySpriteInstance.myPosition = myPosition;
}

void Obstacle::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}

bool Obstacle::CheckCollision(const Helicopter& aHelicopter) const
{
	const Tga::Vector2f& heliPos = aHelicopter.GetPosition();
	const Tga::Vector2f& heliSize = aHelicopter.GetHitboxSize();
	const Tga::Vector2f& obsSize = mySpriteInstance.mySize;

	float halfW = obsSize.x * 0.5f;
	float halfH = obsSize.y * 0.5f;
	float heliHalfW = heliSize.x * 0.5f;
	float heliHalfH = heliSize.y * 0.5f;

	return !(myPosition.x + halfW < heliPos.x - heliHalfW ||
			 myPosition.x - halfW > heliPos.x + heliHalfW ||
			 myPosition.y + halfH < heliPos.y - heliHalfH ||
			 myPosition.y - halfH > heliPos.y + heliHalfH);
}

bool Obstacle::IsOffScreen() const
{
	return myPosition.x + mySpriteInstance.mySize.x * 0.5f < 0;
}

const Tga::Vector2f& Obstacle::GetPosition() const
{
	return myPosition;
}

const Tga::Vector2f& Obstacle::GetSize() const
{
	return mySpriteInstance.mySize;
}

void Obstacle::SetSize(const Tga::Vector2f& aSize)
{
	mySpriteInstance.mySize = aSize;
}

void Obstacle::SetSpeed(float aSpeed)
{
	mySpeed = aSpeed;
}