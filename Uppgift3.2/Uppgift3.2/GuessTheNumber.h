#pragma once
#include <array>
#include <random>
#include <string>
#include "CasinoHelpers.h"

class GuessTheNumberGame
{
public:
    GuessTheNumberGame(const std::string& aTableLabel, int aMinBetAmount, int aMaxBetAmount);

    int GetWinnings() const
    {
        return myWinnings;
    }

    const std::string& GetTableName() const
    {
        return myTableName;
    }

    int GetMinBet() const
    {
        return myMinBet;
    }

    int GetMaxBet() const
    {
        return myMaxBet;
    }

    bool CanAfford(int somePlayerMoney) const
    {
        return somePlayerMoney >= myMinBet;
    }

    static int GetTotalWins()
    {
        return ourTotalWins;
    }

    static int GetTotalLosses()
    {
        return ourTotalLosses;
    }

    static constexpr int GetPayoutMultiplierNumerator()
    {
        return ourPayoutMultiplierNumerator;
    }

    static constexpr int GetPayoutMultiplierDenominator()
    {
        return ourPayoutMultiplierDenominator;
    }

    CasinoHelpers::GameState Play(std::mt19937& aGenerator,
                                  int& somePlayerMoney,
                                  int& aPlayerBet,
                                  std::array<signed int, CasinoHelpers::globalStatHistorySize>& aStatHistory,
                                  const std::string& aPlayerName);

private:
    std::string myTableName;
    int myMinBet;
    int myMaxBet;
    int myWinnings;
    static int ourTotalWins;
    static int ourTotalLosses;
    static constexpr int ourPayoutMultiplierNumerator = 3;
    static constexpr int ourPayoutMultiplierDenominator = 2;

};
