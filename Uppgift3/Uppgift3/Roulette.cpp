#include "CasinoHelpers.h"
#include "Roulette.h"
#include <iostream>
#include <random>
#include <array>
using namespace CasinoHelpers;

namespace Roulette
{
    static const int rouletteLayout[37] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 };

    void PrintRouletteCellWithColour(std::string aString, HANDLE aConsoleHandle, WORD someDefaultTextAttributes, std::string aColour)
    {
        WORD chosenAttributes;
        if (aColour == "green")
        {
            chosenAttributes = BACKGROUND_GREEN;
        }
        else if (aColour == "red")
        {
            chosenAttributes = BACKGROUND_RED;
        }
        else if (aColour == "black")
        {
            chosenAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }
        else
        {
            chosenAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        }
        SetConsoleTextAttribute(aConsoleHandle, chosenAttributes);
        std::cout << aString;
        SetConsoleTextAttribute(aConsoleHandle, someDefaultTextAttributes);
    }

    void DisplayRouletteBoard()
    {
        system("cls");
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO screenInfo{};
        WORD defaultTextAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        if (GetConsoleScreenBufferInfo(consoleHandle, &screenInfo))
        {
            defaultTextAttributes = screenInfo.wAttributes;
        }

        for (int i = 0; i < 37; i++)
        {
            std::string cellToPrint = "";
            if (i == 0)
            {
                cellToPrint = "[         " + std::to_string(rouletteLayout[i]) + "        ]\n";
            }
            else if (i % 3 == 0)
            {
                if (i <= 9)
                {
                    cellToPrint = "[  " + std::to_string(rouletteLayout[i]) + " ]\n";
                }
                else
                {
                    cellToPrint = "[ " + std::to_string(rouletteLayout[i]) + " ]\n";

                }
            }
            else if (i < 9)
            {
                cellToPrint = "[  " + std::to_string(rouletteLayout[i]) + "  ]";


            }
            else
            {
                cellToPrint = "[ " + std::to_string(rouletteLayout[i]) + "  ]";
            }
            std::string cellColour = "";

            if (i == 0)
            {
                cellColour = "green";
            }
            else if (i < 11 || (i > 18 && i < 29))
            {
                if (i % 2 == 0)
                {
                    cellColour = "black";
                }
                else
                {
                    cellColour = "red";
                }
            }
            else if (i > 10 && i < 19 || i >28)
            {
                if (i % 2 == 0)
                {
                    cellColour = "red";
                }
                else
                {
                    cellColour = "black";
                }
            }
            PrintRouletteCellWithColour(cellToPrint, consoleHandle, defaultTextAttributes, cellColour);
        }
    }

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
        bool win = false; 
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
            DisplayRouletteBoard();

            int betTypeChoice = GetInput(
                1, 6,
                ("What type of bet do you want to place?\n1. Straight up \n2. Split \n3. Corner \n4. Red / Black \n5. Odd / Even \n6. Column\n\n"),
                "invalid input."
            );

            std::uniform_int_distribution<int> rouletteDist(0, 36);
            int rouletteResult = rouletteDist(aGenerator);


            DisplayRouletteBoard(); 
            switch (betTypeChoice)
            {
            case 1:
                currentBetType = BetType::Straight;
                {
                    int straightGuess = GetInput(
                        0, 36,
                        ("Which number do you want to bet on?"),
                        "invalid input."
                    );
                    win = (straightGuess == rouletteResult);
                }
                break;
            case 2:
                currentBetType = BetType::Split;
                {
                    int splitBet1 = GetInput(
                        0, 36,
                        ("What is the first number you want to bet on?"),
                        "invalid input."
                    );

                    int splitBet2;
                    while (true)
                    {
                        splitBet2 = GetInput(
                            0, 36,
                            ("What is the second number you want to bet on? (must be adjacent to the first number)"),
                            "invalid input."
                        );

                        if (splitBet2 == splitBet1 - 1 || splitBet2 == splitBet1 + 1 ||
                            splitBet2 == splitBet1 - 3 || splitBet2 == splitBet1 + 3)
                        {
                            break;
                        }
                        std::cout << "Invalid choice. The second number must be adjacent to the first number.\n";
                    }

                    win = (splitBet1 == rouletteResult || splitBet2 == rouletteResult);
                }
                break;
            case 3:
                currentBetType = BetType::Corner;
                {
                    int cornerStart;
                    while (true)
                    {
                        cornerStart = GetInput(
                            1, 32,
                            ("Enter the top-left number of the corner (1-32, excluding the rightmost column):"),
                            "invalid input."
                        );

                        if (cornerStart % 3 != 0)
                        {
                            break;
                        }
                        std::cout << "Invalid choice. The top-left number cannot be in the rightmost column.\n";
                    }

                    win = (rouletteResult == cornerStart || rouletteResult == cornerStart + 1 ||
                           rouletteResult == cornerStart + 3 || rouletteResult == cornerStart + 4);
                }
                break;
            case 4:
                currentBetType = BetType::RedOrBlack;
                {
                    int colorChoice = GetInput(
                        0, 1,
                        ("Bet on color (0: Black, 1: Red):"),
                        "invalid input."
                    );
                    bool isRed = (rouletteResult > 0 && ((rouletteResult <= 10 || (rouletteResult >= 19 && rouletteResult <= 28)) ? rouletteResult % 2 != 0 : rouletteResult % 2 == 0));
                    win = (colorChoice == 1 && isRed) || (colorChoice == 0 && !isRed);
                }
                break;
            case 5:
                currentBetType = BetType::OddOrEven;
                {
                    int oddEvenChoice = GetInput(
                        0, 1,
                        ("Bet on (0: Even, 1: Odd):"),
                        "invalid input."
                    );
                    win = (rouletteResult != 0 && ((rouletteResult % 2 == 0 && oddEvenChoice == 0) || (rouletteResult % 2 != 0 && oddEvenChoice == 1)));
                }
                break;
            case 6:
                currentBetType = BetType::Column;
                {
                    int columnChoice = GetInput(
                        1, 3,
                        ("Bet on column (1, 2, or 3):"),
                        "invalid input."
                    );
                    win = (rouletteResult != 0 && (rouletteResult - 1) % 3 == (columnChoice - 1));
                }
                break;
            default:
                currentBetType = BetType::None;
                win = false;
                break;
            }

            DrawHUD(somePlayerMoney, aStatHistory);
            std::cout << "\nThe roulette spin result is: " << rouletteResult << "\n";

            if (win)
            {
                lossStreak = 0;
                ++winCounter;
                int payout = aPlayerBet * PAYOUT_MULTIPLIER;
                HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
                UpdatePlayerStatHistory(aStatHistory, payout);
                someWinningsOddOrEven += payout;
                std::cout << "You win\n";
                std::cout << GetWinTaunt(winCounter);
                std::cout << "He shoves your stack over: +" << (payout - aPlayerBet) << ".\n";
            }
            else if (!win)
            {
                winCounter = 0;
                ++lossStreak;
                UpdatePlayerStatHistory(aStatHistory, -aPlayerBet);
                someWinningsOddOrEven -= aPlayerBet;
                std::cout << "You lose\n";
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

