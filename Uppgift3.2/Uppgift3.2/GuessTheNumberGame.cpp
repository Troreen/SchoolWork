#include "GuessTheNumberGame.h"
#include <iostream>
using namespace CasinoHelpers;

int GuessTheNumberGame::totalWins = 0;
int GuessTheNumberGame::totalLosses = 0;

CasinoHelpers::GameState GuessTheNumberGame::play(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES, "Do you want instructions? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::GuessTheNumber);
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
        if (RecognizePlayer(GameState::GuessTheNumber, winnings, 0, 0, 0, 0))
        {
            return GameState::Menu;
        }
        DrawHUD(playerMoney, statHistory);
        Bet(playerMoney, playerBet);
        int guess = GetInput(2, 12, "Enter your guess for the sum of two dice (2-12):", "Please enter a number between 2 and 12.");
        DrawHUD(playerMoney, statHistory);
        std::cout << "Rolling the dice...";
        int die1 = RollDie(generator);
        int die2 = RollDie(generator);
        int sum = die1 + die2;
        std::cout << "\nResult:\n";
        std::cout << "Die 1: " << die1 << "\n";
        std::cout << "Die 2: " << die2 << "\n";
        std::cout << "Total: " << sum << "\n";
        if (guess == sum)
        {
            lossStreak = 0;
            ++winCounter;
            int payout = playerBet * PAYOUT_MULTIPLIER;
            HandlePlayerMoney(playerMoney, playerBet, payout);
            winnings += payout;
            UpdatePlayerStatHistory(statHistory, payout);
            ++totalWins;
            std::cout << "You guessed correctly. You win " << (payout - playerBet) << ".\n";
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            std::cout << "Incorrect guess. You lose your bet of " << playerBet << ".\n";
            winnings -= playerBet;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            ++totalLosses;
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
            std::cout << ("\nExiting to menu.\n");
        }
    }
    return GameState::Menu;
}
