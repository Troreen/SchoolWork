#include "CasinoHelpers.h"
#include "GuessTheNumber.h"
#include <iostream>
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace GuessTheNumber
{
    GameState PlayGuessTheNumber(std::mt19937& aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsGuessTheNumber, std::array<signed int, 5>& aStatHistory)
    {
        int seeInstructions = GetInput(
            CHOICE_NO, CHOICE_YES,
            "Do you want instructions? (0: No, 1: Yes)",
            "Don't test me, choose 0 for No or 1 for Yes"
        );
        if (seeInstructions)
        {
            ShowInstructions(GameState::GuessTheNumber);
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
            if (somePlayerMoney <= 0)
            {
                return HandleBankruptcy(somePlayerMoney, aStatHistory);
            }
            if (RecognizePlayer(GameState::GuessTheNumber, someWinningsGuessTheNumber, 0, 0, 0, 0))
            {
                return GameState::Menu;
            }
            Bet(somePlayerMoney, aPlayerBet);
            int guess = GetInput(
                2, 12,
                "Lay your wager... what's the sum gonna be?",
                "Heh, don't play me for a fool... pick a number between 2 and 12."
            );
            DrawHUD(somePlayerMoney, aStatHistory);
            std::cout << "Hands off the table. Dice are talking now...";
            int die1 = RollDie(aGenerator);
            int die2 = RollDie(aGenerator);
            int sum = die1 + die2;
            std::cout << "\nThe dice hit the table...\n";
            std::cout << "Die 1: " << die1 << "\n";
            std::cout << "Die 2: " << die2 << "\n";
            std::cout << "Total showing: " << sum << "\n";
            if (guess == sum)
            {
                lossStreak = 0;
                ++winCounter;
                int payout = aPlayerBet * PAYOUT_MULTIPLIER;
                HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
                someWinningsGuessTheNumber += payout;
                UpdatePlayerStatHistory(aStatHistory, payout);
                std::cout << GetWinTaunt(winCounter);
                std::cout << "House peels off a stack and slides it back: +" << (payout - aPlayerBet)
                    << ". Don't get cocky.\n";
            }
            else
            {
                winCounter = 0;
                ++lossStreak;
                std::cout << GetLossTaunt(lossStreak);
                std::cout << "House keeps your stake: -" << aPlayerBet << ". Maybe the next alley's kinder.\n";
                someWinningsGuessTheNumber -= aPlayerBet;
                UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
                aPlayerBet = 0;
                system("pause");
                if (somePlayerMoney <= 0)
                {
                    return HandleBankruptcy(somePlayerMoney, aStatHistory);
                }
            }
            playAgain = GetInput(
                CHOICE_NO, CHOICE_YES,
                "\nYou feelin' bold enough for another round? (0: No, 1: Yes): ",
                "Eh-eh, only '0' for No or '1' for Yes. Try again..."
            );
            if (playAgain)
            {
                DrawHUD(somePlayerMoney, aStatHistory);
                std::cout << "Back for more punishment? I like your style...\n\n";
            }
            else
            {
                std::cout << ("\nSmart. Live to lose another night.");
            }
        }
        return GameState::Menu;
    }
}
