#pragma once
#include "GameObject.h"
#include "GameTypes.h"

class Enemy : public GameObject
{
public:
	Enemy(const EnemyType& aType, CommonUtilities::Vector2<float> aStartPos);

	void Update(float aDeltaTime) override;
	void OnCollision(GameObject* other) override;
	void BuildRenderData(RenderWorld& out) const override;

	int GetScoreValue() const;
	bool IsDead() const;
	void Hit();

private:
	const EnemyType* myType = nullptr;
	int myHealth = 1;
};
