#pragma once

#include <string>

#include "GameContext.h"
#include "StateTimer.h"

class StateStack;

class IGameState
{
public:
    IGameState(GameContext& aContext, const std::string& aDebugName);
    virtual ~IGameState();

    virtual void OnEnter();
    virtual void OnExit();
    virtual void Update(StateStack& aStateStack) = 0;

    double GetElapsedSeconds() const;
    const std::string& GetDebugName() const;

protected:
    GameContext& myContext;

private:
    std::string myDebugName;
    StateTimer myStateTimer;
};
