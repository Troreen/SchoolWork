#include "BlackJackGame.h"

#include <array>
#include <cassert>
#include <iostream>
#include <random>

using namespace CasinoHelpers;

int BlackJackGame::ourTotalWins = 0;
int BlackJackGame::ourTotalLosses = 0;

namespace
{
    std::array<int, 52> CreateDeck()
    {
        std::array<int, 52> deck{};
        for (int index = 0; index < 52; ++index)
        {
            deck[index] = index;
        }

        return deck;
    }

    void ShuffleDeck(std::array<int, 52>& aDeck, std::mt19937& aGenerator)
    {
        for (int index = static_cast<int>(aDeck.size()) - 1; index > 0; --index)
        {
            std::uniform_int_distribution<int> distribution(0, index);
            int chosenIndex = distribution(aGenerator);
            std::swap(aDeck[index], aDeck[chosenIndex]);
        }
    }

    int GetCardValue(int aCardIndex)
    {
        int rank = (aCardIndex % 13) + 1;
        if (rank == 1)
        {
            return 11;
        }

        if (rank > 10)
        {
            return 10;
        }

        return rank;
    }

    int GetHandValue(const std::array<int, 12>& aHand, int aCardCount)
    {
        int totalValue = 0;
        int aceCount = 0;
        for (int index = 0; index < aCardCount; ++index)
        {
            int cardValue = GetCardValue(aHand[index]);
            totalValue += cardValue;
            if (cardValue == 11)
            {
                ++aceCount;
            }
        }

        while (totalValue > 21 && aceCount > 0)
        {
            totalValue -= 10;
            --aceCount;
        }

        return totalValue;
    }

    int DealOneCard(const std::array<int, 52>& aDeck, int& aDeckTop)
    {
        assert(aDeckTop < static_cast<int>(aDeck.size()));
        int dealtCard = aDeck[aDeckTop];
        ++aDeckTop;
        return dealtCard;
    }

    void ShowHands(const std::array<int, 12>& aPlayerHand,
                   int aPlayerCount,
                   const std::array<int, 12>& aDealerHand,
                   int aDealerCount,
                   bool aRevealDealer)
    {
        std::cout << "Player's hand: ";
        for (int index = 0; index < aPlayerCount; ++index)
        {
            std::cout << GetCardValue(aPlayerHand[index]) << ' ';
        }

        std::cout << "(Total: " << GetHandValue(aPlayerHand, aPlayerCount) << ")\n";
        std::cout << "Dealer's hand: ";
        if (aRevealDealer)
        {
            for (int index = 0; index < aDealerCount; ++index)
            {
                std::cout << GetCardValue(aDealerHand[index]) << ' ';
            }

            std::cout << "(Total: " << GetHandValue(aDealerHand, aDealerCount) << ")\n";
        }
        else
        {
            std::cout << GetCardValue(aDealerHand[0]) << " ?\n";
        }
    }
}

CasinoHelpers::GameState BlackJackGame::Play(std::mt19937& aGenerator,
                                             int& somePlayerMoney,
                                             int& aPlayerBet,
                                             std::array<signed int, CasinoHelpers::globalStatHistorySize>& aStatHistory,
                                             const std::string& aPlayerName)
{
    int seeInstructions = GetInput(globalChoiceNo, globalChoiceYes,
                                   "Would you like to see the instructions? (0: No, 1: Yes)",
                                   "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::BlackJack, aPlayerName);
    }

    int acceptRules = GetInput(globalChoiceNo, globalChoiceYes,
                               "Do you want to continue? (0: No, 1: Yes)",
                               "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }

    int playAgain = globalPlayAgainYes;
    while (playAgain)
    {
        if (RecognizePlayer(GameState::BlackJack, 0, 0, myWinnings, 0, 0, aPlayerName))
        {
            return GameState::Menu;
        }

        if (somePlayerMoney <= 0)
        {
            return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
        }

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        Bet(somePlayerMoney, aPlayerBet, aPlayerName);

        std::array<int, 52> deck = CreateDeck();
        ShuffleDeck(deck, aGenerator);
        int deckTop = 0;
        std::array<int, 12> playerHand{};
        std::array<int, 12> dealerHand{};
        int playerCount = 0;
        int dealerCount = 0;

        playerHand[playerCount++] = DealOneCard(deck, deckTop);
        dealerHand[dealerCount++] = DealOneCard(deck, deckTop);
        playerHand[playerCount++] = DealOneCard(deck, deckTop);
        dealerHand[dealerCount++] = DealOneCard(deck, deckTop);

        const int betAmount = aPlayerBet;
        bool playerBust = false;
        bool dealerBust = false;
        bool playerStand = false;
        while (!playerBust && !playerStand)
        {
            ShowHands(playerHand, playerCount, dealerHand, dealerCount, false);
            int playerHandValue = GetHandValue(playerHand, playerCount);
            if (playerHandValue > 21)
            {
                playerBust = true;
                break;
            }

            int action = GetInput(1, 2,
                                  "Choose your action: 1) Hit (draw a card) 2) Stand (end your turn)",
                                  "Please enter 1 for Hit or 2 for Stand.");
            if (action == 1)
            {
                playerHand[playerCount++] = DealOneCard(deck, deckTop);
            }
            else
            {
                playerStand = true;
            }
        }

        if (!playerBust)
        {
            while (GetHandValue(dealerHand, dealerCount) < 17)
            {
                dealerHand[dealerCount++] = DealOneCard(deck, deckTop);
            }

            if (GetHandValue(dealerHand, dealerCount) > 21)
            {
                dealerBust = true;
            }
        }

        ShowHands(playerHand, playerCount, dealerHand, dealerCount, true);
        int playerValue = GetHandValue(playerHand, playerCount);
        int dealerValue = GetHandValue(dealerHand, dealerCount);
        int roundResult = 0;
        if (playerBust)
        {
            roundResult = -betAmount;
            myWinnings -= betAmount;
            ++ourTotalLosses;
            aPlayerBet = 0;
            std::cout << aPlayerName << ", you bust! Dealer wins this round. You lose " << betAmount << " chips.\n";
        }
        else if (dealerBust || playerValue > dealerValue)
        {
            int payout = betAmount * ourPayoutMultiplier;
            HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
            roundResult = payout;
            myWinnings += payout;
            ++ourTotalWins;
            std::cout << aPlayerName << ", you win " << (payout - betAmount) << " chips!\n";
        }
        else if (playerValue == dealerValue)
        {
            HandlePlayerMoney(somePlayerMoney, aPlayerBet, aPlayerBet);
            roundResult = 0;
            std::cout << "It's a push. Bets are returned.\n";
        }
        else
        {
            roundResult = -betAmount;
            myWinnings -= betAmount;
            ++ourTotalLosses;
            std::cout << "Dealer wins this round. You lose " << betAmount << " chips.\n";
            aPlayerBet = 0;
        }

        UpdatePlayerStatHistory(aStatHistory, roundResult);

        playAgain = GetInput(globalPlayAgainNo, globalPlayAgainYes,
                             "Would you like to play again? (0: No, 1: Yes): ",
                             "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
            std::cout << "Shuffling for another round...\n\n";
        }
        else
        {
            std::cout << "\nReturning to menu.\n";
        }
    }

    return GameState::Menu;
}
