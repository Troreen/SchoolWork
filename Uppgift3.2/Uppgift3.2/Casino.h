#pragma once
#include <random>
#include <array>
#include <string>
#include "GuessTheNumber.h"
#include "OddOrEven.h"
#include "BlackJackGame.h"
#include "SlotMachine.h"
#include "Roulette.h"
#include "CasinoHelpers.h"

class Casino
{
private:
    std::array<GuessTheNumberGame, 2> guessTheNumberTables;
    OddOrEvenGame oddOrEvenGame;
    BlackJackGame blackJackGame;
    SlotMachineGame slotMachineGame;
    RouletteGame rouletteGame;
    std::string playerName;
    int playerMoney;
    int playerBet;
    std::array<signed int, 5> statHistory;
    std::mt19937 generator;

    CasinoHelpers::GameState playGuessTheNumber();
    std::string promptForPlayerName() const;

public:
    Casino();
    void run();
    const std::array<GuessTheNumberGame, 2>& getGuessTheNumberTables() const { return guessTheNumberTables; }
    const OddOrEvenGame& getOddOrEvenGame() const { return oddOrEvenGame; }
    const BlackJackGame& getBlackJackGame() const { return blackJackGame; }
    const SlotMachineGame& getSlotMachineGame() const { return slotMachineGame; }
    const RouletteGame& getRouletteGame() const { return rouletteGame; }
    int getPlayerMoney() const { return playerMoney; }
    int getPlayerBet() const { return playerBet; }
    const std::array<signed int, 5>& getStatHistory() const { return statHistory; }
};

