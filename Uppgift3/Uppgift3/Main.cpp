#include "CasinoHelpers.h"
#include "GuessTheNumber.h"
#include "OddOrEven.h"
#include "BlackJack.h"
#include "SlotMachine.h"
#include <random>
#include <iostream>
#include <array>

    

int main()
{
    int playerMoney = PLAYER_INITIAL_MONEY;
    int playerBet = 0;
    int winningsGuessTheNumber = 0;
    int winningsOddOrEven = 0;
    int winningsBlackJack = 0;
    int winningsSlot = 0;
    int winningsRoulette = 0;
    std::array<signed int, STAT_HISTORY_SIZE> statHistory = {};

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    DrawHUD(playerMoney, statHistory);

    system("cls");
    GameState currentState = GameState::Menu;


    /*while (true)
    {
        switch (currentState)
        {
        case GameState::Menu:
        {
            currentState = MenuState(playerMoney, playerBet, statHistory);
            break;
        }
        case GameState::GuessTheNumber:
        {
            currentState = GuessTheNumber::PlayGuessTheNumber(generator, playerMoney, playerBet, winningsGuessTheNumber, statHistory);
            std::cout << "\nThat's it, huh? Walking away while you still can.\n";
            std::cout << "Fair enough, kid. The alley's always open if you change your mind...\n";
            break;
        }
        case GameState::OddOrEven:
        {
            currentState = OddOrEven::PlayOddOrEven(generator, playerMoney, playerBet, winningsOddOrEven, statHistory);
            std::cout << "\nStreet's that way, hotshot.\n";
            break;
        }
        case GameState::BlackJack:
        {
            currentState = BlackJack::PlayBlackJack(generator, playerMoney, playerBet, winningsBlackJack, statHistory);
            std::cout << "\nStreet's that way, hotshot.\n";
            break;
        }
        case GameState::SlotMachine:
        {
            currentState = SlotMachine::PlaySlotMachine(generator, playerMoney, playerBet, winningsSlot, statHistory);
            std::cout << "\nStreet's that way, hotshot.\n";
            break;
        }
        case GameState::Roulette:
        {
            currentState = SlotMachine::PlaySlotMachine(generator, playerMoney, playerBet, winningsRoulette, statHistory);
            std::cout << "\nStreet's that way, hotshot.\n";
            break;
        }
        case GameState::Exit:
        {
            system("cls");
            std::cout << "\n\nHere is how much money you're leaving with: " << playerMoney << "\n\n";
            system("pause");
            return 0;
        }
        default:
            return 0;
        }
    }*/
    return 0;
}
