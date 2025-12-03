#include "ShieldBlock.h"

#include "GameTypes.h"
#include "RenderTypes.h"
#include <algorithm>
#include <tge/graphics/TextureResource.h>

ShieldBlock::ShieldBlock(const ShieldType& aType, CommonUtilities::Vector2<float> aStartPos)
{
	myType = &aType;
	myPosition = aStartPos;
}

void ShieldBlock::Update(float)
{
	// Static block; no per-frame logic.
}

void ShieldBlock::OnCollision(GameObject*)
{
	Hit();
}

void ShieldBlock::BuildRenderData(RenderWorld& out) const
{
	if (!myType || myHits >= myType->maxHits)
	{
		return;
	}

	const int spriteIndex = std::min(myHits, 3);
	Tga::SpriteSharedData* spriteData = myType->damageSprites[spriteIndex];
	if (!spriteData)
	{
		return;
	}

	RenderSprite sprite{};
	sprite.shared = spriteData;
	sprite.instance.myPosition = { myPosition.x, myPosition.y };
	sprite.instance.myPivot = { 0.5f, 0.5f };
	sprite.instance.mySize = spriteData->myTexture->CalculateTextureSize();
	sprite.instance.myColor = Tga::Color(1, 1, 1, 1);
	out.sprites.push_back(sprite);
}

void ShieldBlock::Hit()
{
	if (!myIsAlive)
	{
		return;
	}

	++myHits;
	if (myHits >= (myType ? myType->maxHits : 1))
	{
		Kill();
	}
}

bool ShieldBlock::IsDestroyed() const
{
	return !myIsAlive;
}
