#include "SlotMachine.h"
#include <iostream>

using namespace CasinoHelpers;

SlotMachineGame::SlotMachineGame()
    : myWinnings(0)
{
}

int SlotMachineGame::ourTotalWins = 0;
int SlotMachineGame::ourTotalLosses = 0;
const int SlotMachineGame::ourPayoutMultiplierTwoOfAKind = 2;
const int SlotMachineGame::ourPayoutMultiplierJackpot = 5;

int SlotMachineGame::GetWinnings() const
{
    return myWinnings;
}

int SlotMachineGame::GetTotalWins()
{
    return ourTotalWins;
}

int SlotMachineGame::GetTotalLosses()
{
    return ourTotalLosses;
}

int SlotMachineGame::GetPayoutMultiplierTwoOfAKind()
{
    return ourPayoutMultiplierTwoOfAKind;
}

int SlotMachineGame::GetPayoutMultiplierJackpot()
{
    return ourPayoutMultiplierJackpot;
}

CasinoHelpers::GameState SlotMachineGame::Play(std::mt19937 &aGenerator,
                                               int &somePlayerMoney,
                                               int &aPlayerBet,
                                               CasinoHelpers::StatHistory &aStatHistory,
                                               const std::string &aPlayerName)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES,
                                   "Do you want instructions? (0: No, 1: Yes)",
                                   "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        std::cout << playerName << ", here's how the slot machine works: bet, spin, and hunt for three of a kind.\n";
        std::cout << "Payouts: 3 of a kind = 5x bet, 2 of a kind = 2x bet, otherwise you lose your bet.\n\n";
    }

    int acceptRules = GetInput(CHOICE_NO, CHOICE_YES,
                               "Do you want to continue? (0: No, 1: Yes)",
                               "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }

    int playAgain = PLAY_AGAIN_YES;
    while (playAgain)
    {
        if (RecognizePlayer(GameState::SlotMachine, 0, 0, 0, 0, winnings, playerName))
        {
            return GameState::Menu;
        }

        if (playerMoney <= 0)
        {
            return HandleBankruptcy(playerMoney, statHistory, playerName);
        }

        DrawHUD(playerMoney, statHistory, playerName);
        Bet(playerMoney, playerBet, playerName);

        std::cout << playerName << ", spinning the reels...\n";
        std::uniform_int_distribution<int> symbolDist(1, 5);
        int reels[3] = { symbolDist(generator), symbolDist(generator), symbolDist(generator) };
        std::cout << "[ " << reels[0] << " | " << reels[1] << " | " << reels[2] << " ]\n";

        int payout = 0;
        if (reels[0] == reels[1] && reels[1] == reels[2])
        {
            payout = playerBet * PAYOUT_MULTIPLIER_JACKPOT;
            std::cout << "Three of a kind, " << playerName << ". You win " << payout << ".\n";
            ++totalWins;
        }
        else if (reels[0] == reels[1] || reels[1] == reels[2] || reels[0] == reels[2])
        {
            payout = playerBet * PAYOUT_MULTIPLIER_TWO_OF_A_KIND;
            std::cout << "Two of a kind, " << playerName << ". You win " << payout << ".\n";
            ++totalWins;
        }
        else
        {
            payout = 0;
            std::cout << "No match this time, " << playerName << ". You lose your bet.\n";
            ++totalLosses;
        }

        HandlePlayerMoney(playerMoney, playerBet, payout);
        UpdatePlayerStatHistory(statHistory, payout - playerBet);

        playAgain = GetInput(PLAY_AGAIN_NO, PLAY_AGAIN_YES,
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