#include "EnemyFormation.h"

#include "GameObjectFactory.h"
#include <algorithm>

EnemyFormation::EnemyFormation() = default;
EnemyFormation::~EnemyFormation() = default;

void EnemyFormation::Init(const std::vector<const EnemyType*>& someEnemyTypes, const CommonUtilities::Vector2<float>& aStartPos)
{
	myEnemies.clear();
	myOrigin = aStartPos;

	const int typesCount = static_cast<int>(someEnemyTypes.size());
	myRows = typesCount > 0 ? typesCount : 1;
	const int columns = myColumns;

	const float spacingX = 80.0f;
	const float spacingY = 40.0f;

	for (int row = 0; row < myRows; ++row)
	{
		const EnemyType* type = someEnemyTypes.empty() ? nullptr : someEnemyTypes[std::min(row, typesCount - 1)];
		for (int col = 0; col < columns; ++col)
		{
			CommonUtilities::Vector2<float> pos;
			pos.x = myOrigin.x + static_cast<float>(col) * spacingX;
			pos.y = myOrigin.y + static_cast<float>(row) * spacingY;

			if (type)
			{
				myEnemies.push_back(std::make_unique<Enemy>(*type, pos));
			}
		}
	}

	myDirection = 1;
	myStepTimer = 0.0f;
	myStepIndex = 0;
}

void EnemyFormation::Update(float aDeltaTime)
{
	if (myEnemies.empty())
	{
		return;
	}

	myStepTimer += aDeltaTime;
	if (myStepTimer < myStepInterval)
	{
		return;
	}

	myStepTimer = 0.0f;
	if (myStepIndex >= static_cast<int>(myEnemies.size()))
	{
		myStepIndex = 0;
	}

	const float step = myStepDistance * static_cast<float>(myDirection);
	for (int i = 0; i < static_cast<int>(myEnemies.size()); ++i)
	{
		const int idx = (myStepIndex + i) % static_cast<int>(myEnemies.size());
		Enemy* enemy = myEnemies[idx].get();
		if (!enemy->IsAlive())
		{
			continue;
		}

		CommonUtilities::Vector2<float> pos = enemy->GetPosition();
		pos.x += step;

		bool hitEdge = (pos.x < myHorizontalMin) || (pos.x > myHorizontalMax);
		if (hitEdge)
		{
			for (auto& e : myEnemies)
			{
				if (!e->IsAlive())
				{
					continue;
				}
				auto p = e->GetPosition();
				p.y -= myDropDistance;
				e->SetPosition(p);
			}
			myDirection *= -1;
			break;
		}
		else
		{
			enemy->SetPosition(pos);
			myStepIndex = idx + 1;
			break;
		}
	}
}

void EnemyFormation::OnEnemyKilled()
{
	if (myEnemies.empty())
	{
		return;
	}

	const int alive = static_cast<int>(std::count_if(myEnemies.begin(), myEnemies.end(),
		[](const std::unique_ptr<Enemy>& e) { return !e->IsDead(); }));
	const float progress = myColumns * myRows > 0
		? 1.0f - (static_cast<float>(alive) / static_cast<float>(myColumns * myRows))
		: 0.0f;
	const float minInterval = 0.12f;
	const float maxInterval = 0.55f;
	myStepInterval = std::max(minInterval, maxInterval - progress * (maxInterval - minInterval));
}

void EnemyFormation::RemoveDead()
{
	myEnemies.erase(std::remove_if(myEnemies.begin(), myEnemies.end(),
		[](const std::unique_ptr<Enemy>& e) { return e->IsDead(); }), myEnemies.end());
}

void EnemyFormation::ResetForNewWave(int)
{
	for (auto& enemy : myEnemies)
	{
		enemy->Kill();
	}
	myEnemies.clear();
}

const std::vector<std::unique_ptr<Enemy>>& EnemyFormation::GetEnemies() const
{
	return myEnemies;
}
