#include "CasinoHelpers.h"
#include "OddOrEven.h"
#include <iostream>
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace OddOrEven
{
    GameState PlayOddOrEven(std::mt19937& aGenerator, int& playerMoney, int& playerBet, int& winningsOddOrEven, std::array<signed int, 5>& globalStatHistory)
    {
        int seeInstructions = GetInput(
                CHOICE_NO, CHOICE_YES,
            "Do you want instructions? (0: No, 1: Yes)",
            "Don't test me, choose 0 for No or 1 for Yes"
        );
        if (seeInstructions)
        {
            ShowInstructions(GameState::OddOrEven);
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
            if (playerMoney <= 0)
            {
                return HandleBankruptcy(playerMoney, globalStatHistory);
            }
            if (RecognizePlayer(GameState::OddOrEven, 0, winningsOddOrEven, 0))
            {
                return GameState::Menu;
            }
            Bet(playerMoney, playerBet);
            int guess = GetInput(
                CHOICE_NO, CHOICE_YES,
                "Whisper it to me, pal: odd (1) or even (2)  -  both bones gotta match",
                "Use your fingers if you gotta - 1 for odd, 2 for even."
            );
        
            DrawHUD(playerMoney, globalStatHistory);

            std::cout << "Hands off the felt. Let the bones breathe...\n";
            int die1 = RollDie(aGenerator);
            int die2 = RollDie(aGenerator);
            std::cout << "\nThe dice kiss concrete...\n";
            std::cout << "Die 1: " << die1 << "\n";
            std::cout << "Die 2: " << die2 << "\n";
            bool bothOdd = (((die1 % 2) != 0) && ((die2 % 2) != 0));
            bool bothEven = (((die1 % 2) == 0) && ((die2 % 2) == 0));
            bool split1 = (((die1 % 2) != 0) && ((die2 % 2) == 0));
            bool split2 = (((die1 % 2) == 0) && ((die2 % 2) != 0));
            bool win = (((guess == 1) && bothOdd) || ((guess == 2) && bothEven));

            if (win)
            {
                lossStreak = 0;
                ++winCounter;
                int payout = playerBet * PAYOUT_MULTIPLIER;
                HandlePlayerMoney(playerMoney, playerBet, payout);
                UpdatePlayerStatHistory(globalStatHistory, payout);
                winningsOddOrEven += payout;
                std::cout << "Grease whistles low: \"" << (bothEven ? "Both even" : "Both odd") << ". You threaded it.\"\n";
                std::cout << GetWinTaunt(winCounter);
                std::cout << "He shoves your stack over: +" << (payout - playerBet) << ".\n";
            }
            else if (split1 || split2)
            {
                winCounter = 0;
                ++lossStreak;
                UpdatePlayerStatHistory(globalStatHistory, -playerBet);
                winningsOddOrEven -= playerBet;
                std::cout << "Grease shrugs: \"Split shoes - one odd, one even. House eats.\"\n";
                std::cout << "He pockets your bet like it owed him money: -" << playerBet << ".\n";
                playerBet = 0;
                system("pause");
                if (playerMoney <= 0)
                {
                    return HandleBankruptcy(playerMoney, globalStatHistory);
                }
            }
            else
            {
                winCounter = 0;
                ++lossStreak;
                UpdatePlayerStatHistory(globalStatHistory, -playerBet);
                winningsOddOrEven -= playerBet;
                std::cout << "Grease shrugs: \"Hah! Not your day is it? House eats.\"\n";
                std::cout << "He pockets your bet like it owed him money: -" << playerBet << ".\n";
                playerBet = 0;
                system("pause");
                if (playerMoney <= 0)
                {
                    return HandleBankruptcy(playerMoney, globalStatHistory);
                }
            }
            playAgain = GetInput(
                CHOICE_NO, CHOICE_YES,
                "\nYou sticking around this alley? (0: No, 1: Yes): ",
                "Eh-eh, it's binary, hotshot - '0' for No or '1' for Yes."
            );
            if (playAgain)
            {
                DrawHUD(playerMoney, globalStatHistory);
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

