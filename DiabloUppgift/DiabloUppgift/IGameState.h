#pragma once

#include "GameContext.h"
#include "StateTimer.h"

class StateStack;

class IGameState
{
public:
    explicit IGameState(GameContext& aContext);
    virtual ~IGameState();

    virtual void OnEnter();
    virtual void OnExit();
    virtual void Update(StateStack& aStateStack) = 0;

    double GetElapsedSeconds() const;

protected:
    GameContext& myContext;

private:
    StateTimer myStateTimer;
};
