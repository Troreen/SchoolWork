#pragma once
#include <vector>
#include <Vector2.hpp>
#include <memory>
#include "Enemy.h"

class EnemyFormation
{
public:
	EnemyFormation();
	~EnemyFormation();

	void Init(const std::vector<const EnemyType*>& someEnemyTypes, const CommonUtilities::Vector2<float>& aStartPos);
	void Update(float aDeltaTime);
	void OnEnemyKilled(); //increase speed etc.
	void ResetForNewWave(int aLevel);

	const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const;

private:
	std::vector<std::unique_ptr<Enemy>> myEnemies;
	float myStepTimer = 0.0f;
	float myStepInterval = 0.5f;
	int myDirection = 1; //1 = right, -1 = left
};