#include "OddOrEven.h"

#include <cstdlib>
#include <iostream>

using namespace CasinoHelpers;

int OddOrEvenGame::ourTotalWins = 0;
int OddOrEvenGame::ourTotalLosses = 0;

CasinoHelpers::GameState OddOrEvenGame::Play(std::mt19937& aGenerator,
                                             int& somePlayerMoney,
                                             int& aPlayerBet,
                                             std::array<signed int, CasinoHelpers::globalStatHistorySize>& aStatHistory,
                                             const std::string& aPlayerName)
{
    int seeInstructions = GetInput(globalChoiceNo, globalChoiceYes,
                                   "Do you want instructions? (0: No, 1: Yes)",
                                   "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::OddOrEven, aPlayerName);
    }

    int acceptRules = GetInput(globalChoiceNo, globalChoiceYes,
                               "Do you want to continue? (0: No, 1: Yes)",
                               "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }

    int playAgain = globalPlayAgainYes;
    int lossStreak = 0;
    int winCounter = 0;
    while (playAgain)
    {
        if (somePlayerMoney <= 0)
        {
            return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
        }

        if (RecognizePlayer(GameState::OddOrEven, 0, myWinnings, 0, 0, 0, aPlayerName))
        {
            return GameState::Menu;
        }

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        Bet(somePlayerMoney, aPlayerBet, aPlayerName);

        int guess = GetInput(1, 2,
                             "Enter 1 for odd or 2 for even (both dice must match):",
                             "Please enter 1 for odd or 2 for even.");

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        std::cout << "Rolling the dice...\n";
        int dieOne = RollDie(aGenerator);
        int dieTwo = RollDie(aGenerator);

        std::cout << "\nResult:\n";
        std::cout << "Die 1: " << dieOne << "\n";
        std::cout << "Die 2: " << dieTwo << "\n";

        const bool bothOdd = ((dieOne % 2) != 0) && ((dieTwo % 2) != 0);
        const bool bothEven = ((dieOne % 2) == 0) && ((dieTwo % 2) == 0);
        const bool splitOddEven = ((dieOne % 2) != 0) && ((dieTwo % 2) == 0);
        const bool splitEvenOdd = ((dieOne % 2) == 0) && ((dieTwo % 2) != 0);
        const bool playerWon = ((guess == 1) && bothOdd) || ((guess == 2) && bothEven);

        if (playerWon)
        {
            lossStreak = 0;
            ++winCounter;
            const int payout = aPlayerBet * ourPayoutMultiplier;
            HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
            UpdatePlayerStatHistory(aStatHistory, payout);
            myWinnings += payout;
            ++ourTotalWins;
            std::cout << aPlayerName << ", you guessed correctly. You win " << (payout - aPlayerBet) << ".\n";
        }
        else if (splitOddEven || splitEvenOdd)
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
            myWinnings -= aPlayerBet;
            ++ourTotalLosses;
            std::cout << "One die is odd and one is even, " << aPlayerName << ". You lose your bet of " << aPlayerBet << ".\n";
            aPlayerBet = 0;
            system("pause");
            if (somePlayerMoney <= 0)
            {
                return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
            }
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
            myWinnings -= aPlayerBet;
            ++ourTotalLosses;
            std::cout << "Incorrect guess, " << aPlayerName << ". You lose your bet of " << aPlayerBet << ".\n";
            aPlayerBet = 0;
            system("pause");
            if (somePlayerMoney <= 0)
            {
                return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
            }
        }

        playAgain = GetInput(globalChoiceNo, globalChoiceYes,
                              "Play again? (0: No, 1: Yes): ",
                              "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
            std::cout << "Starting a new round, " << aPlayerName << ".\n\n";
        }
        else
        {
            std::cout << "\nExiting to menu, " << aPlayerName << ".\n";
        }
    }

    return GameState::Menu;
}
