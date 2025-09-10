#pragma once
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace GuessTheNumber
{
	const int PAYOUT_MULTIPLIER = 3 / 2;

	GameState PlayGuessTheNumber(std::mt19937& aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsGuessTheNumber, std::array<signed int, 5>& aStatHistory);
}
