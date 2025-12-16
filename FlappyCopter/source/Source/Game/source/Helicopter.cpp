#include "Helicopter.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>

#include <InputHandler.h>
#include "ConfigLoader.h"

Helicopter::Helicopter()
{
	mySpriteInstance = {};
	mySharedData = {};
	myPosition = { 0.0f, 0.0f };
	myScreenResolution = { 1920.0f, 1080.0f };
	myMoveSpeed = 1200.0f;
	myInput = nullptr;
	myVelocityY = 0.0f;
	myGravity = 2000.0f;
	myJumpVelocity = 450.0f;
}

Helicopter::~Helicopter()
{
}

void Helicopter::Init(Tga::Engine& anEngine)
{
	mySharedData.myTexture = anEngine.GetTextureManager().GetTexture(("Sprites/Helicopter.png"));

	Tga::Vector2ui intResolution = anEngine.GetRenderSize();
	myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y) };

	mySpriteInstance.myPivot = { 0.5f, 0.5f };
	mySpriteInstance.mySize = { 170.0f, 80.0f };
	mySpriteInstance.myColor = Tga::Color(1, 1, 1, 1);
	
	ResetPosition();
}

void Helicopter::ApplyConfig(const GameConfig& cfg)
{
	if (cfg.playerMoveSpeed > 0.0f)
	{
		myMoveSpeed = cfg.playerMoveSpeed;
	}
	if (cfg.playerGravity > 0.0f)
	{
		myGravity = cfg.playerGravity;
	}
	if (cfg.playerJumpVelocity > 0.0f)
	{
		myJumpVelocity = cfg.playerJumpVelocity;
	}
}

void Helicopter::SetInput(InputHandler* anInput)
{
	myInput = anInput;
}

const Tga::Vector2f& Helicopter::GetPosition() const
{
	return myPosition;
}

const Tga::Vector2f& Helicopter::GetSize() const
{
	return mySpriteInstance.mySize;
}

const Tga::Vector2f& Helicopter::GetScreenResolution() const
{
	return myScreenResolution;
}

void Helicopter::ResetPosition()
{
	const float xMargin = 150.0f;
	myPosition = { xMargin, myScreenResolution.y * 0.5f };
	mySpriteInstance.myPosition = myPosition;
	myVelocityY = 0.0f;
}

void Helicopter::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
	aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}

void Helicopter::Update(float aDeltaTime)
{
	ApplyGravity(aDeltaTime);
	ClampToScreen();
	mySpriteInstance.myPosition = myPosition;
}

void Helicopter::ApplyGravity(float aDeltaTime)
{
	myVelocityY -= myGravity * aDeltaTime;
	myPosition.y += myVelocityY * aDeltaTime;

	if (myInput && myInput ->IsKeyPressed(VK_SPACE))
	{
		myVelocityY = myJumpVelocity;
	}
}

void Helicopter::ClampToScreen()
{
	const float halfH = 0.5f * mySpriteInstance.mySize.y;

	if (myPosition.y < halfH)
	{
		myPosition.y = halfH;
	}
	if (myPosition.y > myScreenResolution.y - halfH)
	{
		myPosition.y = myScreenResolution.y - halfH;
	}
}

const Tga::Vector2f& Helicopter::GetHitboxSize() const
{
    static const Tga::Vector2f hitboxSize{170.0f, 80.0f};
    return hitboxSize;
}