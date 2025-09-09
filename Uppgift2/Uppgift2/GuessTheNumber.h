#pragma once
#include <random>
#include <array>

namespace GuessTheNumber {
    GameState PlayGuessTheNumber(std::mt19937& aGenerator, int& playerMoney, int& playerBet, int& winningsGuessTheNumber, std::array<signed int, 5>& globalStatHistory);
    const char* GetLossTaunt(int aStreak);
    const char* GetWinTaunt(int aWinCounter);
}
