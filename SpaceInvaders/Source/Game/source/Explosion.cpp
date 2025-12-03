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

	// Simple 2-frame animation: switch to frame 2 halfway through
	float half = (myType ? myType->duration * 0.5f : 0.0f);
	if (myTimeLeft <= half)
	{
		myCurrentFrame = 1;
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

	Tga::SpriteSharedData* frame = myType->frames[myCurrentFrame];
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
