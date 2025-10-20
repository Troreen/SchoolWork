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
    void Run();

    const std::array<GuessTheNumberGame, 2>& GetGuessTheNumberTables() const;

    const OddOrEvenGame& GetOddOrEvenGame() const;

    const BlackJackGame& GetBlackJackGame() const;

    const SlotMachineGame& GetSlotMachineGame() const;

    const RouletteGame& GetRouletteGame() const;

    int GetPlayerMoney() const;

    int GetPlayerBet() const;

    const CasinoHelpers::StatHistory& GetStatHistory() const;

private:
    std::array<GuessTheNumberGame, 2> myGuessTheNumberTables;
    OddOrEvenGame myOddOrEvenGame;
    BlackJackGame myBlackJackGame;
    SlotMachineGame mySlotMachineGame;
    RouletteGame myRouletteGame;
    std::string myPlayerName;
    int myPlayerMoney;
    int myPlayerBet;
    CasinoHelpers::StatHistory myStatHistory;
    std::mt19937 myGenerator;

    CasinoHelpers::GameState PlayGuessTheNumber();
    std::string PromptForPlayerName() const;

};

