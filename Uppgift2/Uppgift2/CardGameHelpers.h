#pragma once
#include <random>
#include <array>

std::array<int, 52> CreateDeck();
void ShuffleDeck(std::array<int, 52>& aDeck, std::mt19937& aGenerator);
int GetCardValue(int aCardIndex);
int GetHandValue(std::array<int, 12>& aHand, int aCardCount);
int DealOneCard(const std::array<int, 52>& aDeck, int& aDeckTop);
void ShowHands(const std::array<int, 12>& aPlayer, int aPlayerCount, const std::array<int, 12>& aDealer, int aDealerCount, bool aRevealDealer, int playerMoney, const std::array<signed int, 5>& globalStatHistory);
