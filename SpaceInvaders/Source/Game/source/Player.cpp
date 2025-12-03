#include "Player.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>
#include <InputHandler.h>
#include "RenderTypes.h"

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
	GameObject::SetPosition({ myPosition.x, myPosition.y });
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
	GameObject::SetPosition({ myPosition.x, myPosition.y });
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
	const float bottomMargin = 800.0f;
	myPosition = { myScreenResolution.x * 0.5f, myScreenResolution.y - (mySpriteInstance.mySize.y * 0.5f) - bottomMargin};
	GameObject::SetPosition({ myPosition.x, myPosition.y });
}

void Player::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}

void Player::Move(float aDeltaTime)
{
	if (!myInput)
		return;

	// Snappy movement: immediate velocity set, no acceleration/glide
	if (myInput->IsKeyDown(VK_LEFT))
	{
		myVelocityX = -myMoveSpeed;
	}
	else if (myInput->IsKeyDown(VK_RIGHT))
	{
		myVelocityX = myMoveSpeed;
	}
	else
	{
		myVelocityX = 0.0f;
	}

	myPosition.x += myVelocityX * aDeltaTime;
}

void Player::OnCollision(GameObject*)
{
	// Life handling is managed by GameWorld; player stays active here.
}

void Player::BuildRenderData(RenderWorld& out) const
{
	RenderSprite sprite{};
    sprite.shared = const_cast<Tga::SpriteSharedData*>(&mySharedData);
	sprite.instance = mySpriteInstance;
	sprite.instance.myPosition = { myPosition.x, myPosition.y };
	out.sprites.push_back(sprite);
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

