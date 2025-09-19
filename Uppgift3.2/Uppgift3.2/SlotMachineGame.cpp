#include "SlotMachineGame.h"
#include <iostream>
using namespace CasinoHelpers;

int SlotMachineGame::totalWins = 0;
int SlotMachineGame::totalLosses = 0;

CasinoHelpers::GameState SlotMachineGame::play(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES, "Do you want instructions? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        std::cout << "Slot Machine: Bet, spin, and try to get 3 of a kind.\n";
        std::cout << "Payouts: 3 of a kind = 5x bet, 2 of a kind = 2x bet, otherwise you lose your bet.\n\n";
    }
    int acceptRules = GetInput(CHOICE_NO, CHOICE_YES, "Do you want to continue? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }
    int playAgain = PLAY_AGAIN_YES;
    while (playAgain)
    {
        if (RecognizePlayer(GameState::SlotMachine, 0, 0, 0, 0, winnings))
        {
            return GameState::Menu;
        }
        if (playerMoney <= 0)
        {
            return HandleBankruptcy(playerMoney, statHistory);
        }
        DrawHUD(playerMoney, statHistory);
        Bet(playerMoney, playerBet);
        std::cout << "Spinning the reels...\n";
        std::uniform_int_distribution<int> symbolDist(1, 5);
        int reels[3] = { symbolDist(generator), symbolDist(generator), symbolDist(generator) };
        std::cout << "[ " << reels[0] << " | " << reels[1] << " | " << reels[2] << " ]\n";
        int payout = 0;
        if (reels[0] == reels[1] && reels[1] == reels[2])
        {
            payout = playerBet * PAYOUT_MULTIPLIER_JACKPOT;
            std::cout << "Three of a kind. You win " << payout << ".\n";
            ++totalWins;
        }
        else if (reels[0] == reels[1] || reels[1] == reels[2] || reels[0] == reels[2])
        {
            payout = playerBet * PAYOUT_MULTIPLIER_TWO_OF_A_KIND;
            std::cout << "Two of a kind. You win " << payout << ".\n";
            ++totalWins;
        }
        else
        {
            payout = 0;
            std::cout << "No match. You lose your bet.\n";
            ++totalLosses;
        }
        HandlePlayerMoney(playerMoney, playerBet, payout);
        UpdatePlayerStatHistory(statHistory, payout - playerBet);
        playAgain = GetInput(PLAY_AGAIN_NO, PLAY_AGAIN_YES, "Play again? (0: No, 1: Yes): ", "Please enter 0 for No or 1 for Yes.");
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
