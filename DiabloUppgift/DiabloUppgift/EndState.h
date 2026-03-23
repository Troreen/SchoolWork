#pragma once

#include "IGameState.h"

class EndState : public IGameState
{
public:
    explicit EndState(GameContext& aContext);

    void Update(StateStack& aStateStack) override;
};
