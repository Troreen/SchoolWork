#pragma once
#include <array>
#include <random>
#include <string>
#include "GuessTheNumber.h"
#include "OddOrEven.h"
#include "BlackJackGame.h"
#include "SlotMachine.h"
#include "Roulette.h"
#include "CasinoHelpers.h"

class Casino
{
public:
    Casino();
    void Run();

    const std::array<GuessTheNumberGame, 2>& GetGuessTheNumberTables() const
    {
        return myGuessTheNumberTables;
    }

    const OddOrEvenGame& GetOddOrEvenGame() const
    {
        return myOddOrEvenGame;
    }

    const BlackJackGame& GetBlackJackGame() const
    {
        return myBlackJackGame;
    }

    const SlotMachineGame& GetSlotMachineGame() const
    {
        return mySlotMachineGame;
    }

    const RouletteGame& GetRouletteGame() const
    {
        return myRouletteGame;
    }

    int GetPlayerMoney() const
    {
        return myPlayerMoney;
    }

    int GetPlayerBet() const
    {
        return myPlayerBet;
    }

    const std::array<signed int, CasinoHelpers::globalStatHistorySize>& GetStatHistory() const
    {
        return myStatHistory;
    }

private:
    std::array<GuessTheNumberGame, 2> myGuessTheNumberTables;
    OddOrEvenGame myOddOrEvenGame;
    BlackJackGame myBlackJackGame;
    SlotMachineGame mySlotMachineGame;
    RouletteGame myRouletteGame;
    std::string myPlayerName;
    int myPlayerMoney;
    int myPlayerBet;
    std::array<signed int, CasinoHelpers::globalStatHistorySize> myStatHistory;
    std::mt19937 myGenerator;

    CasinoHelpers::GameState PlayGuessTheNumber();
    std::string PromptForPlayerName() const;

};
