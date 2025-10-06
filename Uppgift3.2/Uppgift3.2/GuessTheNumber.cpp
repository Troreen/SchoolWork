#include "GuessTheNumber.h"

#include <cstdlib>
#include <iostream>

using namespace CasinoHelpers;

int GuessTheNumberGame::ourTotalWins = 0;
int GuessTheNumberGame::ourTotalLosses = 0;

GuessTheNumberGame::GuessTheNumberGame(const std::string& aTableLabel,
                                       int aMinBetAmount,
                                       int aMaxBetAmount)
    : myTableName(aTableLabel)
    , myMinBet(1)
    , myMaxBet(1)
    , myWinnings(0)
{
    const int normalizedMin = (aMinBetAmount < 1) ? 1 : aMinBetAmount;
    const int normalizedMax = (aMaxBetAmount < normalizedMin) ? normalizedMin : aMaxBetAmount;
    myMinBet = normalizedMin;
    myMaxBet = normalizedMax;
}

CasinoHelpers::GameState GuessTheNumberGame::Play(std::mt19937& aGenerator,
                                                  int& somePlayerMoney,
                                                  int& aPlayerBet,
                                                  std::array<signed int, CasinoHelpers::globalStatHistorySize>& aStatHistory,
                                                  const std::string& aPlayerName)
{
    if (!CanAfford(somePlayerMoney))
    {
        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        std::cout << '\n' << aPlayerName << ", you need at least " << myMinBet
                  << " to join the " << myTableName << " table.\n\n";
        system("pause");
        return GameState::Menu;
    }

    DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
    std::cout << "\nWelcome to the " << myTableName << " table, " << aPlayerName << "!\n";
    std::cout << "Betting limits: minimum " << myMinBet << ", maximum " << myMaxBet << ".\n";

    int seeInstructions = GetInput(globalChoiceNo, globalChoiceYes,
                                   "Do you want instructions? (0: No, 1: Yes)",
                                   "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::GuessTheNumber, aPlayerName);
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
        if (!CanAfford(somePlayerMoney))
        {
            DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
            std::cout << '\n' << aPlayerName << ", you do not have the minimum " << myMinBet
                      << " to continue at the " << myTableName << " table.\n\n";
            system("pause");
            return GameState::Menu;
        }

        if (somePlayerMoney <= 0)
        {
            return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
        }

        if (RecognizePlayer(GameState::GuessTheNumber, myWinnings, 0, 0, 0, 0, aPlayerName))
        {
            return GameState::Menu;
        }

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        std::cout << '\n' << myTableName << " table betting limits: minimum " << myMinBet
                  << ", maximum " << myMaxBet << ".\n";

        int maxAllowedBet = (somePlayerMoney < myMaxBet) ? somePlayerMoney : myMaxBet;
        Bet(somePlayerMoney, aPlayerBet, aPlayerName, myMinBet, maxAllowedBet);

        int guess = GetInput(2, 12,
                             "Enter your guess for the sum of two dice (2-12):",
                             "Please enter a number between 2 and 12.");

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        std::cout << "Rolling the dice...";
        int dieOne = RollDie(aGenerator);
        int dieTwo = RollDie(aGenerator);
        const int rolledSum = dieOne + dieTwo;

        std::cout << "\nResult:\n";
        std::cout << "Die 1: " << dieOne << "\n";
        std::cout << "Die 2: " << dieTwo << "\n";
        std::cout << "Total: " << rolledSum << "\n";

        if (guess == rolledSum)
        {
            lossStreak = 0;
            ++winCounter;
            const int payout = aPlayerBet * ourPayoutMultiplierNumerator / ourPayoutMultiplierDenominator;
            HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
            myWinnings += payout;
            UpdatePlayerStatHistory(aStatHistory, payout);
            ++ourTotalWins;
            std::cout << aPlayerName << ", you guessed correctly. You win " << (payout - aPlayerBet) << ".\n";
            std::cout << GetWinTaunt(winCounter);
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            std::cout << "Incorrect guess, " << aPlayerName << ". You lose your bet of " << aPlayerBet << ".\n";
            myWinnings -= aPlayerBet;
            UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
            ++ourTotalLosses;
            std::cout << GetLossTaunt(lossStreak);
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
            std::cout << "Starting a new round at the " << myTableName << " table.\n\n";
        }
        else
        {
            std::cout << "\nExiting to menu.\n";
        }
    }

    return GameState::Menu;
}
