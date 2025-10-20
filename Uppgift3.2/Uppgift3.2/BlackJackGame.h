#pragma once
#include <array>
#include <random>
#include <string>
#include "CasinoHelpers.h"

class BlackJackGame
{
public:
    BlackJackGame();

    int GetWinnings() const;

    static int GetTotalWins();

    static int GetTotalLosses();

    static int GetPayoutMultiplier();

    CasinoHelpers::GameState Play(std::mt19937& aGenerator,
                                  int& somePlayerMoney,
                                  int& aPlayerBet,
                                  CasinoHelpers::StatHistory& aStatHistory,
                                  const std::string& aPlayerName);

private:
    int myWinnings;
    static int ourTotalWins;
    static int ourTotalLosses;
    static const int ourPayoutMultiplier;

    static std::array<int, 52> CreateDeck();
    static void ShuffleDeck(std::array<int, 52>& aDeck, std::mt19937& aGenerator);
    static int GetCardValue(int aCardIndex);
    static int GetHandValue(const std::array<int, 12>& aHand, int aCardCount);
    static int DealOneCard(const std::array<int, 52>& aDeck, int& aDeckTop);
    static void ShowHands(const std::array<int, 12>& aPlayerHand,
                          int aPlayerCount,
                          const std::array<int, 12>& aDealerHand,
                          int aDealerCount,
                          bool aRevealDealer);

};
