#include "Projectile.h"

#include "GameTypes.h"
#include "RenderTypes.h"
#include <tge/graphics/TextureResource.h>

Projectile::Projectile(const ProjectileType& aType, const CommonUtilities::Vector2<float>& aStartPos)
	: myType(&aType)
{
	myPosition = aStartPos;
}

void Projectile::Update(float aDeltaTime)
{
	if (!myType)
	{
		return;
	}

	// Flip direction: downwards should decrease Y, upwards should increase Y
	const float dir = myType->goesDown ? -1.0f : 1.0f;
	myPosition.y += dir * myType->speed * aDeltaTime;
}

void Projectile::OnCollision(GameObject*)
{
	myDestroy = true;
	Kill();
}

void Projectile::BuildRenderData(RenderWorld& out) const
{
	if (!myType || !myType->spriteData)
	{
		return;
	}

	RenderSprite sprite{};
	sprite.shared = myType->spriteData;
	sprite.instance.myPosition = { myPosition.x, myPosition.y };
	sprite.instance.myPivot = { 0.5f, 0.5f };
	sprite.instance.mySize = myType->spriteData->myTexture->CalculateTextureSize();
	sprite.instance.myColor = Tga::Color(1, 1, 1, 1);
	out.sprites.push_back(sprite);
}

bool Projectile::IsPlayerShot() const
{
	return myType ? !myType->goesDown : false;
}

bool Projectile::ShouldDestroy() const
{
	return myDestroy;
}
