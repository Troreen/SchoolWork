#include "CardGameHelpers.h"
#include <iostream>
#include <random>
#include <array>

static const char* CARD_RANKS[14] = { "", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
static const char* CARD_SUITS[4] = { "S", "H", "D", "C" };

std::array<int, 52> CreateDeck() {
    std::array<int, 52> deck{};
    for (int i = 0; i < 52; ++i) deck[i] = i;
    return deck;
}

void ShuffleDeck(std::array<int, 52>& aDeck, std::mt19937& aGenerator) {
    for (int i = static_cast<int>(aDeck.size()) - 1; i > 0; i--) {
        std::uniform_int_distribution<int> distribution(0, i);
        int j = distribution(aGenerator);
        int tempVar = aDeck[i];
        aDeck[i] = aDeck[j];
        aDeck[j] = tempVar;
    }
}

int GetCardValue(int aCardIndex) {
    int rank = (aCardIndex % 13) + 1;
    if (rank == 13) {
        rank = 11;
    } else if (rank > 9) {
        rank = 10;
    } else {
        rank++;
    }
    return rank;
}

int GetHandValue(std::array<int, 12>& aHand, int aCardCount) {
    int total = 0;
    int aceCount = 0;
    for (int i = 0; i < aCardCount; i++) {
        int cardValue = GetCardValue(aHand[i]);
        total += cardValue;
        if (cardValue == 11) {
            aceCount++;
        }
    }
    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    return total;
}

int DealOneCard(const std::array<int, 52>& aDeck, int& aDeckTop) {
    int card = aDeck[aDeckTop];
    ++aDeckTop;
    return card;
}

void ShowHands(const std::array<int, 12>& aPlayerHand, int aPlayerCardCount, const std::array<int, 12>& aDealerHand, int aDealerCardCount, bool aRevealDealer, int playerMoney, const std::array<signed int, 5>& globalStatHistory) {
    // Visualize hands for BlackJack
    std::cout << "\nYour hand: ";
    for (int i = 0; i < aPlayerCardCount; ++i) {
        int suit = aPlayerHand[i] / 13;
        int rank = (aPlayerHand[i] % 13) + 1;
        std::cout << CARD_RANKS[rank] << CARD_SUITS[suit] << " ";
    }
    std::cout << "(Value: " << GetHandValue(const_cast<std::array<int, 12>&>(aPlayerHand), aPlayerCardCount) << ")\n";
    std::cout << "Dealer's hand: ";
    if (aRevealDealer) {
        for (int i = 0; i < aDealerCardCount; ++i) {
            int suit = aDealerHand[i] / 13;
            int rank = (aDealerHand[i] % 13) + 1;
            std::cout << CARD_RANKS[rank] << CARD_SUITS[suit] << " ";
        }
        std::cout << "(Value: " << GetHandValue(const_cast<std::array<int, 12>&>(aDealerHand), aDealerCardCount) << ")";
    } else {
        if (aDealerCardCount > 0) {
            int suit = aDealerHand[0] / 13;
            int rank = (aDealerHand[0] % 13) + 1;
            std::cout << CARD_RANKS[rank] << CARD_SUITS[suit] << " ";
            for (int i = 1; i < aDealerCardCount; ++i) {
                std::cout << "[Hidden] ";
            }
        }
        std::cout << "(Value: " << GetCardValue(aDealerHand[0]) << " + ?)";
    }
    std::cout << "\n";
}

int EvaluatePokerHand(const int hand[], int handSize) {
    int rankCounts[14] = {0};
    int suitCounts[4] = {0};
    for (int i = 0; i < handSize; ++i) {
        int rank = (hand[i] % 13) + 1;
        int suit = hand[i] / 13;
        rankCounts[rank]++;
        suitCounts[suit]++;
    }
    // Check for flush
    bool flush = false;
    for (int i = 0; i < 4; ++i) if (suitCounts[i] == handSize) flush = true;
    // Check for straight
    int consecutive = 0, maxConsecutive = 0;
    for (int r = 1; r <= 13; ++r) {
        if (rankCounts[r]) consecutive++;
        else consecutive = 0;
        if (consecutive > maxConsecutive) maxConsecutive = consecutive;
    }
    bool straight = (maxConsecutive == handSize);
    // Count pairs/triples/quads
    int pairs = 0, triples = 0, quads = 0;
    for (int r = 1; r <= 13; ++r) {
        if (rankCounts[r] == 2) pairs++;
        if (rankCounts[r] == 3) triples++;
        if (rankCounts[r] == 4) quads++;
    }
    // Return hand strength (higher is better)
    if (straight && flush) return 8; // Straight flush
    if (quads) return 7; // Four of a kind
    if (triples && pairs) return 6; // Full house
    if (flush) return 5;
    if (straight) return 4;
    if (triples) return 3;
    if (pairs == 2) return 2; // Two pair
    if (pairs == 1) return 1; // One pair
    return 0; // High card
}
