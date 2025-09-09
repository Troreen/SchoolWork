#pragma once
#include "CasinoHelpers.h"
#include <random>
#include <array>

GameState PlaySlotMachine(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory);
