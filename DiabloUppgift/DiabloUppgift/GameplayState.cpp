#include "GameplayState.h"

#include <sstream>

#include "StateId.h"
#include "StateStack.h"

GameplayState::GameplayState(GameContext& aContext)
    : IGameState(aContext, "GameplayState")
    , myInteraction(aContext.player, aContext.rooms, aContext.currentRoom, aContext.doors)
{
}

void GameplayState::Update(StateStack& aStateStack)
{
    const bool keepPlaying = myInteraction.HandleTurn();
    if (keepPlaying)
    {
        return;
    }

    std::ostringstream message;
    message << "Gameplay ended after " << static_cast<int>(GetElapsedSeconds()) << " second(s).";
    myContext.endMessage = message.str();
    aStateStack.PushState(StateId::End);
}
