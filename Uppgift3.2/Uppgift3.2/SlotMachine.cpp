#include "SlotMachine.h"

#include <cstdlib>
#include <iostream>

using namespace CasinoHelpers;

SlotMachineGame::SlotMachineGame()
    : myWinnings(0)
{
}

SlotMachineGame::~SlotMachineGame() = default;

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

CasinoHelpers::GameState SlotMachineGame::Play(std::mt19937& aGenerator,
    int& somePlayerMoney,
    int& aPlayerBet,
    CasinoHelpers::StatHistory& aStatHistory,
    const std::string& aPlayerName)
{
    int seeInstructions = GetInput(globalChoiceNo, globalChoiceYes,
        "Do you want instructions? (0: No, 1: Yes)",
        "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        std::cout << aPlayerName << ", here's how the slot machine works: bet, spin, and hunt for three of a kind.\n";
        std::cout << "Payouts: 3 of a kind = 5x bet, 2 of a kind = 2x bet, otherwise you lose your bet.\n\n";
    }

    int acceptRules = GetInput(globalChoiceNo, globalChoiceYes,
        "Do you want to continue? (0: No, 1: Yes)",
        "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }

    int playAgain = globalPlayAgainYes;
    while (playAgain)
    {
        if (RecognizePlayer(GameState::SlotMachine, 0, 0, 0, 0, myWinnings, aPlayerName))
        {
            return GameState::Menu;
        }

        if (somePlayerMoney <= 0)
        {
            return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
        }

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        Bet(somePlayerMoney, aPlayerBet, aPlayerName);

        std::cout << aPlayerName << ", spinning the reels...\n";
        std::uniform_int_distribution<int> symbolDistribution(1, 5);
        std::array<int, 3> reels{ symbolDistribution(aGenerator), symbolDistribution(aGenerator), symbolDistribution(aGenerator) };
        std::cout << "[ " << reels[0] << " | " << reels[1] << " | " << reels[2] << " ]\n";

        int payout = 0;
        if (reels[0] == reels[1] && reels[1] == reels[2])
        {
            payout = aPlayerBet * ourPayoutMultiplierJackpot;
            std::cout << "Three of a kind, " << aPlayerName << ". You win " << payout << ".\n";
            ++ourTotalWins;
        }
        else if (reels[0] == reels[1] || reels[1] == reels[2] || reels[0] == reels[2])
        {
            payout = aPlayerBet * ourPayoutMultiplierTwoOfAKind;
            std::cout << "Two of a kind, " << aPlayerName << ". You win " << payout << ".\n";
            ++ourTotalWins;
        }
        else
        {
            payout = 0;
            std::cout << "No match this time, " << aPlayerName << ". You lose your bet.\n";
            ++ourTotalLosses;
        }

        const int betAmount = aPlayerBet;
        HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
        UpdatePlayerStatHistory(aStatHistory, payout - betAmount);
        myWinnings += payout - betAmount;

        playAgain = GetInput(globalPlayAgainNo, globalPlayAgainYes,
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
