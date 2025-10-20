#pragma once
#include <array>
#include <random>
#include <string>
#include "CasinoHelpers.h"

class SlotMachineGame
{
public:
    SlotMachineGame();

    int GetWinnings() const;

    static int GetTotalWins();

    static int GetTotalLosses();

    static int GetPayoutMultiplierTwoOfAKind();

    static int GetPayoutMultiplierJackpot();
    
    CasinoHelpers::GameState Play(std::mt19937& aGenerator,
                                  int& somePlayerMoney,
                                  int& aPlayerBet,
                                  CasinoHelpers::StatHistory& aStatHistory,
                                  const std::string& aPlayerName);

private:
    int myWinnings;
    static int ourTotalWins;
    static int ourTotalLosses;
    static const int ourPayoutMultiplierTwoOfAKind;
    static const int ourPayoutMultiplierJackpot;

};
