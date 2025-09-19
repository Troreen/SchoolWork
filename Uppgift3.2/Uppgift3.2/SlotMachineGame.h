#pragma once
#include <random>
#include <array>
#include "CasinoHelpers.h"

class SlotMachineGame
{
private:
    int winnings;
    static int totalWins;
    static int totalLosses;
    static const int PAYOUT_MULTIPLIER_TWO_OF_A_KIND = 2;
    static const int PAYOUT_MULTIPLIER_JACKPOT = 5;
public:
    SlotMachineGame() : winnings(0) {}
    int getWinnings() const { return winnings; }
    static int getTotalWins() { return totalWins; }
    static int getTotalLosses() { return totalLosses; }
    static int getPayoutMultiplierTwoOfAKind() { return PAYOUT_MULTIPLIER_TWO_OF_A_KIND; }
    static int getPayoutMultiplierJackpot() { return PAYOUT_MULTIPLIER_JACKPOT; }
    CasinoHelpers::GameState play(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory);
};
