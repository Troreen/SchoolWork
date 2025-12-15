#pragma once
#include "RenderTypes.h"
#include <Vector2.hpp> 

class GameObject
{
public:
	virtual ~GameObject() = default;
	
	virtual void Update(float aDeltaTime) = 0;
	virtual void OnCollision(GameObject* other) = 0;
	virtual void BuildRenderData(RenderWorld& out) const = 0;

	const CommonUtilities::Vector2<float>& GetPosition() const;
	void SetPosition(const CommonUtilities::Vector2<float>& aPosition);

	bool IsAlive() const;
	void Kill();

protected:
	CommonUtilities::Vector2<float> myPosition;
	bool myIsAlive = true;
};
