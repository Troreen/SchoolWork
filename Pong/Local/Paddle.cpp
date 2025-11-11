#include "Paddle.h"

#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/SpriteDrawer.h>

Paddle::Paddle(PaddleSide aSide)
    : mySide(aSide)
{
    myMoveSpeed = 650.0f;
    myPosition = { 0.0f,0.0f };
    myScreenResolution = { 1920.0f,1080.0f };
}

Paddle::~Paddle() = default;

void Paddle::Init(Tga::Engine& anEngine)
{
    mySharedData.myTexture = anEngine.GetTextureManager().GetTexture(("Sprites/paddle.dds"));

    Tga::Vector2ui intResolution = anEngine.GetRenderSize();
    myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y) };

    mySpriteInstance.myPivot = { 0.5f, 0.5f };

    mySpriteInstance.mySize = mySharedData.myTexture ? mySharedData.myTexture->CalculateTextureSize() : Tga::Vector2f{ 20.0f, 120.0f };

    mySpriteInstance.myColor = Tga::Color(1, 1, 1, 1);

    float xOffset = 60.0f;
    if (mySide == PaddleSide::Left)
    {
        myPosition = { xOffset, myScreenResolution.y * 0.5f };
    }
    else
    {
        myPosition = { myScreenResolution.x - xOffset, myScreenResolution.y * 0.5f };
    }
    mySpriteInstance.myPosition = myPosition;
}

void Paddle::Render(Tga::SpriteDrawer& aSpriteDrawer)
{
    aSpriteDrawer.Draw(mySharedData, mySpriteInstance);
}

void Paddle::Update(float aDeltaTime, int aDirection)
{
    myPosition.y += static_cast<float>(aDirection) * myMoveSpeed * aDeltaTime;

    float halfHeight = mySpriteInstance.mySize.y * 0.5f;
    if (myPosition.y < halfHeight)
        myPosition.y = halfHeight;
    else if (myPosition.y > myScreenResolution.y - halfHeight)
        myPosition.y = myScreenResolution.y - halfHeight;

    mySpriteInstance.myPosition = myPosition;
}