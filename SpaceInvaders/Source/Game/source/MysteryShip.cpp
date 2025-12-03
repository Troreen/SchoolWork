#include "MysteryShip.h"
#include "RenderTypes.h"
#include <tge/graphics/TextureResource.h>

MysteryShip::MysteryShip(float aSpeed, int aHealth, int aScore, CommonUtilities::Vector2<float> aStartPos, Tga::SpriteSharedData* sprite)
{
	mySpeed = aSpeed;
	myHealth = aHealth;
	myScore = aScore;
	myPosition = aStartPos;
	mySprite = sprite;
}

void MysteryShip::Update(float aDeltaTime)
{
	myPosition.x += mySpeed * aDeltaTime;
}

void MysteryShip::OnCollision(GameObject*)
{
	if (--myHealth <= 0)
	{
		Kill();
	}
}

void MysteryShip::BuildRenderData(RenderWorld& out) const
{
	if (!mySprite)
		return;
	RenderSprite sprite{};
	sprite.shared = mySprite;
	sprite.instance.myPosition = { myPosition.x, myPosition.y };
	sprite.instance.myPivot = { 0.5f, 0.5f };
	sprite.instance.mySize = mySprite->myTexture->CalculateTextureSize();
	sprite.instance.myColor = Tga::Color(1, 0, 0, 1);
	out.sprites.push_back(sprite);
}

bool MysteryShip::IsDead() const
{
	return !myIsAlive;
}

int MysteryShip::GetScoreValue() const
{
	return myScore;
}
