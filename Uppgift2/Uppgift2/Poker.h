#pragma once
#include <random>
#include <array>
#include "CardGameHelpers.h"

GameState PlayPoker(std::mt19937& aGenerator, int& aPlayerMoney, int& aPlayerBet, int& aWinningsPoker, std::array<signed int, 5>& aGlobalStatHistory);
GameState PlayTexasHoldem(std::mt19937& aGenerator, int aPlayerMoney[], int aPlayerBet[], int& aWinningsTexasHoldem, std::array<signed int, 5>& aGlobalStatHistory, int aNumPlayers, int aPlayerIndex);
