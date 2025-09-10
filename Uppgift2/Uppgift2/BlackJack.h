#pragma once
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace BlackJack
{
	const int PAYOUT_MULTIPLIER = 2;

	GameState PlayBlackJack(std::mt19937 & aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsBlackJack, std::array<signed int, 5>&aStatHistory);
	std::array<int, 52> CreateDeck();
	void ShuffleDeck(std::array<int, 52>&aDeck, std::mt19937 & aGenerator);
	int GetCardValue(int aCardIndex);
	int GetHandValue(std::array<int, 12>&aHand, int aCardCount);
	int DealOneCard(const std::array<int, 52>&aDeck, int& deckTop);
	void ShowHands(const std::array<int, 12>&aPlayer, int aPlayerCount, const std::array<int, 12>&aDealer, int aDealerCount, bool aRevealDealer, int somePlayerMoney, const std::array<signed int, 5>&aStatHistory);
}
