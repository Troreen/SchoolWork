#pragma once
#include "GameObject.h"
#include "GameTypes.h"
#include <Vector2.hpp>

class Explosion : public GameObject
{
public:
	Explosion(const ExplosionType& aType, CommonUtilities::Vector2<float> aStartPos);

	void Update(float aDeltaTime) override;
	void OnCollision(GameObject* other) override;
	void BuildRenderData(RenderWorld& out) const override;

	bool IsFinished() const;

private:
	const ExplosionType* myType = nullptr;
	float myTimeLeft = 0.0f;
	int myCurrentFrame = 0;
};
