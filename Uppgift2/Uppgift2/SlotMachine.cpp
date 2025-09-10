#include "CasinoHelpers.h"
#include "SlotMachine.h"
#include <random>
#include <iostream>
#include <array>
#include <string>
using namespace CasinoHelpers;

namespace SlotMachine
{
    GameState PlaySlotMachine(std::mt19937& aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsSlot, std::array<signed int, 5>& aStatHistory)
    {
    int seeInstructions = GetInput(
        CHOICE_NO, CHOICE_YES,
        "Do you want instructions? (0: No, 1: Yes)",
        "Don't test me, choose 0 for No or 1 for Yes"
    );
    if (seeInstructions)
    {
        system("cls");
        std::cout << "Slot Machine: Bet, spin, and hope for 3 of a kind!\n";
        std::cout << "Payouts: 3 of a kind = 5x bet, 2 of a kind = 2x bet, else lose your bet.\n\n";
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
    while (playAgain)
    {
        if (RecognizePlayer(GameState::SlotMachine, 0, 0, 0, someWinningsSlot))
        {
            return GameState::Menu;
        }
        if (somePlayerMoney <= 0)
        {
            return HandleBankruptcy(somePlayerMoney, aStatHistory);
        }
        Bet(somePlayerMoney, aPlayerBet);
        DrawHUD(somePlayerMoney, aStatHistory);
        std::cout << "Spinning the reels...\n";
        std::uniform_int_distribution<int> symbolDist(1, 5); 
        int reels[3] = { symbolDist(aGenerator), symbolDist(aGenerator), symbolDist(aGenerator) };
        std::cout << "[ " << reels[0] << " | " << reels[1] << " | " << reels[2] << " ]\n";
        int payout = 0;
        if (reels[0] == reels[1] && reels[1] == reels[2])
        {
                payout = aPlayerBet * PAYOUT_MULTIPLIER_JACKPOT;
            std::cout << "Jackpot! 3 of a kind! You win " << payout << "!\n";
        }
        else if (reels[0] == reels[1] || reels[1] == reels[2] || reels[0] == reels[2])
        {
                payout = aPlayerBet * PAYOUT_MULTIPLIER_TWO_OF_A_KIND;
            std::cout << "Nice! 2 of a kind! You win " << payout << "!\n";
        }
        else
        {
            payout = 0;
            std::cout << "No luck! You lose your bet.\n";
        }
            HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
            UpdatePlayerStatHistory(aStatHistory, payout - aPlayerBet);
        playAgain = GetInput(
            PLAY_AGAIN_NO, PLAY_AGAIN_YES,
            "Spin again? (0: No, 1: Yes): ",
            "Only '0' for No or '1' for Yes. Try again..."
        );
        if (playAgain)
        {
                DrawHUD(somePlayerMoney, aStatHistory);
            std::cout << "The reels are ready for another spin...\n\n";
        }
        else
        {
            std::cout << "\nSmart. Save your luck for another night.";
        }
    }
    return GameState::Menu;
}
}
