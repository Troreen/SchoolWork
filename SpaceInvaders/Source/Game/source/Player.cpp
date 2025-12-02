#include "Player.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>
#include <InputHandler.h>

Player::Player()
	: myMoveSpeed(600.0f)
	, myVelocityX(0.0f)
	, myAcceleration(2000.0f)
	, myInput(nullptr)
{
}

Player::~Player()
{
}

void Player::Init(Tga::Engine& anEngine)
{
	mySharedData.myTexture = anEngine.GetTextureManager().GetTexture(("Sprites/Player.dds"));

	Tga::Vector2ui intResolution = anEngine.GetRenderSize();
	myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y) };

	mySpriteInstance.myPivot = { 0.5f, 0.5f };
	mySpriteInstance.mySize = mySharedData.myTexture->CalculateTextureSize();
	mySpriteInstance.myColor = Tga::Color(1, 1, 1, 1);

	ResetPosition();
}

void Player::SetInput(InputHandler* aInput)
{
	myInput = aInput;
}

void Player::Update(float aDeltaTime)
{
	Move(aDeltaTime);
	ClampToScreen();
	mySpriteInstance.myPosition = myPosition;
}

const Tga::Vector2f& Player::GetPosition() const
{
	return myPosition;
}

const Tga::Vector2f& Player::GetSize() const
{
	return mySpriteInstance.mySize;
}

const Tga::Vector2f& Player::GetScreenResolution() const
{
	return myScreenResolution;
}

void Player::ResetPosition()
{
	const float bottomMargin = 10.0f;
	myPosition = { myScreenResolution.x * 0.5f, myScreenResolution.y - (mySpriteInstance.mySize.y * 0.5f) - bottomMargin};
}

void Player::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}

void Player::Move(float aDeltaTime)
{
	if (!myInput)
		return;

	float targetAcceleration = 0.0f;

	if (myInput->IsKeyDown(VK_LEFT))
	{
		targetAcceleration = -myAcceleration;
	}
	else if (myInput->IsKeyDown(VK_RIGHT))
	{
		targetAcceleration = myAcceleration;
	}
	else
	{
		if (myVelocityX > 0.0f) targetAcceleration = -myAcceleration;
		else if (myVelocityX < 0.0f) targetAcceleration = myAcceleration;
	}

	myVelocityX += targetAcceleration * aDeltaTime;

	const float maxSpeed = myMoveSpeed;
	if (myVelocityX > maxSpeed)  
	{
		myVelocityX = maxSpeed;
	}
	if (myVelocityX < -maxSpeed) 
	{
		myVelocityX = -maxSpeed;
	}

	myPosition.x += myVelocityX * aDeltaTime;
}


void Player::ClampToScreen()
{
	const float halfWidth = mySpriteInstance.mySize.x * 0.5f;
	if (myPosition.x - halfWidth < 0.0f)
	{
		myPosition.x = halfWidth;
		myVelocityX = 0.0f;
	}
	else if (myPosition.x + halfWidth > myScreenResolution.x)
	{
		myPosition.x = myScreenResolution.x - halfWidth;
		myVelocityX = 0.0f;
	}
}

