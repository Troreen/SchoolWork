#include "Casino.h"

#include <cctype>
#include <iostream>

using namespace CasinoHelpers;

Casino::Casino()
    : myGuessTheNumberTables{
          GuessTheNumberGame("Low Stakes", 10, 100),
          GuessTheNumberGame("High Stakes", 100, 500)}
    , myOddOrEvenGame()
    , myBlackJackGame()
    , mySlotMachineGame()
    , myRouletteGame()
    , myPlayerName()
    , myPlayerMoney(globalPlayerInitialMoney)
    , myPlayerBet(0)
    , myStatHistory{0}
    , myGenerator()
{
    std::random_device randomSource;
    myGenerator = std::mt19937(randomSource());
}

void Casino::Run()
{
    if (myPlayerName.empty())
    {
        myPlayerName = PromptForPlayerName();
    }

    GameState currentState = GameState::Menu;
    while (true)
    {
        switch (currentState)
        {
        case GameState::Menu:
            DrawHud(myPlayerMoney, myStatHistory, myPlayerName);
            ShowMenu(myPlayerName);
            currentState = MenuState(myPlayerMoney, myPlayerBet, myStatHistory, myPlayerName);
            break;
        case GameState::GuessTheNumber:
            currentState = PlayGuessTheNumber();
            break;
        case GameState::OddOrEven:
            currentState = myOddOrEvenGame.Play(myGenerator, myPlayerMoney, myPlayerBet, myStatHistory, myPlayerName);
            break;
        case GameState::BlackJack:
            currentState = myBlackJackGame.Play(myGenerator, myPlayerMoney, myPlayerBet, myStatHistory, myPlayerName);
            break;
        case GameState::SlotMachine:
            currentState = mySlotMachineGame.Play(myGenerator, myPlayerMoney, myPlayerBet, myStatHistory, myPlayerName);
            break;
        case GameState::Roulette:
            currentState = myRouletteGame.Play(myGenerator, myPlayerMoney, myPlayerBet, myStatHistory, myPlayerName);
            break;
        case GameState::Exit:
            std::cout << "\n" << myPlayerName << ", you are leaving with: " << myPlayerMoney << "\n";
            return;
        case GameState::Count:
        default:
            return;
        }
    }
}

CasinoHelpers::GameState Casino::PlayGuessTheNumber()
{
    DrawHud(myPlayerMoney, myStatHistory, myPlayerName);
    std::cout << "\nChoose your Guess The Number table, " << myPlayerName << ":\n";
    for (size_t tableIndex = 0; tableIndex < myGuessTheNumberTables.size(); ++tableIndex)
    {
        const GuessTheNumberGame& table = myGuessTheNumberTables[tableIndex];
        std::cout << tableIndex + 1 << ". " << table.GetTableName()
                  << " (min " << table.GetMinBet()
                  << ", max " << table.GetMaxBet() << ")\n";
    }
    std::cout << "0. Return to menu\n";

    const std::string prompt = "Select a table (0 to return):";
    int selection = GetInput(0, static_cast<int>(myGuessTheNumberTables.size()), prompt.c_str(), "Please enter a valid option.");
    if (selection == 0)
    {
        return GameState::Menu;
    }

    GuessTheNumberGame& selectedTable = myGuessTheNumberTables[static_cast<size_t>(selection - 1)];
    return selectedTable.Play(myGenerator, myPlayerMoney, myPlayerBet, myStatHistory, myPlayerName);
}

std::string Casino::PromptForPlayerName() const
{
    while (true)
    {
        std::cout << "Enter your name: ";
        std::string input;
        std::getline(std::cin >> std::ws, input);
        if (input.size() < 2)
        {
            std::cout << "Name must have at least two letters.\n";
            continue;
        }

        bool onlyLetters = true;
        for (char character : input)
        {
            if (!std::isalpha(static_cast<unsigned char>(character)))
            {
                onlyLetters = false;
                break;
            }
        }

        if (!onlyLetters)
        {
            std::cout << "Name may contain letters only.\n";
            continue;
        }

        return input;
    }
}
