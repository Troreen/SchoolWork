#pragma once
#include <array>
#include <random>
#include <string>
#include <Windows.h>
#include "CasinoHelpers.h"

class RouletteGame
{
public:
    RouletteGame();

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
    static const std::array<int, 37> ourRouletteLayout;

    static void PrintRouletteCellWithColour(const std::string& aCellText,
        HANDLE aConsoleHandle,
        WORD someDefaultTextAttributes,
        const std::string& aColour);
    static void DisplayRouletteBoard();

};
