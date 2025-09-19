#pragma once
#include <random>
#include <array>
#include "GuessTheNumberGame.h"
#include "OddOrEvenGame.h"
#include "BlackJackGame.h"
#include "SlotMachineGame.h"
#include "RouletteGame.h"
#include "CasinoHelpers.h"

class Casino
{
private:
    GuessTheNumberGame guessTheNumberGame;
    OddOrEvenGame oddOrEvenGame;
    BlackJackGame blackJackGame;
    SlotMachineGame slotMachineGame;
    RouletteGame rouletteGame;
    int playerMoney;
    int playerBet;
    std::array<signed int, 5> statHistory;
    std::mt19937 generator;
public:
    Casino();
    void run();
    const GuessTheNumberGame& getGuessTheNumberGame() const { return guessTheNumberGame; }
    const OddOrEvenGame& getOddOrEvenGame() const { return oddOrEvenGame; }
    const BlackJackGame& getBlackJackGame() const { return blackJackGame; }
    const SlotMachineGame& getSlotMachineGame() const { return slotMachineGame; }
    const RouletteGame& getRouletteGame() const { return rouletteGame; }
    int getPlayerMoney() const { return playerMoney; }
    int getPlayerBet() const { return playerBet; }
    const std::array<signed int, 5>& getStatHistory() const { return statHistory; }
};
