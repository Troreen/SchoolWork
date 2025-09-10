#pragma once
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace GuessTheNumber 
{
	const int PAYOUT_MULTIPLIER = 3 / 2;

	GameState PlayGuessTheNumber(std::mt19937& aGenerator, int& playerMoney, int& playerBet, int& winningsGuessTheNumber, std::array<signed int, 5>& globalStatHistory);

}
