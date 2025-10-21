#pragma once
#include <array>
#include <random>
#include <string>
#include "CasinoHelpers.h"

class OddOrEvenGame
{
public:
    OddOrEvenGame();

    int GetWinnings() const;

    static int GetTotalWins();

    static int GetTotalLosses();

    static int GetPayoutMultiplier();

    CasinoHelpers::GameState Play(std::mt19937& aGenerator,
        int& somePlayerMoney,
        int& aPlayerBet,
        CasinoHelpers::StatHistory& aStatHistory,
        const std::string& aPlayerName);

private:
    int myWinnings;
    static int ourTotalWins;
    static int ourTotalLosses;
    static const int ourPayoutMultiplier;

};
