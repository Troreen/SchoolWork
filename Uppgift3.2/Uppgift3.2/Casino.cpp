#include "Casino.h"
#include <iostream>
using namespace CasinoHelpers;

Casino::Casino()
    : playerMoney(PLAYER_INITIAL_MONEY), playerBet(0), statHistory{0}
{
    std::random_device rd;
    generator = std::mt19937(rd());
}

void Casino::run()
{
    GameState currentState = GameState::Menu;
    while (true)
    {
        switch (currentState)
        {
        case GameState::Menu:
            DrawHUD(playerMoney, statHistory);
            ShowMenu();
            currentState = MenuState(playerMoney, playerBet, statHistory);
            break;
        case GameState::GuessTheNumber:
            currentState = guessTheNumberGame.play(generator, playerMoney, playerBet, statHistory);
            break;
        case GameState::OddOrEven:
            currentState = oddOrEvenGame.play(generator, playerMoney, playerBet, statHistory);
            break;
        case GameState::BlackJack:
            currentState = blackJackGame.play(generator, playerMoney, playerBet, statHistory);
            break;
        case GameState::SlotMachine:
            currentState = slotMachineGame.play(generator, playerMoney, playerBet, statHistory);
            break;
        case GameState::Roulette:
            currentState = rouletteGame.play(generator, playerMoney, playerBet, statHistory);
            break;
        case GameState::Exit:
            std::cout << "\nYou are leaving with: " << playerMoney << "\n";
            return;
        default:
            return;
        }
    }
}
