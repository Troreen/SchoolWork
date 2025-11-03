#pragma once
#include "Ball.h"

// CommonUtilities
#include <Timer.h>
#include <InputHandler.h>

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(float aTimeDelta);
	void Render();

	inline void SetInputHandler(InputHandler* anInput);
private:
	Ball* myBall;

	CommonUtilities::Timer myTimer;

	InputHandler* myInput;
};