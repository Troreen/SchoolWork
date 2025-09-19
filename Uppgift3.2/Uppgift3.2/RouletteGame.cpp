#include "RouletteGame.h"
#include <iostream>
#include <Windows.h>
using namespace CasinoHelpers;

namespace {
    static const int rouletteLayout[37] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 };

    void PrintRouletteCellWithColour(const std::string& aString, HANDLE aConsoleHandle, WORD someDefaultTextAttributes, const std::string& aColour)
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
            else if (i > 10 && i < 19 || i > 28)
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
}

int RouletteGame::totalWins = 0;
int RouletteGame::totalLosses = 0;

CasinoHelpers::GameState RouletteGame::play(std::mt19937& generator, int& playerMoney, int& playerBet, std::array<signed int, 5>& statHistory)
{
    int seeInstructions = GetInput(CHOICE_NO, CHOICE_YES, "Would you like to see the instructions? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::Roulette);
    }
    int acceptRules = GetInput(CHOICE_NO, CHOICE_YES, "Do you want to continue? (0: No, 1: Yes)", "Please enter 0 for No or 1 for Yes.");
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
            return HandleBankruptcy(playerMoney, statHistory);
        }
        if (RecognizePlayer(GameState::Roulette, 0, 0, 0, 0, winnings))
        {
            return GameState::Menu;
        }
        DrawHUD(playerMoney, statHistory);
        Bet(playerMoney, playerBet);
        DisplayRouletteBoard();
        std::cout << "Roulette: Choose your bet type.\n";
        std::cout << "1. Straight up\n2. Split\n3. Corner\n4. Red/Black\n5. Odd/Even\n6. Column\n";
        int betTypeChoice = GetInput(1, 6, "Select bet type (1-6):", "Please enter a number between 1 and 6.");
        std::uniform_int_distribution<int> rouletteDist(0, 36);
        int rouletteResult = rouletteDist(generator);
        bool win = false;
        switch (betTypeChoice)
        {
        case 1:
        {
            int straightGuess = GetInput(0, 36, "Enter the number you want to bet on (0-36):", "Please enter a number between 0 and 36.");
            win = (straightGuess == rouletteResult);
            break;
        }
        case 2:
        {
            int splitBet1 = GetInput(0, 36, "Enter the first number (0-36):", "Please enter a number between 0 and 36.");
            int splitBet2 = GetInput(0, 36, "Enter the second number (must be adjacent):", "Please enter a number between 0 and 36.");
            win = (splitBet1 == rouletteResult || splitBet2 == rouletteResult);
            break;
        }
        case 3:
        {
            int cornerStart = GetInput(1, 32, "Enter the top-left number of the corner (1-32):", "Please enter a number between 1 and 32.");
            win = (rouletteResult == cornerStart || rouletteResult == cornerStart + 1 || rouletteResult == cornerStart + 3 || rouletteResult == cornerStart + 4);
            break;
        }
        case 4:
        {
            int colorChoice = GetInput(0, 1, "Bet on color (0: Black, 1: Red):", "Please enter 0 for Black or 1 for Red.");
            bool isRed = (rouletteResult > 0 && ((rouletteResult <= 10 || (rouletteResult >= 19 && rouletteResult <= 28)) ? rouletteResult % 2 != 0 : rouletteResult % 2 == 0));
            win = (colorChoice == 1 && isRed) || (colorChoice == 0 && !isRed);
            break;
        }
        case 5:
        {
            int oddEvenChoice = GetInput(0, 1, "Bet on (0: Even, 1: Odd):", "Please enter 0 for Even or 1 for Odd.");
            win = (rouletteResult != 0 && ((rouletteResult % 2 == 0 && oddEvenChoice == 0) || (rouletteResult % 2 != 0 && oddEvenChoice == 1)));
            break;
        }
        case 6:
        {
            int columnChoice = GetInput(1, 3, "Bet on column (1, 2, or 3):", "Please enter 1, 2, or 3.");
            win = (rouletteResult != 0 && (rouletteResult - 1) % 3 == (columnChoice - 1));
            break;
        }
        default:
            win = false;
            break;
        }
        DrawHUD(playerMoney, statHistory);
        std::cout << "Roulette result: " << rouletteResult << "\n";
        if (win)
        {
            lossStreak = 0;
            ++winCounter;
            int payout = playerBet * PAYOUT_MULTIPLIER;
            HandlePlayerMoney(playerMoney, playerBet, payout);
            UpdatePlayerStatHistory(statHistory, payout);
            winnings += payout;
            ++totalWins;
            std::cout << "You won. Your payout is " << (payout - playerBet) << ".\n";
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(statHistory, -playerBet);
            winnings -= playerBet;
            ++totalLosses;
            std::cout << "You lost. You lose your bet of " << playerBet << ".\n";
            playerBet = 0;
            system("pause");
            if (playerMoney <= 0)
            {
                return HandleBankruptcy(playerMoney, statHistory);
            }
        }
        playAgain = GetInput(CHOICE_NO, CHOICE_YES, "Would you like to play again? (0: No, 1: Yes): ", "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHUD(playerMoney, statHistory);
            std::cout << "Starting a new round.\n\n";
        }
        else
        {
            std::cout << "\nReturning to menu.\n";
        }
    }
    return GameState::Menu;
}
