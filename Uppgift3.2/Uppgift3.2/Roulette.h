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
    int winnings;
    static int totalWins;
    static int totalLosses;
    static constexpr int PAYOUT_MULTIPLIER = 3;

public:
    RouletteGame() : winnings(0) {}

    int getWinnings() const { return winnings; }
    static int getTotalWins() { return totalWins; }
    static int getTotalLosses() { return totalLosses; }
    static constexpr int getPayoutMultiplier() { return PAYOUT_MULTIPLIER; }

    CasinoHelpers::GameState play(std::mt19937& generator,
        int& playerMoney,
        int& playerBet,
        std::array<signed int, 5>& statHistory,
        const std::string& playerName);
};
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
