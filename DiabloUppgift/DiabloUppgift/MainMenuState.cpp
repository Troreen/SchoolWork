#include "MainMenuState.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "ConsoleView.h"
#include "StateId.h"
#include "StateStack.h"

namespace
{
    int ReadInt(const std::string& aPrompt)
    {
        while (true)
        {
            std::cout << aPrompt;

            int value = 0;
            if (std::cin >> value)
            {
                std::cin.ignore(1000000, '\n');
                return value;
            }

            std::cin.clear();
            std::cin.ignore(1000000, '\n');
            std::cout << "Please enter a number.\n";
        }
    }
}

MainMenuState::MainMenuState(GameContext& aContext)
    : IGameState(aContext)
{
}

void MainMenuState::Update(StateStack& aStateStack)
{
    ConsoleView::ClearScreen();

    std::cout << "=== Diablo Ripoff Game ===\n";
    std::cout << "Main menu timer: " << std::fixed << std::setprecision(1) << GetElapsedSeconds() << " s\n\n";
    std::cout << "1 - Start game\n";
    std::cout << "0 - Exit\n\n";

    const int choice = ReadInt("Choice: ");
    if (choice == 1)
    {
        aStateStack.PopState();
        aStateStack.PushState(StateId::Gameplay);
        return;
    }

    if (choice == 0)
    {
        myContext.endMessage = "Exited from main menu after " + std::to_string(static_cast<int>(GetElapsedSeconds())) + " second(s).";
        aStateStack.PushState(StateId::End);
        return;
    }

    std::cout << "Invalid choice.\n";
    ConsoleView::Pause();
}
