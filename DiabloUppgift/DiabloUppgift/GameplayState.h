#pragma once

#include "IGameState.h"
#include "InteractionController.h"

class GameplayState : public IGameState
{
public:
    explicit GameplayState(GameContext& aContext);

    void Update(StateStack& aStateStack) override;

private:
    InteractionController myInteraction;
};
