#include "OddOrEvenGame.h"
#include <iostream>
using namespace CasinoHelpers;

int OddOrEvenGame::totalWins = 0;
int OddOrEvenGame::totalLosses = 0;

CasinoHelpers::GameState OddOrEvenGame::play(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES, "Do you want instructions? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::OddOrEven);
    }
    int acceptRules = GetInput(CHOICE_NO, CHOICE_YES, "Do you want to continue? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }
    int playAgain = PLAY_AGAIN_YES;
    int lossStreak = 0;
    int winCounter = 0;
    while (playAgain)
    {
        if (playerMoney <= 0)
        {
            return HandleBankruptcy(playerMoney, statHistory);
        }
        if (RecognizePlayer(GameState::OddOrEven, 0, winnings, 0, 0, 0))
        {
            return GameState::Menu;
        }
        DrawHUD(playerMoney, statHistory);
        Bet(playerMoney, playerBet);
        int guess = GetInput(1, 2, "Enter 1 for odd or 2 for even (both dice must match):", "Please enter 1 for odd or 2 for even.");
        DrawHUD(playerMoney, statHistory);
        std::cout << "Rolling the dice...\n";
        int die1 = RollDie(generator);
        int die2 = RollDie(generator);
        std::cout << "\nResult:\n";
        std::cout << "Die 1: " << die1 << "\n";
        std::cout << "Die 2: " << die2 << "\n";
        bool bothOdd = (((die1 % 2) != 0) && ((die2 % 2) != 0));
        bool bothEven = (((die1 % 2) == 0) && ((die2 % 2) == 0));
        bool split1 = (((die1 % 2) != 0) && ((die2 % 2) == 0));
        bool split2 = (((die1 % 2) == 0) && ((die2 % 2) != 0));
        bool win = (((guess == 1) && bothOdd) || ((guess == 2) && bothEven));
        if (win)
        {
            lossStreak = 0;
            ++winCounter;
            int payout = playerBet * PAYOUT_MULTIPLIER;
            HandlePlayerMoney(playerMoney, playerBet, payout);
            UpdatePlayerStatHistory(statHistory, payout);
            winnings += payout;
            ++totalWins;
            std::cout << "You guessed correctly. You win " << (payout - playerBet) << ".\n";
        }
        else if (split1 || split2)
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            winnings -= playerBet;
            ++totalLosses;
            std::cout << "One die is odd and one is even. You lose your bet of " << playerBet << ".\n";
            playerBet = 0;
            system("pause");
            if (playerMoney <= 0)
            {
                return HandleBankruptcy(playerMoney, statHistory);
            }
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            winnings -= playerBet;
            ++totalLosses;
            std::cout << "Incorrect guess. You lose your bet of " << playerBet << ".\n";
            playerBet = 0;
            system("pause");
            if (playerMoney <= 0)
            {
                return HandleBankruptcy(playerMoney, statHistory);
            }
        }
        playAgain = GetInput(CHOICE_NO, CHOICE_YES, "Play again? (0: No, 1: Yes): ", "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHUD(playerMoney, statHistory);
            std::cout << "Starting a new round.\n\n";
        }
        else
        {
            std::cout << "\nExiting to menu.\n";
        }
    }
    return GameState::Menu;
}
