#include "IGameState.h"

IGameState::IGameState(GameContext& aContext, const std::string& aDebugName)
    : myContext(aContext)
    , myDebugName(aDebugName)
    , myStateTimer()
{
}

IGameState::~IGameState() = default;

void IGameState::OnEnter()
{
    myStateTimer.Reset();
}

void IGameState::OnExit()
{
}

double IGameState::GetElapsedSeconds() const
{
    return myStateTimer.GetElapsedSeconds();
}

const std::string& IGameState::GetDebugName() const
{
    return myDebugName;
}
