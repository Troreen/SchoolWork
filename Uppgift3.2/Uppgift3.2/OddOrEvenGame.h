#pragma once
#include <random>
#include <array>
#include "CasinoHelpers.h"

class OddOrEvenGame {
private:
    int winnings;
    static int totalWins;
    static int totalLosses;
    static const int PAYOUT_MULTIPLIER = 3;
public:
    OddOrEvenGame() : winnings(0) {}
    int getWinnings() const { return winnings; }
    static int getTotalWins() { return totalWins; }
    static int getTotalLosses() { return totalLosses; }
    static int getPayoutMultiplier() { return PAYOUT_MULTIPLIER; }
    CasinoHelpers::GameState play(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory);
};
