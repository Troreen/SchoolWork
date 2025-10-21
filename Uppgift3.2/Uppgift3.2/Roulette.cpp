#include "Roulette.h"

#include <Windows.h>
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace CasinoHelpers;

RouletteGame::RouletteGame()
    : myWinnings(0)
{
}

RouletteGame::~RouletteGame() = default;

const std::array<int, 37> RouletteGame::ourRouletteLayout{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};

void RouletteGame::PrintRouletteCellWithColour(const std::string& aCellText,
    HANDLE aConsoleHandle,
    WORD someDefaultTextAttributes,
    const std::string& aColour)
{
    WORD chosenAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
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

    SetConsoleTextAttribute(aConsoleHandle, chosenAttributes);
    std::cout << aCellText;
    SetConsoleTextAttribute(aConsoleHandle, someDefaultTextAttributes);
}

void RouletteGame::DisplayRouletteBoard()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screenInfo{};
    WORD defaultTextAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    if (GetConsoleScreenBufferInfo(consoleHandle, &screenInfo))
    {
        defaultTextAttributes = screenInfo.wAttributes;
    }

    for (int index = 0; index < static_cast<int>(ourRouletteLayout.size()); ++index)
    {
        std::string cellText;
        if (index == 0)
        {
            cellText = "[         " + std::to_string(ourRouletteLayout[index]) + "        ]\n";
        }
        else if (index % 3 == 0)
        {
            if (index <= 9)
            {
                cellText = "[  " + std::to_string(ourRouletteLayout[index]) + " ]\n";
            }
            else
            {
                cellText = "[ " + std::to_string(ourRouletteLayout[index]) + " ]\n";
            }
        }
        else if (index < 10)
        {
            cellText = "[  " + std::to_string(ourRouletteLayout[index]) + "  ]";
        }
        else
        {
            cellText = "[ " + std::to_string(ourRouletteLayout[index]) + "  ]";
        }

        std::string cellColour;
        if (index == 0)
        {
            cellColour = "green";
        }
        else if (index < 11 || (index > 18 && index < 29))
        {
            cellColour = (index % 2 == 0) ? "black" : "red";
        }
        else if ((index > 10 && index < 19) || index > 28)
        {
            cellColour = (index % 2 == 0) ? "red" : "black";
        }
        else
        {
            cellColour = "black";
        }

        PrintRouletteCellWithColour(cellText, consoleHandle, defaultTextAttributes, cellColour);
    }
}

int RouletteGame::ourTotalWins = 0;
int RouletteGame::ourTotalLosses = 0;
const int RouletteGame::ourPayoutMultiplier = 3;

int RouletteGame::GetWinnings() const
{
    return myWinnings;
}

int RouletteGame::GetTotalWins()
{
    return ourTotalWins;
}

int RouletteGame::GetTotalLosses()
{
    return ourTotalLosses;
}

int RouletteGame::GetPayoutMultiplier()
{
    return ourPayoutMultiplier;
}

CasinoHelpers::GameState RouletteGame::Play(std::mt19937& aGenerator,
    int& somePlayerMoney,
    int& aPlayerBet,
    CasinoHelpers::StatHistory& aStatHistory,
    const std::string& aPlayerName)
{
    int seeInstructions = GetInput(globalChoiceNo, globalChoiceYes,
        "Would you like to see the instructions? (0: No, 1: Yes)",
        "Please enter 0 for No or 1 for Yes.");
    if (seeInstructions)
    {
        ShowInstructions(GameState::Roulette, aPlayerName);
    }

    int acceptRules = GetInput(globalChoiceNo, globalChoiceYes,
        "Do you want to continue? (0: No, 1: Yes)",
        "Please enter 0 for No or 1 for Yes.");
    if (!acceptRules)
    {
        return GameState::Menu;
    }

    int playAgain = globalPlayAgainYes;
    int lossStreak = 0;
    int winCounter = 0;
    while (playAgain)
    {
        if (somePlayerMoney <= 0)
        {
            return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
        }

        if (RecognizePlayer(GameState::Roulette, 0, 0, 0, 0, myWinnings, aPlayerName))
        {
            return GameState::Menu;
        }

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        Bet(somePlayerMoney, aPlayerBet, aPlayerName);
        DisplayRouletteBoard();
        const int betAmount = aPlayerBet;

        std::cout << "Roulette: Choose your bet type.\n";
        std::cout << "1. Straight up\n2. Split\n3. Corner\n4. Red/Black\n5. Odd/Even\n6. Column\n";
        int betTypeChoice = GetInput(1, 6,
            "Select bet type (1-6):",
            "Please enter a number between 1 and 6.");

        std::uniform_int_distribution<int> rouletteDistribution(0, 36);
        int rouletteResult = rouletteDistribution(aGenerator);
        bool playerWon = false;

        switch (betTypeChoice)
        {
        case 1:
        {
            int straightGuess = GetInput(0, 36,
                "Enter the number you want to bet on (0-36):",
                "Please enter a number between 0 and 36.");
            playerWon = (straightGuess == rouletteResult);
            break;
        }
        case 2:
        {
            int splitFirst = GetInput(0, 36,
                "Enter the first number (0-36):",
                "Please enter a number between 0 and 36.");
            int splitSecond = GetInput(0, 36,
                "Enter the second number (must be adjacent):",
                "Please enter a number between 0 and 36.");
            playerWon = (splitFirst == rouletteResult || splitSecond == rouletteResult);
            break;
        }
        case 3:
        {
            int cornerStart = GetInput(1, 32,
                "Enter the top-left number of the corner (1-32):",
                "Please enter a number between 1 and 32.");
            playerWon = (rouletteResult == cornerStart ||
                rouletteResult == cornerStart + 1 ||
                rouletteResult == cornerStart + 3 ||
                rouletteResult == cornerStart + 4);
            break;
        }
        case 4:
        {
            int colourChoice = GetInput(0, 1,
                "Bet on colour (0: Black, 1: Red):",
                "Please enter 0 for Black or 1 for Red.");
            bool isRed = (rouletteResult > 0 &&
                ((rouletteResult <= 10 || (rouletteResult >= 19 && rouletteResult <= 28))
                    ? rouletteResult % 2 != 0
                    : rouletteResult % 2 == 0));
            playerWon = (colourChoice == 1 && isRed) || (colourChoice == 0 && !isRed);
            break;
        }
        case 5:
        {
            int oddEvenChoice = GetInput(0, 1,
                "Bet on (0: Even, 1: Odd):",
                "Please enter 0 for Even or 1 for Odd.");
            playerWon = (rouletteResult != 0 &&
                ((oddEvenChoice == 1 && rouletteResult % 2 != 0) ||
                    (oddEvenChoice == 0 && rouletteResult % 2 == 0)));
            break;
        }
        case 6:
        {
            int columnChoice = GetInput(1, 3,
                "Bet on column (1-3):",
                "Please enter 1, 2, or 3.");
            if (rouletteResult != 0)
            {
                int columnIndex = ((rouletteResult - 1) % 3) + 1;
                playerWon = (columnIndex == columnChoice);
            }
            else
            {
                playerWon = false;
            }
            break;
        }
        default:
            break;
        }

        DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
        std::cout << "Roulette result: " << rouletteResult << "\n";

        if (playerWon)
        {
            lossStreak = 0;
            ++winCounter;
            const int payout = betAmount * ourPayoutMultiplier;
            HandlePlayerMoney(somePlayerMoney, aPlayerBet, payout);
            UpdatePlayerStatHistory(aStatHistory, payout);
            myWinnings += payout;
            ++ourTotalWins;
            std::cout << aPlayerName << ", you win " << (payout - betAmount) << " chips!\n";
        }
        else
        {
            winCounter = 0;
            ++lossStreak;
            UpdatePlayerStatHistory(aStatHistory, -betAmount);
            myWinnings -= betAmount;
            ++ourTotalLosses;
            std::cout << "No luck this round, " << aPlayerName << ". You lose " << betAmount << " chips.\n";
            aPlayerBet = 0;
            system("pause");
            if (somePlayerMoney <= 0)
            {
                return HandleBankruptcy(somePlayerMoney, aStatHistory, aPlayerName);
            }
        }

        playAgain = GetInput(globalPlayAgainNo, globalPlayAgainYes,
            "Play again? (0: No, 1: Yes): ",
            "Please enter 0 for No or 1 for Yes.");
        if (playAgain)
        {
            DrawHud(somePlayerMoney, aStatHistory, aPlayerName);
            std::cout << "Starting a new round" << (playerWon ? ", winner!\n\n" : ".\n\n");
        }
        else
        {
            std::cout << "\nExiting to menu, " << aPlayerName << ".\n";
        }
    }

    return GameState::Menu;
}
