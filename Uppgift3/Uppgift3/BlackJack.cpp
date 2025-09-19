#include "CasinoHelpers.h"
#include "BlackJack.h"
#include <array>
#include <random>
#include <iostream>
#include <Windows.h>
#include <string>
using namespace CasinoHelpers;

namespace BlackJack
{

    static const char* RANKS[14] = { "", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
    static const char* SUITS[4] = { "S", "H", "D", "C" };

    std::array<int, 52> CreateDeck()
    {
        std::array<int, 52> deck{ };
        for (int i = 0; i < 52; ++i)
        {
            deck[i] = i;
        }
        return deck;
    }

    void ShuffleDeck(std::array<int, 52>& aDeck, std::mt19937& aGenerator)
    {
        for (int i = static_cast<int>(aDeck.size()) - 1; i > 0; i--)
        {
            std::uniform_int_distribution<int> distribution(0, i);
            int j = distribution(aGenerator);
            int tempVar = aDeck[i];
            aDeck[i] = aDeck[j];
            aDeck[j] = tempVar;
        }
    }

    int GetCardValue(int aCardIndex)
    {
        int rank = (aCardIndex % 13) + 1;
        if (rank == 13)
        {
            rank = 11;
        }
        else if (rank > 9)
        {
            rank = 10;
        }
        else
        {
            rank++;
        }
        return rank;
    }

    int GetHandValue(std::array<int, 12>& aHand, int aCardCount)
    {
        int total = 0;
        int aceCount = 0;
        for (int i = 0; i < aCardCount; i++)
        {
            int cardValue = GetCardValue(aHand[i]);
            total += cardValue;
            if (cardValue == 11)
            {
                aceCount++;
            }
        }
        while (total > 21 && aceCount > 0)
        {
            total -= 10;
            aceCount--;
        }
        return total;
    }

    int DealOneCard(const std::array<int, 52>& aDeck, int& aDeckTop)
    {
        int card = aDeck[aDeckTop];
        ++aDeckTop;
        return card;
    }

    std::string CardToText(int aCardIndex)
    {
        int suit = aCardIndex / 13;
        int rank = (aCardIndex % 13) + 1;
        return std::string(RANKS[rank]) + SUITS[suit];
    }

    void ShowHands(const std::array<int, 12>& aPlayerHand, int aPlayerCardCount, const std::array<int, 12>& aDealerHand, int aDealerCardCount, bool aRevealDealer, int somePlayerMoney, const std::array<signed int, 5>& aStatHistory)
    {
        DrawHUD(somePlayerMoney, aStatHistory);
        std::cout << "\nYour hand: ";
        for (int i = 0; i < aPlayerCardCount; ++i)
        {
            std::cout << CardToText(aPlayerHand[i]) << " ";
        }
        std::cout << "(Value: " << GetHandValue(const_cast<std::array<int, 12>&>(aPlayerHand), aPlayerCardCount) << ")\n";
        std::cout << "Dealer's hand: ";
        if (aRevealDealer)
        {
            for (int i = 0; i < aDealerCardCount; ++i)
            {
                std::cout << CardToText(aDealerHand[i]) << " ";
            }
            std::cout << "(Value: " << GetHandValue(const_cast<std::array<int, 12>&>(aDealerHand), aDealerCardCount) << ")";
        }
        else
        {
            if (aDealerCardCount > 0)
            {
                std::cout << CardToText(aDealerHand[0]) << " ";
                for (int i = 1; i < aDealerCardCount; ++i)
                {
                    std::cout << "[Hidden] ";
                }
            }
            std::cout << "(Value: " << GetCardValue(aDealerHand[0]) << " + ?)";

        }
        std::cout << "\n";
    }

    GameState PlayBlackJack(std::mt19937& aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsBlackJack, std::array<signed int, 5>& aStatHistory)
    {
        int seeInstructions = GetInput(
            CHOICE_NO, CHOICE_YES,
            "Do you want instructions? (0: No, 1: Yes)",
            "Don't test me, choose 0 for No or 1 for Yes"
        );
        if (seeInstructions)
        {
            ShowInstructions(GameState::BlackJack);
        }
        int acceptRules = GetInput(
            CHOICE_NO, CHOICE_YES,
            "Do you still want to play knowing the stakes? (0: No, 1: Yes)",
            "Give me a real answer. 0 for No, 1 for Yes."
        );
        if (!acceptRules)
        {
            return GameState::Menu;
        }
        int playAgain = PLAY_AGAIN_YES;
        int lossStreak = 0;
        int winCounter = 0;
        while (playAgain)
        {
            std::array<int, 52> deck = CreateDeck();
            ShuffleDeck(deck, aGenerator);
            std::array<int, 12> player; int playerCount = 0;
            std::array<int, 12> dealer; int dealerCount = 0;
            int topCard = 0;
            player[playerCount++] = DealOneCard(deck, topCard);
            dealer[dealerCount++] = DealOneCard(deck, topCard);
            player[playerCount++] = DealOneCard(deck, topCard);
            dealer[dealerCount++] = DealOneCard(deck, topCard);
            if (somePlayerMoney <= 0)
            {
                return HandleBankruptcy(somePlayerMoney, aStatHistory);
            }
            if (RecognizePlayer(GameState::BlackJack, 0, 0, someWinningsBlackJack, 0, 0))
            {
                return GameState::Menu;
            }
            DrawHUD(somePlayerMoney, aStatHistory);
            Bet(somePlayerMoney, aPlayerBet);
            bool ongoingRound = true;
            bool revealDealerCard = false;
            while (ongoingRound && GetHandValue(player, playerCount) < 21)
            {
                ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard, somePlayerMoney, aStatHistory);
                int guess = GetInput(
                    CHOICE_NO, CHOICE_YES,
                    "Whisper it to me, pal: stay (0) or hit (1)",
                    "It ain't too hard, only two choices - 0 to stay, 1 to hit."
                );
                if (guess)
                {
                    player[playerCount++] = DealOneCard(deck, topCard);
                    if (GetHandValue(player, playerCount) > 21)
                    {
                        revealDealerCard = true;
                        ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard, somePlayerMoney, aStatHistory);
                        ongoingRound = false;
                    }
                }
                else
                {
                    revealDealerCard = true;
                    break;
                }
            }
            if (ongoingRound)
            {
                revealDealerCard = true;
                ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard, somePlayerMoney, aStatHistory);
                while (GetHandValue(dealer, dealerCount) < 17)
                {
                    dealer[dealerCount++] = DealOneCard(deck, topCard);
                    ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard, somePlayerMoney, aStatHistory);
                }
                ongoingRound = false;
            }
            if (GetHandValue(player, playerCount) > 21)
            {
                ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard, somePlayerMoney, aStatHistory);
                winCounter = 0;
                ++lossStreak;
                UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
                DrawHUD(somePlayerMoney, aStatHistory);
                someWinningsBlackJack -= aPlayerBet;
                std::cout << "You busted" << "\n";
                std::cout << "You have lost: -" << aPlayerBet << ".\n";
                aPlayerBet = 0;
                system("pause");
                if (somePlayerMoney <= 0)
                {
                    return HandleBankruptcy(somePlayerMoney, aStatHistory);
                }
            }
            else if (GetHandValue(dealer, dealerCount) > 21)
            {
                ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard, somePlayerMoney, aStatHistory);
                lossStreak = 0;
                ++winCounter;
                int payout = aPlayerBet * PAYOUT_MULTIPLIER;
                someWinningsBlackJack += payout;
                HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
                DrawHUD(somePlayerMoney, aStatHistory);
                UpdatePlayerStatHistory(aStatHistory, payout);
                std::cout << "The Dealer bust you win.""\n";
                std::cout << GetWinTaunt(winCounter);
                std::cout << "You win: +" << (payout - aPlayerBet) << ".\n";
            }
            else
            {
                ShowHands(player, playerCount, dealer, dealerCount, revealDealerCard, somePlayerMoney, aStatHistory);
                if (GetHandValue(player, playerCount) <= GetHandValue(dealer, dealerCount))
                {
                    winCounter = 0;
                    ++lossStreak;
                    someWinningsBlackJack -= aPlayerBet;
                    UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
                    DrawHUD(somePlayerMoney, aStatHistory);
                    std::cout << "The dealer has a higher hand, you've lost" << "\n";
                    std::cout << "You have lost: -" << aPlayerBet << ".\n";
                    aPlayerBet = 0;
                    system("pause");
                    if (somePlayerMoney <= 0)
                    {
                        return HandleBankruptcy(somePlayerMoney, aStatHistory);
                    }
                }
                else
                {
                    lossStreak = 0;
                    ++winCounter;
                    int payout = aPlayerBet * PAYOUT_MULTIPLIER;
                    someWinningsBlackJack += payout;
                    HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
                    UpdatePlayerStatHistory(aStatHistory, payout);
                    DrawHUD(somePlayerMoney, aStatHistory);
                    std::cout << "You have the higher hand, you win." << "\n";
                    std::cout << GetWinTaunt(winCounter);
                    std::cout << "You win: +" << (payout - aPlayerBet) << ".\n";
                }
            }
            playAgain = GetInput(
                CHOICE_NO, CHOICE_YES,
                "\nYou sticking around this alley? (0: No, 1: Yes): ",
                "Eh-eh, it's binary, hotshot - '0' for No or '1' for Yes."
            );
            if (playAgain)
            {
                DrawHUD(somePlayerMoney, aStatHistory);
                std::cout << "Grease taps the rail. \"Again.\"\n\n";
            }
            else
            {
                std::cout << "\nGrease waves you off. \"Keep your shoes clean.\"\n";
            }
        }
        return GameState::Menu;
    }
}

