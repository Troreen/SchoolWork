#include "EndState.h"

#include <iomanip>
#include <iostream>

#include "ConsoleView.h"
#include "StateStack.h"

EndState::EndState(GameContext& aContext)
    : IGameState(aContext, "EndState")
{
}

void EndState::Update(StateStack& aStateStack)
{
    ConsoleView::ClearScreen();

    if (!myContext.endMessage.empty())
    {
        std::cout << myContext.endMessage << "\n";
    }
    else
    {
        std::cout << "Game session ended.\n";
    }

    std::cout << "End state timer: " << std::fixed << std::setprecision(1) << GetElapsedSeconds() << " s\n";
    std::cout << "Press Enter to close.\n";
    ConsoleView::Pause();

    myContext.isRunning = false;
    aStateStack.ClearStates();
}
