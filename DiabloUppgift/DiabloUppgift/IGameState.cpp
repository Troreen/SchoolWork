#include "IGameState.h"

IGameState::IGameState(GameContext& aContext)
    : myContext(aContext)
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
