#pragma once
#include <array>
#include <random>
#include <string>
#include "CasinoHelpers.h"

class OddOrEvenGame
{
public:
    OddOrEvenGame()
        : myWinnings(0)
    {
    }

    int GetWinnings() const
    {
        return myWinnings;
    }

    static int GetTotalWins()
    {
        return ourTotalWins;
    }

    static int GetTotalLosses()
    {
        return ourTotalLosses;
    }

    static constexpr int GetPayoutMultiplier()
    {
        return ourPayoutMultiplier;
    }

    CasinoHelpers::GameState Play(std::mt19937& aGenerator,
                                  int& somePlayerMoney,
                                  int& aPlayerBet,
                                  std::array<signed int, CasinoHelpers::globalStatHistorySize>& aStatHistory,
                                  const std::string& aPlayerName);

private:
    int myWinnings;
    static int ourTotalWins;
    static int ourTotalLosses;
    static constexpr int ourPayoutMultiplier = 3;

};
