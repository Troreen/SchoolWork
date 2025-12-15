#include "GameObject.h"

const CommonUtilities::Vector2<float>& GameObject::GetPosition() const
{
	return myPosition;
}

void GameObject::SetPosition(const CommonUtilities::Vector2<float>& aPosition)
{
	myPosition = aPosition;
}

bool GameObject::IsAlive() const
{
	return myIsAlive;
}

void GameObject::Kill()
{
	myIsAlive = false;
}