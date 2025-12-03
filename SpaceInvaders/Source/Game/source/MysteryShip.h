#pragma once
#include "GameObject.h"
#include <tge/sprite/sprite.h>

class MysteryShip : public GameObject
{
public:
	MysteryShip(float aSpeed, int aHealth, int aScore, CommonUtilities::Vector2<float> aStartPos, Tga::SpriteSharedData* sprite);

	void Update(float aDeltaTime) override;
	void OnCollision(GameObject* other) override;
	void BuildRenderData(RenderWorld& out) const override;

	bool IsDead() const;
	int GetScoreValue() const;

private:
	float mySpeed = 0.0f;
	int myHealth = 1;
	int myScore = 0;
	Tga::SpriteSharedData* mySprite = nullptr;
};
