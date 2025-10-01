#include "Casino.h"
#include <cctype>
#include <iostream>

using namespace CasinoHelpers;

Casino::Casino()
    : guessTheNumberTables{
          GuessTheNumberGame("Low Stakes", 10, 100),
          GuessTheNumberGame("High Stakes", 100, 500)}
    , oddOrEvenGame()
    , blackJackGame()
    , slotMachineGame()
    , rouletteGame()
    , playerName()
    , playerMoney(PLAYER_INITIAL_MONEY)
    , playerBet(0)
    , statHistory{0}
{
    std::random_device rd;
    generator = std::mt19937(rd());
}

void Casino::run()
{
    if (playerName.empty())
    {
        playerName = promptForPlayerName();
    }

    GameState currentState = GameState::Menu;
    while (true)
    {
        switch (currentState)
        {
        case GameState::Menu:
            DrawHUD(playerMoney, statHistory, playerName);
            ShowMenu(playerName);
            currentState = MenuState(playerMoney, playerBet, statHistory, playerName);
            break;
        case GameState::GuessTheNumber:
            currentState = playGuessTheNumber();
            break;
        case GameState::OddOrEven:
            currentState = oddOrEvenGame.play(generator, playerMoney, playerBet, statHistory, playerName);
            break;
        case GameState::BlackJack:
            currentState = blackJackGame.play(generator, playerMoney, playerBet, statHistory, playerName);
            break;
        case GameState::SlotMachine:
            currentState = slotMachineGame.play(generator, playerMoney, playerBet, statHistory, playerName);
            break;
        case GameState::Roulette:
            currentState = rouletteGame.play(generator, playerMoney, playerBet, statHistory, playerName);
            break;
        case GameState::Exit:
            std::cout << "\n" << playerName << ", you are leaving with: " << playerMoney << "\n";
            return;
        default:
            return;
        }
    }
}

CasinoHelpers::GameState Casino::playGuessTheNumber()
{
    DrawHUD(playerMoney, statHistory, playerName);
    std::cout << "\nChoose your Guess The Number table, " << playerName << ":\n";
    for (size_t index = 0; index < guessTheNumberTables.size(); ++index)
    {
        const auto& table = guessTheNumberTables[index];
        std::cout << index + 1 << ". " << table.getTableName()
                  << " (min " << table.getMinBet()
                  << ", max " << table.getMaxBet() << ")\n";
    }
    std::cout << "0. Return to menu\n";

    const std::string prompt = "Select a table (0 to return):";
    int selection = GetInput(0, static_cast<int>(guessTheNumberTables.size()), prompt.c_str(), "Please enter a valid option.");
    if (selection == 0)
    {
        return GameState::Menu;
    }

    GuessTheNumberGame& selectedTable = guessTheNumberTables[static_cast<size_t>(selection - 1)];
    return selectedTable.play(generator, playerMoney, playerBet, statHistory, playerName);
}

std::string Casino::promptForPlayerName() const
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
        for (char ch : input)
        {
            if (!std::isalpha(static_cast<unsigned char>(ch)))
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




