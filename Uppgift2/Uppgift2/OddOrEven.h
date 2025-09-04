#pragma once
#include <random>
#include <array>

const int GUESS_ODD = 1;
const int GUESS_EVEN = 2;
const int PAYOUT_MULTIPLIER = 3;

GameState PlayOddOrEven(std::mt19937& aGenerator, int& playerMoney, int& playerBet, int& winningsOddOrEven, std::array<signed int, 5>& globalStatHistory);
