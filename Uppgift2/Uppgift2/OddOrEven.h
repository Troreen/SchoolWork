#pragma once
#include <random>
#include <array>

GameState PlayOddOrEven(std::mt19937& aGenerator, int& playerMoney, int& playerBet, int& winningsOddOrEven, std::array<signed int, 5>& globalStatHistory);
