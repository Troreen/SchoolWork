#pragma once
#include <array>
#include <random>
#include <string>
#include "CasinoHelpers.h"

class GuessTheNumberGame
{
private:
    std::string tableName;
    int minBet;
    int maxBet;
    int winnings;
    static int totalWins;
    static int totalLosses;
    static constexpr int PAYOUT_MULTIPLIER_NUMERATOR = 3;
    static constexpr int PAYOUT_MULTIPLIER_DENOMINATOR = 2;

public:
    GuessTheNumberGame(const std::string& tableLabel, int minBetAmount, int maxBetAmount);

    int getWinnings() const { return winnings; }
    const std::string& getTableName() const { return tableName; }
    int getMinBet() const { return minBet; }
    int getMaxBet() const { return maxBet; }
    bool canAfford(int playerMoney) const { return playerMoney >= minBet; }
    static int getTotalWins() { return totalWins; }
    static int getTotalLosses() { return totalLosses; }
    static constexpr int getPayoutMultiplierNumerator() { return PAYOUT_MULTIPLIER_NUMERATOR; }
    static constexpr int getPayoutMultiplierDenominator() { return PAYOUT_MULTIPLIER_DENOMINATOR; }

    CasinoHelpers::GameState play(std::mt19937& generator,
                                  int& playerMoney,
                                  int& playerBet,
                                  std::array<signed int, 5>& statHistory,
                                  const std::string& playerName);
};