#include "CasinoHelpers.h"
#include "Roulette.h"
#include <iostream>
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace Roulette
{
    enum class BetType
    {
        StraightUp,
        Split,
        Street,
        Corner,
        Line,
        RedOrBlack,
        OddOrEven,
        Half,
        Dozen,
        Column,
        None
    };


    GameState Roulette(std::mt19937& aGenerator, int& somePlayerMoney, int& aPlayerBet, int& someWinningsOddOrEven, std::array<signed int, 5>& aStatHistory)
    {
        BetType currentBetType = BetType::None;
        int seeInstructions = GetInput(
            CHOICE_NO, CHOICE_YES,
            "Do you want instructions? (0: No, 1: Yes)",
            "Don't test me, choose 0 for No or 1 for Yes"
        );
        if (seeInstructions)
        {
            ShowInstructions(GameState::Roulette);
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
            if (RecognizePlayer(GameState::Roulette, 0, 0, 0, 0, 0))
            {
                return GameState::Menu;
            }
            Bet(somePlayerMoney, aPlayerBet);
            int guess = GetInput(
                1, 10,
                ("What type of bet do you want to place?\n1. Straight up \n2. Split \n3. Street \n4. Corner \n5. Line \n6. Red / Black \n7. Odd / Even \n8. Half \n9. Dozen \n10. Column"),
                "invalid input."
            );
            switch (guess)
            {
            case 1:
                currentBetType = BetType::StraightUp;
                break;
            case 2:
                currentBetType = BetType::Split;
            case 3:
                currentBetType = BetType::Street;
            case 4:
                currentBetType = BetType::Corner;
            case 5:
                currentBetType = BetType::Line;
            case 6:
                currentBetType = BetType::RedOrBlack;
            case 7:
                currentBetType = BetType::OddOrEven;
            case 8:
                currentBetType = BetType::Half;
            case 9:
                currentBetType = BetType::Dozen;
            case 10:
                currentBetType = BetType::Column;

            default:
                break;
            }
            DrawHUD(somePlayerMoney, aStatHistory);

            //====================
            //Game Logc
            //====================
            

            //====================
            //Lose Win handling
            //====================
            if (win)
            {
                lossStreak = 0;
                ++winCounter;
                int payout = aPlayerBet * PAYOUT_MULTIPLIER;
                HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
                UpdatePlayerStatHistory(aStatHistory, payout);
                someWinningsOddOrEven += payout;
                std::cout << "Grease whistles low: \"" << (bothEven ? "Both even" : "Both odd") << ". You threaded it.\"\n";
                std::cout << GetWinTaunt(winCounter);
                std::cout << "He shoves your stack over: +" << (payout - aPlayerBet) << ".\n";
            }
            else if (split1 || split2)
            {
                winCounter = 0;
                ++lossStreak;
                UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
                someWinningsOddOrEven -= aPlayerBet;
                std::cout << "Grease shrugs: \"Split shoes - one odd, one even. House eats.\"\n";
                std::cout << "He pockets your bet like it owed him money: -" << aPlayerBet << ".\n";
                aPlayerBet = 0;
                system("pause");
                if (somePlayerMoney <= 0)
                {
                    return HandleBankruptcy(somePlayerMoney, aStatHistory);
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

