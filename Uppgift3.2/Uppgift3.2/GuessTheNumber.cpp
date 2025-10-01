#include "GuessTheNumber.h"
#include <iostream>

using namespace CasinoHelpers;

int GuessTheNumberGame::totalWins = 0;
int GuessTheNumberGame::totalLosses = 0;

GuessTheNumberGame::GuessTheNumberGame(const std::string& tableLabel,
                                       int minBetAmount,
                                       int maxBetAmount)
    : tableName(tableLabel)
    , minBet(1)
    , maxBet(1)
    , winnings(0)
{
    const int normalizedMin = (minBetAmount < 1) ? 1 : minBetAmount;
    const int normalizedMax = (maxBetAmount < normalizedMin) ? normalizedMin : maxBetAmount;
    minBet = normalizedMin;
    maxBet = normalizedMax;
}

CasinoHelpers::GameState GuessTheNumberGame::play(std::mt19937& generator,
                                                  int& playerMoney,
                                                  int& playerBet,
                                                  std::array<signed int, 5>& statHistory,
                                                  const std::string& playerName)
{
    if (!canAfford(playerMoney))
    {
        DrawHUD(playerMoney, statHistory, playerName);
        std::cout << '\n' << playerName << ", you need at least " << minBet
                  << " to join the " << tableName << " table.\n\n";
        system("pause");
        return GameState::Menu;
    }

    DrawHUD(playerMoney, statHistory, playerName);
    std::cout << "\nWelcome to the " << tableName << " table, " << playerName << "!\n";
    std::cout << "Betting limits: minimum " << minBet << ", maximum " << maxBet << ".\n";

    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES,
                                   "Do you want instructions? (0: No, 1: Yes)",
                                   "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::GuessTheNumber, playerName);
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
        if (!canAfford(playerMoney))
        {
            DrawHUD(playerMoney, statHistory, playerName);
            std::cout << '\n' << playerName << ", you do not have the minimum " << minBet
                      << " to continue at the " << tableName << " table.\n\n";
            system("pause");
            return GameState::Menu;
        }

        if (playerMoney <= 0)
        {
            return HandleBankruptcy(playerMoney, statHistory, playerName);
        }

        if (RecognizePlayer(GameState::GuessTheNumber, winnings, 0, 0, 0, 0, playerName))
        {
            return GameState::Menu;
        }

        DrawHUD(playerMoney, statHistory, playerName);
        std::cout << '\n' << tableName << " table betting limits: minimum " << minBet
                  << ", maximum " << maxBet << ".\n";

        int maxAllowedBet = (playerMoney < maxBet) ? playerMoney : maxBet;
        Bet(playerMoney, playerBet, playerName, minBet, maxAllowedBet);

        int guess = GetInput(2, 12,
                             "Enter your guess for the sum of two dice (2-12):",
                             "Please enter a number between 2 and 12.");

        DrawHUD(playerMoney, statHistory, playerName);
        std::cout << "Rolling the dice...";
        int die1 = RollDie(generator);
        int die2 = RollDie(generator);
        const int sum = die1 + die2;

        std::cout << "\nResult:\n";
        std::cout << "Die 1: " << die1 << "\n";
        std::cout << "Die 2: " << die2 << "\n";
        std::cout << "Total: " << sum << "\n";

        if (guess == sum)
        {
            lossStreak = 0;
            ++winCounter;
            const int payout = playerBet * PAYOUT_MULTIPLIER_NUMERATOR / PAYOUT_MULTIPLIER_DENOMINATOR;
            HandlePlayerMoney(playerMoney, playerBet, payout);
            winnings += payout;
            UpdatePlayerStatHistory(statHistory, payout);
            ++totalWins;
            std::cout << playerName << ", you guessed correctly. You win " << (payout - playerBet) << ".\n";
            std::cout << GetWinTaunt(winCounter);
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            std::cout << "Incorrect guess, " << playerName << ". You lose your bet of " << playerBet << ".\n";
            winnings -= playerBet;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            ++totalLosses;
            std::cout << GetLossTaunt(lossStreak);
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
            std::cout << "Starting a new round at the " << tableName << " table.\n\n";
        }
        else
        {
            std::cout << "\nExiting to menu.\n";
        }
    }

    return GameState::Menu;
}