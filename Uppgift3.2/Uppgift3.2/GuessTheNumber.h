#pragma once
#include <array>
#include <random>
#include <string>
#include "CasinoHelpers.h"

class GuessTheNumberGame
{
public:
    GuessTheNumberGame(const std::string& aTableLabel, int aMinBetAmount, int aMaxBetAmount);

    int GetWinnings() const;

    const std::string& GetTableName() const;

    int GetMinBet() const;

    int GetMaxBet() const;

    bool CanAfford(int somePlayerMoney) const;

    static int GetTotalWins();

    static int GetTotalLosses();

    static int GetPayoutMultiplierNumerator();

    static int GetPayoutMultiplierDenominator();

    CasinoHelpers::GameState Play(std::mt19937& aGenerator,
        int& somePlayerMoney,
        int& aPlayerBet,
        CasinoHelpers::StatHistory& aStatHistory,
        const std::string& aPlayerName);

private:
    std::string myTableName;
    int myMinBet;
    int myMaxBet;
    int myWinnings;
    static int ourTotalWins;
    static int ourTotalLosses;
    static const int ourPayoutMultiplierNumerator;
    static const int ourPayoutMultiplierDenominator;

};
