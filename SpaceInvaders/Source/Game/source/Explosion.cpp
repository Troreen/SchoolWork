#include "Explosion.h"

#include "GameTypes.h"
#include "RenderTypes.h"
#include <tge/graphics/TextureResource.h>

Explosion::Explosion(const ExplosionType& aType, CommonUtilities::Vector2<float> aStartPos)
{
	myType = &aType;
	myPosition = aStartPos;
	myTimeLeft = aType.duration;
	myCurrentFrame = 0;
}

void Explosion::Update(float aDeltaTime)
{
	if (myTimeLeft <= 0.0f)
	{
		Kill();
		return;
	}

	myTimeLeft -= aDeltaTime;

	// 3-frame animation: frame 0 for first third, 1 for second third, 2 for final third
	if (myType && myType->duration > 0.0f)
	{
		float oneThird = myType->duration / 3.0f;
		if (myTimeLeft <= oneThird * 2.0f && myTimeLeft > oneThird)
		{
			myCurrentFrame = 1;
		}
		else if (myTimeLeft <= oneThird)
		{
			myCurrentFrame = 2;
		}
		else
		{
			myCurrentFrame = 0;
		}
	}

	if (myTimeLeft <= 0.0f)
	{
		Kill();
	}
}

void Explosion::OnCollision(GameObject*)
{
	// Explosions do not interact with collisions.
}

void Explosion::BuildRenderData(RenderWorld& out) const
{
	if (!myType)
	{
		return;
	}

	// Clamp frame index to valid range
	int frameIndex = myCurrentFrame;
	if (frameIndex < 0) frameIndex = 0;
	if (frameIndex > 2) frameIndex = 2;

	Tga::SpriteSharedData* frame = myType->frames[frameIndex];
	if (!frame)
	{
		return;
	}

	RenderSprite sprite{};
	sprite.shared = frame;
	sprite.instance.myPosition = { myPosition.x, myPosition.y };
	sprite.instance.myPivot = { 0.5f, 0.5f };
	sprite.instance.mySize = frame->myTexture->CalculateTextureSize();
	sprite.instance.myColor = Tga::Color(1, 1, 1, 1);
	out.sprites.push_back(sprite);
}

bool Explosion::IsFinished() const
{
	return myTimeLeft <= 0.0f;
}
