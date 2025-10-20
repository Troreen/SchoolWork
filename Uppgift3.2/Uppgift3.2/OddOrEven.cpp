#include "OddOrEven.h"
#include <iostream>

using namespace CasinoHelpers;

OddOrEvenGame::OddOrEvenGame()
    : myWinnings(0)
{
}

int OddOrEvenGame::ourTotalWins = 0;
int OddOrEvenGame::ourTotalLosses = 0;
const int OddOrEvenGame::ourPayoutMultiplier = 3;

int OddOrEvenGame::GetWinnings() const
{
    return myWinnings;
}

int OddOrEvenGame::GetTotalWins()
{
    return ourTotalWins;
}

int OddOrEvenGame::GetTotalLosses()
{
    return ourTotalLosses;
}

int OddOrEvenGame::GetPayoutMultiplier()
{
    return ourPayoutMultiplier;
}

CasinoHelpers::GameState OddOrEvenGame::Play(std::mt19937& aGenerator,
                                             int& somePlayerMoney,
                                             int& aPlayerBet,
                                             CasinoHelpers::StatHistory& aStatHistory,
                                             const std::string& aPlayerName)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES,
                                   "Do you want instructions? (0: No, 1: Yes)",
                                   "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::OddOrEven, playerName);
    }

    int acceptRules = GetInput(CHOICE_NO, CHOICE_YES,
                               "Do you want to continue? (0: No, 1: Yes)",
                               "Please enter 0 for No or 1 for Yes.");
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
            return HandleBankruptcy(playerMoney, statHistory, playerName);
        }

        if (RecognizePlayer(GameState::OddOrEven, 0, winnings, 0, 0, 0, playerName))
        {
            return GameState::Menu;
        }

        DrawHUD(playerMoney, statHistory, playerName);
        Bet(playerMoney, playerBet, playerName);

        int guess = GetInput(1, 2,
                             "Enter 1 for odd or 2 for even (both dice must match):",
                             "Please enter 1 for odd or 2 for even.");

        DrawHUD(playerMoney, statHistory, playerName);
        std::cout << "Rolling the dice...\n";
        int die1 = RollDie(generator);
        int die2 = RollDie(generator);

        std::cout << "\nResult:\n";
        std::cout << "Die 1: " << die1 << "\n";
        std::cout << "Die 2: " << die2 << "\n";

        const bool bothOdd = ((die1 % 2) != 0) && ((die2 % 2) != 0);
        const bool bothEven = ((die1 % 2) == 0) && ((die2 % 2) == 0);
        const bool split1 = ((die1 % 2) != 0) && ((die2 % 2) == 0);
        const bool split2 = ((die1 % 2) == 0) && ((die2 % 2) != 0);
        const bool win = ((guess == 1) && bothOdd) || ((guess == 2) && bothEven);

        if (win)
        {
            lossStreak = 0;
            ++winCounter;
            const int payout = playerBet * PAYOUT_MULTIPLIER;
            HandlePlayerMoney(playerMoney, playerBet, payout);
            UpdatePlayerStatHistory(statHistory, payout);
            winnings += payout;
            ++totalWins;
            std::cout << playerName << ", you guessed correctly. You win " << (payout - playerBet) << ".\n";
        }
        else if (split1 || split2)
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            winnings -= playerBet;
            ++totalLosses;
            std::cout << "One die is odd and one is even, " << playerName << ". You lose your bet of " << playerBet << ".\n";
            playerBet = 0;
            system("pause");
            if (playerMoney <= 0)
            {
                return HandleBankruptcy(playerMoney, statHistory, playerName);
            }
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            winnings -= playerBet;
            ++totalLosses;
            std::cout << "Incorrect guess, " << playerName << ". You lose your bet of " << playerBet << ".\n";
            playerBet = 0;
            system("pause");
            if (playerMoney <= 0)
            {
                return HandleBankruptcy(playerMoney, statHistory, playerName);
            }
        }

        playAgain = GetInput(CHOICE_NO, CHOICE_YES,
                             "Play again? (0: No, 1: Yes): ",
                             "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHUD(playerMoney, statHistory, playerName);
            std::cout << "Starting a new round, " << playerName << ".\n\n";
        }
        else
        {
            std::cout << "\nExiting to menu, " << playerName << ".\n";
        }
    }

    return GameState::Menu;
}