#pragma once

#include "IGameState.h"

class MainMenuState : public IGameState
{
public:
    explicit MainMenuState(GameContext& aContext);

    void Update(StateStack& aStateStack) override;
};
