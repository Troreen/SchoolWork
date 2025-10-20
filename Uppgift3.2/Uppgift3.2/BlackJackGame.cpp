#include "BlackJackGame.h"
#include <iostream>
#include <array>
#include <random>
#include <cassert>
using namespace CasinoHelpers;

BlackJackGame::BlackJackGame()
    : myWinnings(0)
{
}

int BlackJackGame::ourTotalWins = 0;
int BlackJackGame::ourTotalLosses = 0;
const int BlackJackGame::ourPayoutMultiplier = 2;

std::array<int, 52> BlackJackGame::CreateDeck()
{
    std::array<int, 52> deck{};
    for (int index = 0; index < 52; ++index)
    {
        deck[index] = index;
    }

    return deck;
}

void BlackJackGame::ShuffleDeck(std::array<int, 52>& aDeck, std::mt19937& aGenerator)
{
    for (int index = static_cast<int>(aDeck.size()) - 1; index > 0; --index)
    {
        std::uniform_int_distribution<int> distribution(0, index);
        int chosenIndex = distribution(aGenerator);
        std::swap(aDeck[index], aDeck[chosenIndex]);
    }
}

int BlackJackGame::GetCardValue(int aCardIndex)
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

int BlackJackGame::GetHandValue(const std::array<int, 12>& aHand, int aCardCount)
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

int BlackJackGame::DealOneCard(const std::array<int, 52>& aDeck, int& aDeckTop)
{
    assert(aDeckTop < static_cast<int>(aDeck.size()));
    int dealtCard = aDeck[aDeckTop];
    ++aDeckTop;
    return dealtCard;
}

void BlackJackGame::ShowHands(const std::array<int, 12>& aPlayerHand,
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

int BlackJackGame::GetWinnings() const
{
    return myWinnings;
}

int BlackJackGame::GetTotalWins()
{
    return ourTotalWins;
}

int BlackJackGame::GetTotalLosses()
{
    return ourTotalLosses;
}

int BlackJackGame::GetPayoutMultiplier()
{
    return ourPayoutMultiplier;
}

CasinoHelpers::GameState BlackJackGame::Play(std::mt19937& aGenerator,
                                             int& somePlayerMoney,
                                             int& aPlayerBet,
                                             CasinoHelpers::StatHistory& aStatHistory,
                                             const std::string& aPlayerName)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES, "Would you like to see the instructions? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::BlackJack, playerName);
    }
    int acceptRules = GetInput(CHOICE_NO, CHOICE_YES, "Do you want to continue? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }
    int playAgain = PLAY_AGAIN_YES;
    while (playAgain)
    {
        if (RecognizePlayer(GameState::BlackJack, 0, 0, winnings, 0, 0, playerName))
        {
            return GameState::Menu;
        }
        if (playerMoney <= 0)
        {
            return HandleBankruptcy(playerMoney, statHistory, playerName);
        }
        DrawHUD(playerMoney, statHistory, playerName);
        Bet(playerMoney, playerBet, playerName);
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
            std::cout << playerName << ", you have exceeded 21. You lose your bet of " << playerBet << ".\n";
            winnings -= playerBet;
            ++totalLosses;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            playerBet = 0;
        }
        else if (dealerBust || playerValue > dealerValue)
        {
            payout = playerBet * PAYOUT_MULTIPLIER;
            std::cout << playerName << ", you win. Your payout is " << (payout - playerBet) << ".\n";
            winnings += payout;
            ++totalWins;
            HandlePlayerMoney(playerMoney, playerBet, payout);
            UpdatePlayerStatHistory(statHistory, payout);
        }
        else
        {
            std::cout << "Dealer wins, " << playerName << ". You lose your bet of " << playerBet << ".\n";
            winnings -= playerBet;
            ++totalLosses;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            playerBet = 0;
        }
        playAgain = GetInput(CHOICE_NO, CHOICE_YES, "Would you like to play again? (0: No, 1: Yes): ", "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHUD(playerMoney, statHistory, playerName);
            std::cout << "Starting a new round, " << playerName << ".\\n\\n";
        }
        else
        {
            std::cout << "\\nReturning to menu, " << playerName << ".\\n";
        }
    }
    return GameState::Menu;
}












