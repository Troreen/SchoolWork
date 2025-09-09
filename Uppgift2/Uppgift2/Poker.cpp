#include "BlackJack.h"
#include "CasinoHelpers.h"
#include <array>
#include <random>
#include <iostream>
#include <string>

// Constants
const int NUM_PLAYERS = 4;
const int HOLE_CARDS = 2;
const int BOARD_CARDS = 5;
const int SMALL_BLIND = 20;
const int BIG_BLIND = 40;
const int RAISE_SIZE = 40;
const int RAISE_CAP = 3;

// MVP: Only high card showdown, no raises yet
GameState PlayPoker(std::mt19937& aGenerator, int chips[NUM_PLAYERS], int& dealerIndex, std::array<signed int, 5>& globalStatHistory)
{
    // Per-hand state (reset only ONCE per hand)
    int betThisRound[NUM_PLAYERS] = {0};
    int isFolded[NUM_PLAYERS] = {0};
    int holes[NUM_PLAYERS][HOLE_CARDS];
    int board[BOARD_CARDS] = {0};
    int basePot = 0;
    int currentBet = 0;
    int raisesThisRound = 0;
    int topCard = 0;
    std::array<int, 52> deck = CreateDeck();
    ShuffleDeck(deck, aGenerator);

    // Advance dealer
    dealerIndex = (dealerIndex + 1) % NUM_PLAYERS;

    // Deal hole cards ONCE per hand
    for (int p = 0; p < NUM_PLAYERS; ++p) {
        holes[p][0] = DealOneCard(deck, topCard);
        holes[p][1] = DealOneCard(deck, topCard);
        isFolded[p] = 0;
        betThisRound[p] = 0;
    }
    for (int c = 0; c < BOARD_CARDS; ++c) board[c] = 0;
    basePot = 0;
    currentBet = 0;
    raisesThisRound = 0;
    topCard = 0;

    // Post blinds
    int sb = (dealerIndex + 1) % NUM_PLAYERS;
    int bb = (dealerIndex + 2) % NUM_PLAYERS;
    betThisRound[sb] = SMALL_BLIND;
    betThisRound[bb] = BIG_BLIND;
    chips[sb] -= SMALL_BLIND;
    chips[bb] -= BIG_BLIND;
    currentBet = BIG_BLIND;
    raisesThisRound = 0;

    // Betting rounds: Preflop, Flop, Turn, River
    int street = 0;
    int boardDeal[4] = {0, 3, 1, 1}; // Flop:3, Turn:1, River:1
    int boardPos[4] = {0, 0, 3, 4};
    int handEnded = 0;
    for (street = 0; street < 4 && !handEnded; ++street) {
        // Move bets to pot
        for (int p = 0; p < NUM_PLAYERS; ++p) basePot += betThisRound[p];
        for (int p = 0; p < NUM_PLAYERS; ++p) betThisRound[p] = 0;
        currentBet = 0;
        raisesThisRound = 0;
        // Deal board cards
        for (int c = 0; c < boardDeal[street]; ++c) {
            board[boardPos[street] + c] = DealOneCard(deck, topCard);
        }
        // Betting round
        int activePlayers = 0;
        for (int p = 0; p < NUM_PLAYERS; ++p) if (!isFolded[p] && chips[p] > 0) ++activePlayers;
        if (activePlayers == 1) { handEnded = 1; break; }
        int firstToAct = (street == 0) ? (bb + 1) % NUM_PLAYERS : (dealerIndex + 1) % NUM_PLAYERS;
        int acted[NUM_PLAYERS] = {0};
        int lastAggressor = -1;
        int stableCount = 0;
        int p = firstToAct;
        while (stableCount < activePlayers) {
            if (!isFolded[p] && chips[p] > 0) {
                std::cout << "\n========================================\n";
                std::cout << "--- Player " << p << "'s turn ---\n";
                std::cout << "Chips: " << chips[p] << "\n";
                int toCall = currentBet - betThisRound[p];
                int visiblePot = basePot;
                for (int q = 0; q < NUM_PLAYERS; ++q) visiblePot += betThisRound[q];
                DrawHUD(chips[p], globalStatHistory, p); // Show chips and current player in HUD
                std::cout << "Pot: " << visiblePot << "\n";
                std::cout << "Board: ";
                for (int c = 0; c < BOARD_CARDS; ++c) if (board[c]) std::cout << CardToText(board[c]) << " ";
                std::cout << "\nYour hand: " << CardToText(holes[p][0]) << " " << CardToText(holes[p][1]) << "\n";
                std::cout << "To call: " << toCall << "\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                if (toCall == 0) {
                    int action = GetInput(0, 1, "Check (0) or Fold (1)?", "Choose 0 to check or 1 to fold.");
                    if (action == 1) { isFolded[p] = 1; --activePlayers; }
                    stableCount++;
                } else {
                    int action = GetInput(0, 1, "Call (0) or Fold (1)?", "Choose 0 to call or 1 to fold.");
                    if (action == 1) { isFolded[p] = 1; --activePlayers; }
                    else {
                        betThisRound[p] += toCall;
                        chips[p] -= toCall;
                    }
                    stableCount++;
                }
                if (activePlayers == 1) { handEnded = 1; break; }
            }
            p = (p + 1) % NUM_PLAYERS;
        }
    }
    // Showdown
    int winners[NUM_PLAYERS] = {0};
    int bestRank = -1;
    for (int p = 0; p < NUM_PLAYERS; ++p) {
        if (!isFolded[p] && chips[p] >= 0) {
            // MVP: High card only
            int ranks[7];
            ranks[0] = holes[p][0] % 13 + 2;
            ranks[1] = holes[p][1] % 13 + 2;
            int idx = 2;
            for (int c = 0; c < BOARD_CARDS; ++c) ranks[idx++] = board[c] % 13 + 2;
            int highestRank = ranks[0];
            for (int i = 1; i < 7; ++i) if (ranks[i] > highestRank) highestRank = ranks[i];
            if (highestRank > bestRank) {
                bestRank = highestRank;
                for (int q = 0; q < NUM_PLAYERS; ++q) winners[q] = 0;
                winners[p] = 1;
            } else if (highestRank == bestRank) {
                winners[p] = 1;
            }
        }
    }
    // Award pot
    int pot = basePot;
    for (int p = 0; p < NUM_PLAYERS; ++p) pot += betThisRound[p];
    int numWinners = 0;
    for (int p = 0; p < NUM_PLAYERS; ++p) if (winners[p]) ++numWinners;
    int winAmount = (numWinners > 0) ? pot / numWinners : 0;
    for (int p = 0; p < NUM_PLAYERS; ++p) if (winners[p]) chips[p] += winAmount;
    // Show results
    std::cout << "\nShowdown!\n";
    for (int p = 0; p < NUM_PLAYERS; ++p) {
        std::cout << "Player " << p << (isFolded[p] ? " (folded)" : "") << ": ";
        std::cout << CardToText(holes[p][0]) << " " << CardToText(holes[p][1]);
        if (!isFolded[p]) std::cout << " | High card: " << bestRank;
        if (winners[p]) std::cout << " | Winner! +" << winAmount;
        std::cout << "\n";
    }
    std::cout << "Pot was: " << pot << "\n";
    // Play again?
    int playAgain = GetInput(CHOICE_NO, CHOICE_YES, "Play another hand? (0: No, 1: Yes)", "Choose 0 or 1.");
    if (playAgain) return GameState::Poker;
    else return GameState::Menu;
}
