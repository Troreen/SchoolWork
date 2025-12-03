#pragma once
#include "GameObject.h"
#include "GameTypes.h"

class ShieldBlock : public GameObject
{
public:
	ShieldBlock(const ShieldType& aType, CommonUtilities::Vector2<float> aStartPos);

	void Update(float aDeltaTime) override;
	void OnCollision(GameObject* other) override;
	void BuildRenderData(RenderWorld& out) const override;

	void Hit();
	bool IsDestroyed() const;

private:
	const ShieldType* myType = nullptr;
	int myHits = 0;
};
