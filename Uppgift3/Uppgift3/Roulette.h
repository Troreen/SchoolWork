#pragma once
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace Roulette
{
	const int GUESS_ODD = 1;
	const int GUESS_EVEN = 2;
	const int PAYOUT_MULTIPLIER = 3;

	GameState Roulette(std::mt19937& aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsOddOrEven, std::array<signed int, 5>& aStatHistory);
}
