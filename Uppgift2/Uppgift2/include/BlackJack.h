#pragma once
#include "CasinoHelpers.h"
#include <array>
#include <random>
#include <iostream>
#include <Windows.h>
#include <string>

namespace BlackJack
{
    GameState PlayBlackJack(std::mt19937 & aGenerator, int& playerMoney, int& playerBet, int& winningsBlackJack, std::array<signed int, 5>&globalStatHistory);
    std::array<int, 52> CreateDeck();
    void ShuffleDeck(std::array<int, 52>&aDeck, std::mt19937 & aGenerator);
    int GetCardValue(int aCardIndex);
    int GetHandValue(std::array<int, 12>&aHand, int aCardCount);
    int DealOneCard(const std::array<int, 52>&aDeck, int& aDeckTop);
    std::string CardToText(int aCardIndex);
    void ShowHands(const std::array<int, 12>&aPlayerHand, int aPlayerCardCount, const std::array<int, 12>&aDealerHand, int aDealerCardCount, bool aRevealDealer, int playerMoney, const std::array<signed int, 5>&globalStatHistory);
}
