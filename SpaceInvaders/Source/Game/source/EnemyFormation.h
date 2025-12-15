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
	void OnEnemyKilled(); 
	void ResetForNewWave(int aLevel);
	void RemoveDead();

	const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const;

private:
	std::vector<std::unique_ptr<Enemy>> myEnemies;
	float myStepTimer = 0.0f;
	float myStepInterval = 0.3f;
	int myDirection = 1; 
	int myStepIndex = 0;
	int myColumns = 5;
	int myRows = 3;
	float myStepDistance = 24.0f;
	CommonUtilities::Vector2<float> myOrigin{ 0.0f, 0.0f };
	float myHorizontalMin = 40.0f;
	float myHorizontalMax = 1560.0f;
	float myDropDistance = 25.0f;
};
