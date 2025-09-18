#pragma once
#include "CasinoHelpers.h"
#include <random>
#include <array>
using namespace CasinoHelpers;


namespace SlotMachine
{

	const int PAYOUT_MULTIPLIER_TWO_OF_A_KIND = 2;
	const int PAYOUT_MULTIPLIER_JACKPOT = 5;

	GameState PlaySlotMachine(std::mt19937& aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsSlot, std::array<signed int, 5>& aStatHistory);
}