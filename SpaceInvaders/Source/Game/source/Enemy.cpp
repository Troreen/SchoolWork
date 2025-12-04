#include "Enemy.h"

#include "GameTypes.h"
#include "RenderTypes.h"
#include <tge/graphics/TextureResource.h>

Enemy::Enemy(const EnemyType& aType, CommonUtilities::Vector2<float> aStartPos)
{
	myType = &aType;
	myHealth = aType.health;
	myPosition = aStartPos;
}

void Enemy::Update(float)
{
}

void Enemy::OnCollision(GameObject*)
{
	Hit();
}

void Enemy::BuildRenderData(RenderWorld& out) const
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

int Enemy::GetScoreValue() const
{
	return myType ? myType->score : 0;
}

bool Enemy::IsDead() const
{
	return myHealth <= 0;
}

void Enemy::Hit()
{
	if (!myIsAlive)
	{
		return;
	}

	myHealth -= 1;
	if (myHealth <= 0)
	{
		Kill();
	}
}
