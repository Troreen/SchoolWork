#include "BlackJackGame.h"
#include <iostream>
#include <array>
#include <random>
#include <cassert>
using namespace CasinoHelpers;

int BlackJackGame::totalWins = 0;
int BlackJackGame::totalLosses = 0;

namespace {
    std::array<int, 52> createDeck()
    {
        std::array<int, 52> deck{};
        for (int i = 0; i < 52; ++i)
        {
            deck[i] = i;
        }
        return deck;
    }

    void shuffleDeck(std::array<int, 52>& deck, std::mt19937& generator)
    {
        for (int i = static_cast<int>(deck.size()) - 1; i > 0; i--)
        {
            std::uniform_int_distribution<int> distribution(0, i);
            int j = distribution(generator);
            std::swap(deck[i], deck[j]);
        }
    }

    int getCardValue(int cardIndex)
    {
        int rank = (cardIndex % 13) + 1;
        if (rank == 1)
            return 11; // Ace
        if (rank > 10)
            return 10; // Face cards
        return rank;
    }

    int getHandValue(const std::array<int, 12>& hand, int cardCount)
    {
        int total = 0;
        int aceCount = 0;
        for (int i = 0; i < cardCount; i++)
        {
            int value = getCardValue(hand[i]);
            total += value;
            if (value == 11)
                aceCount++;
        }
        while (total > 21 && aceCount > 0)
        {
            total -= 10;
            aceCount--;
        }
        return total;
    }

    int dealOneCard(const std::array<int, 52>& deck, int& deckTop)
    {
        assert(deckTop < static_cast<int>(deck.size()));
        return deck[deckTop++];
    }

    void showHands(const std::array<int, 12>& player, int playerCount, const std::array<int, 12>& dealer, int dealerCount, bool revealDealer)
    {
        std::cout << "Player's hand: ";
        for (int i = 0; i < playerCount; ++i)
        {
            std::cout << getCardValue(player[i]) << ' ';
        }
        std::cout << "(Total: " << getHandValue(player, playerCount) << ")\n";
        std::cout << "Dealer's hand: ";
        if (revealDealer)
        {
            for (int i = 0; i < dealerCount; ++i)
            {
                std::cout << getCardValue(dealer[i]) << ' ';
            }
            std::cout << "(Total: " << getHandValue(dealer, dealerCount) << ")\n";
        }
        else
        {
            std::cout << getCardValue(dealer[0]) << " ?\n";
        }
    }
}

CasinoHelpers::GameState BlackJackGame::play(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES, "Would you like to see the instructions? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::BlackJack);
    }
    int acceptRules = GetInput(CHOICE_NO, CHOICE_YES, "Do you want to continue? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }
    int playAgain = PLAY_AGAIN_YES;
    while (playAgain)
    {
        if (RecognizePlayer(GameState::BlackJack, 0, 0, winnings, 0, 0))
        {
            return GameState::Menu;
        }
        if (playerMoney <= 0)
        {
            return HandleBankruptcy(playerMoney, statHistory);
        }
        DrawHUD(playerMoney, statHistory);
        Bet(playerMoney, playerBet);
        std::array<int, 52> deck = createDeck();
        shuffleDeck(deck, generator);
        int deckTop = 0;
        std::array<int, 12> playerHand{};
        std::array<int, 12> dealerHand{};
        int playerCount = 0, dealerCount = 0;
        playerHand[playerCount++] = dealOneCard(deck, deckTop);
        dealerHand[dealerCount++] = dealOneCard(deck, deckTop);
        playerHand[playerCount++] = dealOneCard(deck, deckTop);
        dealerHand[dealerCount++] = dealOneCard(deck, deckTop);
        bool playerBust = false;
        bool dealerBust = false;
        bool playerStand = false;
        while (!playerBust && !playerStand)
        {
            showHands(playerHand, playerCount, dealerHand, dealerCount, false);
            int handValue = getHandValue(playerHand, playerCount);
            if (handValue > 21)
            {
                playerBust = true;
                break;
            }
            int action = GetInput(1, 2, "Choose your action: 1) Hit (draw a card) 2) Stand (end your turn)", "Please enter 1 for Hit or 2 for Stand.");
            if (action == 1)
            {
                playerHand[playerCount++] = dealOneCard(deck, deckTop);
            }
            else
            {
                playerStand = true;
            }
        }
        if (!playerBust)
        {
            while (getHandValue(dealerHand, dealerCount) < 17)
            {
                dealerHand[dealerCount++] = dealOneCard(deck, deckTop);
            }
            if (getHandValue(dealerHand, dealerCount) > 21)
            {
                dealerBust = true;
            }
        }
        showHands(playerHand, playerCount, dealerHand, dealerCount, true);
        int playerValue = getHandValue(playerHand, playerCount);
        int dealerValue = getHandValue(dealerHand, dealerCount);
        int payout = 0;
        if (playerBust)
        {
            std::cout << "You have exceeded 21. You lose your bet of " << playerBet << ".\n";
            winnings -= playerBet;
            ++totalLosses;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            playerBet = 0;
        }
        else if (dealerBust || playerValue > dealerValue)
        {
            payout = playerBet * PAYOUT_MULTIPLIER;
            std::cout << "You win. Your payout is " << (payout - playerBet) << ".\n";
            winnings += payout;
            ++totalWins;
            HandlePlayerMoney(playerMoney, playerBet, payout);
            UpdatePlayerStatHistory(statHistory, payout);
        }
        else if (playerValue == dealerValue)
        {
            std::cout << "It's a tie. Your bet is returned.\n";
            HandlePlayerMoney(playerMoney, playerBet, playerBet);
            UpdatePlayerStatHistory(statHistory, 0);
        }
        else
        {
            std::cout << "Dealer wins. You lose your bet of " << playerBet << ".\n";
            winnings -= playerBet;
            ++totalLosses;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            playerBet = 0;
        }
        playAgain = GetInput(CHOICE_NO, CHOICE_YES, "Would you like to play again? (0: No, 1: Yes): ", "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHUD(playerMoney, statHistory);
            std::cout << "Starting a new round.\n\n";
        }
        else
        {
            std::cout << "\nReturning to menu.\n";
        }
    }
    return GameState::Menu;
}
