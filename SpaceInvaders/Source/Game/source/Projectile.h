#pragma once
#include "GameObject.h"
#include "GameTypes.h"

class Projectile : public GameObject
{
public:
	Projectile(const ProjectileType& aType, const CommonUtilities::Vector2<float>& aStartPos);

	void Update(float aDeltaTime) override;
	void OnCollision(GameObject* other) override;
	void BuildRenderData(RenderWorld& out) const override;

	bool IsPlayerShot() const;
	bool ShouldDestroy() const;

private:
	const ProjectileType* myType = nullptr;
	bool myDestroy = false;
};
